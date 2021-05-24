#include "drawingform.h"
#include "ui_drawingform.h"

DrawingForm::DrawingForm(QWidget *parent, CalibrationData *calibData) :
    QWidget(parent),
    ui(new Ui::drawingForm)
{
    ui->setupUi(this);

    m_calibrationData = calibData;

    m_imageWidget = new ImageLabel(this);
    m_historyTable = new QTableWidget(this);

    ui->imageLayout->addWidget(m_imageWidget);

    ui->colorLabel->setAutoFillBackground(true);

    ui->statusLabel->setStyleSheet(QStringLiteral("QLabel{color: rgb(255, 0, 0);}"));

    ui->tableLable->addWidget(m_historyTable);
    m_historyTable->setColumnCount(4);
    m_historyTable->setHorizontalHeaderItem(0,new QTableWidgetItem(QString::fromStdString("Type")));
    m_historyTable->setHorizontalHeaderItem(1,new QTableWidgetItem(QString::fromStdString("Color")));
    m_historyTable->setHorizontalHeaderItem(2,new QTableWidgetItem(QString::fromStdString("Thickness")));
    m_historyTable->setHorizontalHeaderItem(3,new QTableWidgetItem(QString::fromStdString("")));
    m_historyTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    m_historyTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_historyTable->setFocusPolicy(Qt::NoFocus);
    m_historyTable->setSelectionMode(QAbstractItemView::NoSelection);

    displayColor(m_color,m_alpha);

    fillComboBox();

    ui->objectSelectorComboBox->setCurrentIndex(1);

    ui->insertImageButton->hide();
    ui->radiusLabel->hide();
    ui->radiusLineEdit->hide();
    ui->renderButton->setEnabled(false);
    ui->alphaLabel->hide();
    ui->alphaLineEdit->hide();
    ui->lineWidthEdit->setText("1");
    ui->rotationComboBox->hide();
    ui->rotationLabel->hide();
    ui->alphaLineEdit->setText("1.0");
    ui->radiusLineEdit->setText("1");
    ui->correctDistortionButton->setEnabled(false);

    ui->distanceUnitsComboBox->hide();
    ui->measureDistanceButton->hide();
    ui->measureDistanceLabel->hide();

    m_homography = std::make_shared<Homography>();
    m_homography->setHomographyMatrix(m_calibrationData->getHomographyMatrix());

}

DrawingForm::~DrawingForm()
{
    delete ui;
    delete m_imageWidget;
    delete m_historyTable;
}

void DrawingForm::disconnectSignals(){

    disconnect(m_imageWidget,&ImageLabel::signalImagePoint, nullptr, nullptr);
    disconnect(m_imageWidget,&ImageLabel::signalClearImagePoint, nullptr, nullptr);
    m_imageWidget->deactivateImageClick();

}

void DrawingForm::connectSignals(){

    connect(m_imageWidget,&ImageLabel::signalImagePoint,this, &DrawingForm::storeImagePoint, Qt::UniqueConnection);
    connect(m_imageWidget,&ImageLabel::signalClearImagePoint,this, &DrawingForm::clearImagePoint, Qt::UniqueConnection);
    connect(m_historyTable, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(cellDoubleClicked(int,int)), Qt::UniqueConnection);
    m_imageWidget->activateImageClick();
}

void DrawingForm::on_loadImageButton_clicked()
{
    QString imagePath = QFileDialog::getOpenFileName(this, tr("Select image"), qApp->applicationDirPath(),"Image file (*.jpg)");
    std::string path = imagePath.toStdString();
    m_imageWidget->loadBackgroundImage(path);
    m_inputImage = m_imageWidget->getBackgroundImage().clone();

    if(m_inputImage.empty()){
        ui->statusLabel->setText("Failed to load background image.");
        return;
    }

    ui->correctDistortionButton->setEnabled(true);
    ui->statusLabel->setText("Background image loaded");

    m_homography->setHomographyMatrix(m_calibrationData->getHomographyMatrix());

    m_renderer.setBackgroundImage(m_inputImage);

}

