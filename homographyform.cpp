#include "homographyform.h"
#include "ui_homographyform.h"
#include "calibrationdata.h"

#include <iostream>

HomographyForm::HomographyForm(QWidget *parent, CalibrationData *calibData) :
    QWidget(parent),
    ui(new Ui::homographyForm)
{

    ui->setupUi(this);
    m_calibrationData = calibData;
    m_imageWidget = new ImageLabel(this);
    m_mappingWidget = new MappingLabel(this);
    ui->imageLayout->addWidget(m_imageWidget);
    ui->mappingLayout->addWidget(m_mappingWidget);
    ui->statusLabel->setStyleSheet(QStringLiteral("QLabel{color: rgb(255, 0, 0);}"));
    fillComboBox();

    ui->improvePointsButton->setEnabled(false);
    ui->calculateHomographyButton->setEnabled(false);
    ui->selectSportBox->setEnabled(false);
    ui->undistortButton->setEnabled(false);

    ui->pointsScaleFactorLine->hide();
    ui->pointsScaleFactorEdit->hide();
    ui->pointsScaleFactorEdit->setText("10");
    hideSizeForm();
}

HomographyForm::~HomographyForm()
{
    delete ui;
    delete m_imageWidget;
    delete m_mappingWidget;
}

void HomographyForm::disconnectSignals(){

    disconnect(m_mappingWidget, &MappingLabel::signalMappingPoint, nullptr, nullptr);
    disconnect(m_mappingWidget, &MappingLabel::signalClearMappingPont, nullptr, nullptr);
    disconnect(m_imageWidget, &ImageLabel::signalImagePoint, nullptr, nullptr);
    disconnect(m_imageWidget, &ImageLabel::signalClearImagePoint, nullptr, nullptr);
    m_imageWidget->activateImageClick();

}

void HomographyForm::connectSignals(){

    connect(m_mappingWidget,&MappingLabel::signalMappingPoint,this, &HomographyForm::storeMappingPoint, Qt::UniqueConnection);
    connect(m_mappingWidget,&MappingLabel::signalMappingPoint,m_imageWidget, &ImageLabel::activateImageClick, Qt::UniqueConnection);
    connect(m_imageWidget,&ImageLabel::signalImagePoint,this, &HomographyForm::storeImagePoint, Qt::UniqueConnection);
    connect(m_mappingWidget,&MappingLabel::signalClearMappingPont,this, &HomographyForm::clearMappingPoint, Qt::UniqueConnection);
    connect(m_imageWidget,&ImageLabel::signalClearImagePoint,this, &HomographyForm::clearMappingPoint, Qt::UniqueConnection);
    m_imageWidget->deactivateImageClick();
}

void HomographyForm::clearMappingPoint(){

    m_mappingPoint = {-1,-1};

    m_imageWidget->deactivateImageClick();
    m_mappingWidget->setEnabledClick(true);

    m_mappingWidget->displayPoints(m_mappingPoints,true);

}

void HomographyForm::mousePressEvent(QMouseEvent *event){

    if(event->button() == Qt::RightButton){

        clearMappingPoint();

    }

}

void HomographyForm::on_loadImageButton_clicked()
{

    QString imagePath = QFileDialog::getOpenFileName(this, tr("Select image"), qApp->applicationDirPath(),"Image file (*.jpg)");
    std::string path = imagePath.toStdString();
    m_imageWidget->loadBackgroundImage(path);
    m_homographyMatrix = m_calibrationData->getHomographyMatrix().clone();
    ui->selectSportBox->setEnabled(true);
    ui->undistortButton->setEnabled(true);

}

void HomographyForm::on_mainMenuButton_clicked()
{
    emit signalShowMenuForm();
}

void HomographyForm::on_saveHomographyButton_clicked()
{
    if(m_homographyMatrix.empty()){
        ui->statusLabel->setText("Nothing to save");
        return;
    }
    ui->statusLabel->setText("Saved");
    m_calibrationData->setHomographyMatrix(m_homographyMatrix);
    m_calibrationData->setPixelDensity(m_pointManager->computePixelDensity());

    if(ui->selectSportBox->currentIndex() == 7){

        m_calibrationData->setWindowSize({600,400});

    }else if (ui->selectSportBox->currentIndex() == 6){

        m_calibrationData->setWindowSize({static_cast<int>(60*m_pointsScaleFactor),static_cast<int>(30*m_pointsScaleFactor)});

    }else{

        m_calibrationData->setWindowSize(m_pointManager->getWindowSize());

    }

    m_calibrationData->saveCalibrationData();
}