void DrawingForm::on_menuButton_clicked()
{

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Leave drawing", "Are you sure you want to leave to main menu ? All existng objects will be deleted !", QMessageBox::Yes|QMessageBox::No);

      if (reply == QMessageBox::Yes) {

         ui->renderButton->setEnabled(false);
         ui->statusLabel->setText("");
         m_historyTable->setRowCount(0);

         if(!m_inputImage.empty()){
             m_imagePoints = std::vector<cv::Point2f>();
             m_renderer.clearDrawables();
             m_renderer.render();
             m_imageWidget->setBackgroundImage(m_renderer.getOutputImage());
         }

      }else{

          return;

      }
    emit signalShowMenuForm();
}

void DrawingForm::storeImagePoint(){

    m_imagePoints.push_back(m_imageWidget->getImagePoint()) ;
    m_imageWidget->displayPoints(m_imagePoints);

    if(ui->objectSelectorComboBox->currentIndex() == 4 || ui->objectSelectorComboBox->currentIndex() == 5 || ui->objectSelectorComboBox->currentIndex() == 6){

        if(m_imagePoints.size() == 2){
            ui->renderButton->setEnabled(true);
            m_imageWidget->deactivateImageClick();
        }

    }else if(ui->objectSelectorComboBox->currentIndex() == 0){

        if(m_imagePoints.size() == 2){
            m_imageWidget->deactivateImageClick();
            ui->measureDistanceButton->setEnabled(true);
        }

    }else{

        ui->renderButton->setEnabled(true);
        m_imageWidget->deactivateImageClick();

    }

}

void DrawingForm::clearImagePoint(){

    m_imagePoints = std::vector<cv::Point2f>();
    m_renderer.render();
    m_imageWidget->setBackgroundImage(m_renderer.getOutputImage());
    ui->renderButton->setEnabled(false);
    m_imageWidget->activateImageClick();

}

void DrawingForm::on_correctDistortionButton_clicked()
{
    if(m_calibrationData->getDistoritonCoefficient() == 0.0 && m_calibrationData->getScaleCoefficient() == 1.0){
        return;
    }

    m_inputImage = undistort(m_inputImage,m_calibrationData->getDistoritonCoefficient(),m_calibrationData->getScaleCoefficient()).clone();

    m_imageWidget->changeDefaultBackgroundImage(m_inputImage);

    m_renderer.setBackgroundImage(m_inputImage);

}

void DrawingForm::on_objectSelectorComboBox_activated(int index)
{
    ui->statusLabel->setText("");

    switch (index) {

    case 0:
        ui->distanceUnitsComboBox->show();
        ui->measureDistanceButton->show();
        ui->measureDistanceLabel->show();
        ui->measureDistanceButton->setEnabled(false);
        ui->renderButton->hide();
        ui->insertImageButton->hide();
        ui->radiusLabel->hide();
        ui->radiusLineEdit->hide();
        ui->rotationComboBox->hide();
        ui->rotationLabel->hide();
        ui->alphaLabel->hide();
        ui->alphaLineEdit->hide();
        clearImagePoint();
        break;

    case 3: //Circle
        ui->distanceUnitsComboBox->hide();
        ui->measureDistanceButton->hide();
        ui->measureDistanceLabel->hide();
        ui->renderButton->show();
        ui->insertImageButton->hide();
        ui->rotationComboBox->hide();
        ui->rotationLabel->hide();
        ui->alphaLabel->hide();
        ui->alphaLineEdit->hide();
        ui->radiusLabel->show();
        ui->radiusLineEdit->show();
        clearImagePoint();
        break;

    case 6: //Image
        ui->distanceUnitsComboBox->hide();
        ui->measureDistanceButton->hide();
        ui->measureDistanceLabel->hide();
        ui->renderButton->show();
        ui->insertImageButton->show();
        ui->alphaLabel->show();
        ui->alphaLineEdit->show();
        ui->rotationComboBox->show();
        ui->rotationLabel->show();
        clearImagePoint();

        break;
    default:
        ui->distanceUnitsComboBox->hide();
        ui->measureDistanceButton->hide();
        ui->measureDistanceLabel->hide();
        ui->insertImageButton->hide();
        ui->renderButton->show();
        ui->radiusLabel->hide();
        ui->radiusLineEdit->hide();
        ui->rotationComboBox->hide();
        ui->rotationLabel->hide();
        ui->alphaLabel->hide();
        ui->alphaLineEdit->hide();
        clearImagePoint();

        break;
    }
}

void DrawingForm::fillComboBox(){

    ui->objectSelectorComboBox->addItem(QString("Measure distance"));
    ui->objectSelectorComboBox->addItem(QString("Horizontal line"));
    ui->objectSelectorComboBox->addItem(QString("Vertical line"));
    ui->objectSelectorComboBox->addItem(QString("Circle"));
    ui->objectSelectorComboBox->addItem(QString("Square"));
    ui->objectSelectorComboBox->addItem(QString("Rectangle"));
    ui->objectSelectorComboBox->addItem(QString("Image"));

    ui->rotationComboBox->addItem("Automatic");
    ui->rotationComboBox->addItem("90°");
    ui->rotationComboBox->addItem("180°");
    ui->rotationComboBox->addItem("270°");

    ui->distanceUnitsComboBox->addItem("m");
    ui->distanceUnitsComboBox->addItem("cm");
    ui->distanceUnitsComboBox->addItem("mm");

}

void DrawingForm::displayColor(cv::Scalar color, float m_alpha){

    QPalette pal = ui->colorLabel->palette();
    pal.setColor(QPalette::Window, QColor({static_cast<int>(color[2]),static_cast<int>(color[1]),static_cast<int>(color[0]),static_cast<int>(m_alpha*255)}));
    ui->colorLabel->setPalette(pal);

}

void DrawingForm::on_setColorButton_clicked()
{
    ui->statusLabel->setText("");

    QColor color = QColorDialog::getColor({0,0,0},this,{"Select object color"},QColorDialog::ShowAlphaChannel);

    if(color.isValid()){

        m_color = {static_cast<double>(color.blue()),static_cast<double>(color.green()),static_cast<double>(color.red())};
        m_alpha = color.alphaF();

        displayColor(m_color,m_alpha);

    }
}

void DrawingForm::insertTableRow(QString type, cv::Scalar color, float alpha, int thickness){

    m_historyTable->insertRow(m_historyTable->rowCount());

    QTableWidgetItem* objectType = new QTableWidgetItem(type);

    objectType->setFlags(objectType->flags() ^ Qt::ItemIsEditable);

    m_historyTable->setItem(m_historyTable->rowCount()-1,0, objectType);

    QTableWidgetItem* showColor = new QTableWidgetItem();

    showColor->setFlags(showColor->flags() ^ Qt::ItemIsEditable);

    m_historyTable->setItem(m_historyTable->rowCount()-1,1,showColor);

    m_historyTable->item(m_historyTable->rowCount()-1,1)->setBackground(QBrush(QColor({static_cast<int>(color[2]),static_cast<int>(color[1]),static_cast<int>(color[1]),static_cast<int>(alpha*255)})));

    QTableWidgetItem* showThickness = new QTableWidgetItem(QString::number(thickness));

    showThickness->setFlags(showThickness->flags() ^ Qt::ItemIsEditable);

    m_historyTable->setItem(m_historyTable->rowCount()-1,2,showThickness);

    QTableWidgetItem* deleteItem = new QTableWidgetItem("DEL");

    deleteItem->setFlags(deleteItem->flags() ^ Qt::ItemIsEditable);

    m_historyTable->setItem(m_historyTable->rowCount()-1,3,deleteItem);




}