void HomographyForm::fillComboBox(){

    ui->selectSportBox->addItem(QString("No sport selected"));
    ui->selectSportBox->addItem(QString("Football"));
    ui->selectSportBox->addItem(QString("Volleyball"));
    ui->selectSportBox->addItem(QString("Basketball"));
    ui->selectSportBox->addItem(QString("Badminton"));
    ui->selectSportBox->addItem(QString("Tennis"));
    ui->selectSportBox->addItem(QString("Hockey"));
    ui->selectSportBox->addItem(QString("Custom points"));

}

void HomographyForm::on_selectSportBox_activated(int index)
{
    switch(index){
    case 0:
        hideSizeForm();
        break;
    case 1: //Football
        ui->heightLabel->setText(QString("Height: "));
        ui->heightLabel->show();
        ui->widthLabel->setText(QString("Width: "));
        ui->widthLabel->show();
        ui->widthLineEdit->show();
        ui->heightLineEdit->show();
        ui->sizeButton->setText(QString("Set"));
        ui->sizeButton->show();
        ui->pointsScaleFactorLine->show();
        ui->pointsScaleFactorEdit->show();
        m_mappingWidget->setEnabledClick(false);

        break;

    case 7: //Custom
        ui->heightLabel->setText(QString("Y: "));
        ui->heightLabel->show();
        ui->widthLabel->setText(QString("X: "));
        ui->widthLabel->show();
        ui->widthLineEdit->show();
        ui->heightLineEdit->show();
        ui->sizeButton->setText(QString("Add"));
        ui->sizeButton->show();
        ui->clearPointsButton->setText("Clear points");
        ui->clearPointsButton->show();
        m_mappingWidget->setIsCustom(true);

        break;

    default:
        hideSizeForm();
        ui->pointsScaleFactorLine->show();
        ui->pointsScaleFactorEdit->show();
        ui->sizeButton->setText(QString("Set"));
        ui->sizeButton->show();
        m_mappingWidget->setEnabledClick(false);

        break;
    }

}

void HomographyForm::storeMappingPoint(){

    m_mappingPoint = m_mappingWidget->getMappingPoint();
    m_mappingWidget->resetMappingPoint();
    m_mappingWidget->displayPoints({m_mappingPoint});

}

void HomographyForm::storeImagePoint(){

    m_imagePoint = m_imageWidget->getImagePoint();
    m_imageWidget->resetImagePoint();

    if(m_mappingPoint.x != -1 && m_mappingPoint.y != -1 && m_imagePoint.x != -1 && m_imagePoint.y != -1 ){

        m_pointManager->addUserPoint(m_imagePoint,m_mappingPoint);

    }

    m_imagePoint = {-1,-1};
    m_mappingPoint = {-1,-1};



    m_pointManager->copyImageMappingPoints(m_imagePoints,m_mappingPoints);

    m_imageWidget->deactivateImageClick();
    m_mappingWidget->setEnabledClick(true);
    ui->statusLabel->setText("Pair of points added");
    ui->calculateHomographyButton->setEnabled(true);
    ui->improvePointsButton->setEnabled(true);

    m_mappingWidget->displayPoints(m_mappingPoints,true);
    m_imageWidget->displayPoints(m_imagePoints);

}