void DrawingForm::on_renderButton_clicked()
{

    ui->statusLabel->setText("");

    int thickness = ui->lineWidthEdit->text().toInt();

    if(thickness < 1){

        thickness = 1;

    }

    if(ui->objectSelectorComboBox->currentIndex() == 1){

        auto line = Line::create(m_homography, Line::Type::Horizontal, { m_imagePoints[0].x, m_imagePoints[0].y }, m_calibrationData->getWindowSize());

        line->setAlpha(m_alpha);
        line->setColor(m_color);
        line->setThickness(thickness);

        insertTableRow({"Horizontal"},line->getColor(),line->getAlpha(),line->getThickness());

        m_renderer.addDrawable(std::move(line));

    }else if(ui->objectSelectorComboBox->currentIndex() == 2){

        auto line = Line::create(m_homography, Line::Type::Vertical, { m_imagePoints[0].x, m_imagePoints[0].y }, m_calibrationData->getWindowSize());

        line->setAlpha(m_alpha);
        line->setColor(m_color);
        line->setThickness(thickness);

        insertTableRow({"Vertical"},line->getColor(),line->getAlpha(),line->getThickness());

        m_renderer.addDrawable(std::move(line));

    }else if(ui->objectSelectorComboBox->currentIndex() == 3){

        int radius = ui->radiusLineEdit->text().toInt();

        if(radius < 1){

            radius = 1;

        }

        auto circle = Circle::create(m_homography,{ m_imagePoints[0].x, m_imagePoints[0].y }, radius);

        circle->setAlpha(m_alpha);
        circle->setColor(m_color);
        circle->setThickness(thickness);

        insertTableRow({"Horizontal"},circle->getColor(),circle->getAlpha(),circle->getThickness());

        m_renderer.addDrawable(std::move(circle));

    }else if(ui->objectSelectorComboBox->currentIndex() == 4){

        auto rectangle = Rectangle::create(m_homography,Rectangle::Type::Square,{m_imagePoints[0].x,m_imagePoints[0].y},{m_imagePoints[1].x,m_imagePoints[1].y});

                rectangle->setAlpha(m_alpha);
                rectangle->setColor(m_color);
                rectangle->setThickness(thickness);

                insertTableRow({"Horizontal"},rectangle->getColor(),rectangle->getAlpha(),rectangle->getThickness());

                m_renderer.addDrawable(std::move(rectangle));

    }else if(ui->objectSelectorComboBox->currentIndex() == 5){

        auto rectangle = Rectangle::create(m_homography,Rectangle::Type::Rectangle,{m_imagePoints[0].x,m_imagePoints[0].y},{m_imagePoints[1].x,m_imagePoints[1].y});

                rectangle->setAlpha(m_alpha);
                rectangle->setColor(m_color);
                rectangle->setThickness(thickness);

                insertTableRow({"Horizontal"},rectangle->getColor(),rectangle->getAlpha(),rectangle->getThickness());

                m_renderer.addDrawable(std::move(rectangle));

    }else if(ui->objectSelectorComboBox->currentIndex() == 6){

        if(m_imageToInsert.empty()){

            return;

        }

        float opacity = ui->alphaLineEdit->text().toFloat();

        if(opacity < 0.0f){

            opacity = 1.0f;

        }else if(opacity > 1.0f){

            opacity = 1.0f;

        }
        Image::Rotation rotation;

        switch (ui->rotationComboBox->currentIndex()) {

        case 0:
            rotation = Image::Rotation::_0;
            break;
        case 1:
            rotation = Image::Rotation::_90;
            break;
        case 2:
            rotation = Image::Rotation::_180;
            break;
        case 3:
            rotation = Image::Rotation::_270;
            break;
        }


        auto image = Image::create(m_homography, {m_imagePoints[0].x,m_imagePoints[0].y},{m_imagePoints[1].x,m_imagePoints[1].y},m_imageToInsert, rotation);

                image->setAlpha(opacity);

                insertTableRow({"Image"},{},{},{});

                m_renderer.addDrawable(std::move(image));
    }

    clearImagePoint();
}

void DrawingForm::on_insertImageButton_clicked()
{
    QString imagePath = QFileDialog::getOpenFileName(this, tr("Select image"), qApp->applicationDirPath(),"Image file (*.jpg *.png)");
    std::string path = imagePath.toStdString();

    m_imageToInsert = cv::imread(path,cv::IMREAD_UNCHANGED);

    if(m_imageToInsert.empty()){
        ui->statusLabel->setText("Failed to load image");
        return;

    }
        ui->statusLabel->setText("Image loaded");

}

void DrawingForm::on_clearButton_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Delete objects", "Are you sure you want to delete all objects ?", QMessageBox::Yes|QMessageBox::No);

      if (reply == QMessageBox::Yes) {

          m_imagePoints = std::vector<cv::Point2f>();
          m_renderer.clearDrawables();
          m_renderer.render();
          m_imageWidget->setBackgroundImage(m_renderer.getOutputImage());
          ui->renderButton->setEnabled(false);
          ui->statusLabel->setText("");
          m_historyTable->setRowCount(0);

      }else{

          return;

      }

}