void HomographyForm::on_sizeButton_clicked()
{

    m_pointsScaleFactor = ui->pointsScaleFactorEdit->text().toFloat();

    if(m_pointsScaleFactor == 0){

        m_pointsScaleFactor = 1.0f;
        m_mappingWidget->setPointScaleFactor(m_pointsScaleFactor);

    }
    m_mappingWidget->setPointScaleFactor(m_pointsScaleFactor);

    //Football
    if(ui->selectSportBox->currentIndex() == 1){
        if(ui->widthLineEdit->text().isEmpty() == false && ui->heightLineEdit->text().isEmpty() == false){

            m_mappingWidget->setEnabledClick(true);

            m_mappingWidget->setWidth(ui->widthLineEdit->text().toFloat());
            m_mappingWidget->setHeight(ui->heightLineEdit->text().toFloat());

            m_pointManager = PointManager::createForFootball(ui->widthLineEdit->text().toFloat(),ui->heightLineEdit->text().toFloat(),{m_pointsScaleFactor,m_pointsScaleFactor});
            m_mappingWidget->setInputMappingPoints(m_pointManager->getMappingPoints());

            m_mappingWidget->loadFromQrc(QString(":/Resources/Football.jpg"));

            m_mappingWidget->setIsFootball(true);
            m_mappingWidget->displayPoints();

            ui->statusLabel->setText("Size set");

        }

        ui->statusLabel->setText("Size set");
    }else if(ui->selectSportBox->currentIndex() == 2){
        m_mappingWidget->setEnabledClick(true);
        m_pointManager = PointManager::createForVolleyball({m_pointsScaleFactor,m_pointsScaleFactor});
        m_mappingWidget->setInputMappingPoints(m_pointManager->getMappingPoints());;
        m_mappingWidget->loadFromQrc(QString(":/Resources/VolleyballCourt.jpg"),true);
        m_mappingWidget->setWidth(18.0f);
        m_mappingWidget->setHeight(9.0f);
        m_mappingWidget->displayPoints();

    }
    else if(ui->selectSportBox->currentIndex() == 3){
        m_mappingWidget->setEnabledClick(true);
        m_pointManager = PointManager::createForBasketball({m_pointsScaleFactor,m_pointsScaleFactor});
        m_mappingWidget->setInputMappingPoints(m_pointManager->getMappingPoints());;
        m_mappingWidget->loadFromQrc(QString(":/Resources/Basketball.jpg"),true);
        m_mappingWidget->setWidth(28.0f);
        m_mappingWidget->setHeight(15.0f);
        m_mappingWidget->displayPoints();

        }
    else if(ui->selectSportBox->currentIndex() == 4){
        m_mappingWidget->setEnabledClick(true);
        m_pointManager = PointManager::createForBadminton({m_pointsScaleFactor,m_pointsScaleFactor});
        m_mappingWidget->setInputMappingPoints(m_pointManager->getMappingPoints());
        m_mappingWidget->loadFromQrc(QString(":/Resources/Badminton.jpg"));
        m_mappingWidget->setWidth(13.4f);
        m_mappingWidget->setHeight(6.1f);
        m_mappingWidget->displayPoints();

        }
    else if(ui->selectSportBox->currentIndex() == 5){
        m_mappingWidget->setEnabledClick(true);
        m_pointManager = PointManager::createForTennis({m_pointsScaleFactor,m_pointsScaleFactor});
        m_mappingWidget->setInputMappingPoints(m_pointManager->getMappingPoints());
        m_mappingWidget->loadFromQrc(QString(":/Resources/Tennis.jpg"));
        m_mappingWidget->setWidth(23.770f);
        m_mappingWidget->setHeight(10.970f);
        m_mappingWidget->displayPoints();
        }
    else if(ui->selectSportBox->currentIndex() == 6){
        m_mappingWidget->setEnabledClick(true);
        m_pointManager = PointManager::createForHockey({m_pointsScaleFactor,m_pointsScaleFactor});
        m_mappingWidget->setInputMappingPoints(m_pointManager->getMappingPoints());
        m_mappingWidget->loadFromQrc(QString(":/Resources/Hockey123.jpg"));
        m_mappingWidget->setWidth(60.0f);
        m_mappingWidget->setHeight(30.0f);
        m_mappingWidget->displayPoints();

        }
    else if(ui->selectSportBox->currentIndex() == 7){
        //Custom
        m_pointManager = PointManager::createCustom();
        cv::Mat customImage(400,600,CV_8UC3,{1,1,1});
        m_mappingWidget->setBackgroundImage(customImage);

        m_mappingWidget->setWidth(600.0f);
        m_mappingWidget->setHeight(400.0f);

        bool areEqual = false;

        for(uint x = 0; x < m_customPoints.size(); x++){

         if (ui->widthLineEdit->text().toFloat() == m_customPoints[x].x && ui->heightLineEdit->text().toFloat() == m_customPoints[x].y){

             areEqual = true;

         }

        }
        if(!areEqual){

            m_customPoints.push_back({ui->widthLineEdit->text().toFloat(),ui->heightLineEdit->text().toFloat()});
            ui->statusLabel->setText("Coordinates added");

        }else{

            ui->statusLabel->setText("Cannot add the same coordinates");

        }


        m_mappingWidget->setInputMappingPoints(m_customPoints);
        m_mappingWidget->displayPoints();
    }
}