void DrawingForm::on_saveImageButton_clicked()
{

    if(m_renderer.getOutputImage().empty()){
        return;
    }

     QString filePath = QFileDialog::getSaveFileName(this, tr("Save image"), qApp->applicationDirPath(),tr("JPEG (*.jpg)"));

     QString croped_fileName;

    if(!filePath.isEmpty()){

        if(!filePath.endsWith(".jpg")){

            croped_fileName=filePath.section(".",0,0);

            croped_fileName += ".jpg";

        }

    }else{
        ui->statusLabel->setText("Invalid file");
        return;
    }

    std::string path = croped_fileName.toStdString();

    cv::imwrite(path,m_renderer.getOutputImage());

    ui->statusLabel->setText("Image saved");

}

void DrawingForm::on_saveAlphaButton_clicked()
{
    if(m_renderer.getOutputImage().empty()){
        return;
    }

     QString filePath = QFileDialog::getSaveFileName(this, tr("Save image"), qApp->applicationDirPath(),tr("PNG (*.png)"));

     QString croped_fileName;

    if(!filePath.isEmpty()){

        if(!filePath.endsWith(".png")){

            croped_fileName=filePath.section(".",0,0);

            croped_fileName += ".png";

        }

    }else{
        ui->statusLabel->setText("Invalid file");
        return;
    }

    std::string path = croped_fileName.toStdString();

    cv::imwrite(path,m_renderer.getOutputImage(false));

    ui->statusLabel->setText("Image saved");
}

void DrawingForm::on_measureDistanceButton_clicked()
{
    float pixelDensity = m_calibrationData->getPixelDensity();

    float distance;

    if(m_imagePoints.size() > 1){

        distance = cv::norm(m_imagePoints[0]-m_imagePoints[1])*pixelDensity;

    }else{

        ui->statusLabel->setText("Two points needed to measure distance");
        clearImagePoint();
        return;

    }

    switch (ui->distanceUnitsComboBox->currentIndex()) {

    case 0:

        ui->measureDistanceLabel->setText(QString::number(distance));
        break;
    case 1:

        ui->measureDistanceLabel->setText(QString::number(distance*100));
        break;

    case 2:
        ui->measureDistanceLabel->setText(QString::number(distance*1000));
        break;

    }

    clearImagePoint();

}

void DrawingForm::cellDoubleClicked(int row, int column){

    std::vector<std::unique_ptr<Drawable>>& objects = m_renderer.getDrawables();

    if(m_historyTable->item(row,0)->text() != QString::fromStdString("Image")){

        if(column == 1){
        QColor color = QColorDialog::getColor({0,0,0},this,{"Select object color"},QColorDialog::ShowAlphaChannel);

            if(color.isValid()){

                cv::Scalar newColor = {static_cast<double>(color.blue()),static_cast<double>(color.green()),static_cast<double>(color.red())};
                float newAlpha = color.alphaF();

                objects[row]->setColor(newColor);
                objects[row]->setAlpha(newAlpha);

                m_historyTable->item(row,1)->setBackground(QBrush(QColor({static_cast<int>(newColor[2]),static_cast<int>(newColor[1]),static_cast<int>(newColor[1]),static_cast<int>(newAlpha*255)})));
                }

            }else if(column == 2){

                bool ok;
                int newThickness = QInputDialog::getInt(this, {"Insert new line thickness"}, {"Thickness: "}, 1, 1, 100, 1, &ok);

                if(ok){

                    objects[row]->setThickness(newThickness);
                    m_historyTable->item(row,2)->setText(QString::number(newThickness));

                }
            }
    }
    if(column == 3){

        QMessageBox::StandardButton reply = QMessageBox::question(this, "Delete object", "Are you sure you want to delete this object ?", QMessageBox::Yes|QMessageBox::No);

          if (reply == QMessageBox::Yes) {

              m_renderer.removeDrawable(objects[row].get());

              m_historyTable->removeRow(row);

          }

    }

    m_renderer.render();

    m_imageWidget->setBackgroundImage(m_renderer.getOutputImage());

}