void HomographyForm::hideSizeForm(){

    ui->heightLabel->hide();
    ui->widthLabel->hide();
    ui->widthLineEdit->hide();
    ui->heightLineEdit->hide();
    ui->sizeButton->hide();
    ui->clearPointsButton->setText("Clear");
    m_mappingWidget->setIsFootball(false);
    m_mappingWidget->setIsCustom(false);

}

void HomographyForm::on_clearPointsButton_clicked()
{

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Clear points", "Are you sure you want to delete all points ?", QMessageBox::Yes|QMessageBox::No);

      if (reply == QMessageBox::Yes) {

          m_pointManager->clearUserPoints();
          m_mappingPoint = {-1,-1};
          m_imagePoint = {-1,-1};
          m_imagePoints = std::vector<cv::Point2f>();
          m_mappingPoints = std::vector<cv::Point2f>();
          m_mappingWidget->setEnabledClick(true);
          if(ui->selectSportBox->currentIndex() == 7){

              m_mappingWidget->setInputMappingPoints(m_mappingPoints);

          }
          m_imageWidget->deactivateImageClick();
          m_pointManager->clearUserPoints();
          ui->statusLabel->setText("Points cleared");
          m_mappingWidget->displayPoints();
          m_imageWidget->displayPoints();
          m_customPoints = std::vector<cv::Point2f>();

      }



}

void HomographyForm::on_calculateHomographyButton_clicked()
{

    if(m_pointManager->getUserPoints().size() < 4){
        ui->statusLabel->setText("Not enough points");
        return;

    }

    m_homography = std::make_shared<Homography>(*m_pointManager);

    if(checkPointsAxis(m_imagePoints)){

        ui->statusLabel->setText("Points in image cannot be on only one axis");

    }else if(checkPointsAxis(m_mappingPoints)){

        ui->statusLabel->setText("Mapping points cannot be on only one axis");

    }else {

        ui->statusLabel->setText("Matrix calculated");

    }

    m_homographyMatrix = m_homography->getHomographyMatrix();
}

void HomographyForm::on_improvePointsButton_clicked()
{
    if(m_pointManager->getUserPoints().size() < 4){
        ui->statusLabel->setText("Not enough points");
        return;

    }

    m_pointManager->improvePoints(m_imageWidget->getBackgroundImage());

    m_imagePoints = {};
    m_mappingPoints = {};
    m_pointManager->copyImageMappingPoints(m_imagePoints,m_mappingPoints);
    m_imageWidget->displayPoints(m_imagePoints);

    ui->statusLabel->setText("Points improved");
    m_mappingWidget->setEnabledClick(false);

}

void HomographyForm::on_showBirdsEyeBox_stateChanged(int arg1)
{
    if(arg1 == 0){

        m_imageWidget->setBackgroundImage(m_imageWidget->getBackgroundImage());

    }else if(arg1 == 2){
        if(!m_homographyMatrix.empty()){
            m_imageWidget->setBackgroundImage(computeBirdsEyeView(m_homographyMatrix,m_imageWidget->getBackgroundImage(),{static_cast<int>(m_mappingWidget->getWidth()*m_mappingWidget->getPointScaleFactor()),static_cast<int>(m_mappingWidget->getHeight()*m_mappingWidget->getPointScaleFactor())}).clone());
        }else{ui->statusLabel->setText("No homography matrix found");}
    }
}

void HomographyForm::on_undistortButton_clicked()
{
    if(m_imageWidget->getBackgroundImage().empty()){

        return;

    }

    m_imageWidget->changeDefaultBackgroundImage(undistort(m_imageWidget->getBackgroundImage(),m_calibrationData->getDistoritonCoefficient(),m_calibrationData->getScaleCoefficient()));
    ui->statusLabel->setText("Image undistorted");

}

bool HomographyForm::checkPointsAxis(std::vector<cv::Point2f> inputPoints){

    int numberOfSamePoints = 0;

    float firstXPoint = inputPoints[0].x;
    float firstYPoint = inputPoints[0].y;

    for(uint x = 1; x < inputPoints.size(); x++){

        if(inputPoints[x].x == firstXPoint || inputPoints[x].y == firstYPoint){

            numberOfSamePoints++;

        }

    }

    if(numberOfSamePoints + 1 == static_cast<int>(inputPoints.size())){

        return true;

    }else{return false;}

}
