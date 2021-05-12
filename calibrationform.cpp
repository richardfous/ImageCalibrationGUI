#include "calibrationform.h"
#include "ui_calibrationform.h"
#include "imagelabel.h"



CalibrationForm::CalibrationForm(QWidget *parent, CalibrationData *calibData) :
    QWidget(parent),
    ui(new Ui::calibrationForm)
{
    ui->setupUi(this);
    m_calibrationData = calibData;
    ui->statusLabel->setStyleSheet(QStringLiteral("QLabel{color: rgb(255, 0, 0);}"));
    ui->coefitientKValueLabel->setText(QString::number(ui->coefKSlider->value()));
    ui->scaleValueLabel->setText(QString::number(ui->scaleSlider->value()/100));
}

CalibrationForm::~CalibrationForm()
{
    delete ui;
}

void CalibrationForm::on_loadcalibImageButton_clicked()
{

    QString imagePath = QFileDialog::getOpenFileName(this, tr("Select image"), qApp->applicationDirPath(),"Image file (*.jpg)");
    std::string path = imagePath.toStdString();
    m_backgroundImage = cv::imread(path);
    setBackgroundImage(m_backgroundImage);

}

void CalibrationForm::setBackgroundImage(cv::Mat input){

    cv::Mat image = input.clone();

    if(input.empty()){
        return;
    }

    cv::cvtColor(image,image,cv::COLOR_BGR2RGB);
    QPixmap newPixmap = QPixmap::fromImage(QImage(image.data, image.cols, image.rows, image.step, QImage::Format_RGB888));
    ui->imageLabel->setPixmap(newPixmap.scaled(ui->imageLabel->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
    this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

}

void CalibrationForm::on_mainMenuButton_clicked()
{
    emit signalShowMenuForm();
}

void CalibrationForm::on_saveCalibrationButton_clicked()
{
    m_calibrationData->setDistoritonCoefficient(m_coefficientK);
    m_calibrationData->setScaleCoefficient(m_scale);
    m_calibrationData->saveCalibrationData();
    ui->statusLabel->setText("Coefficients saved");
}

void CalibrationForm::on_coefKSlider_sliderReleased()
{
    m_coefficientK = static_cast<double>(ui->coefKSlider->value())/std::pow(10,8);
    ui->coefitientKValueLabel->setText(QString::number(m_coefficientK));

    m_correctedImage = undistort(m_backgroundImage,m_coefficientK,m_scale);

    setBackgroundImage(m_correctedImage);
}

void CalibrationForm::on_scaleSlider_sliderReleased()
{
    m_scale = static_cast<double>(ui->scaleSlider->value())/100;
    ui->scaleValueLabel->setText(QString::number(m_scale));

    cv::Mat temp = m_backgroundImage.clone();

    m_correctedImage = undistort(m_backgroundImage,m_coefficientK,m_scale);

    setBackgroundImage(m_correctedImage);
}

void CalibrationForm::on_saveImageButton_pressed()
{

    if(m_correctedImage.empty()){
        return;
    }

     QString filePath = QFileDialog::getSaveFileName(this, tr("Save undistorted image"), qApp->applicationDirPath(),tr("JPEG (*.jpg)"));

     QString croped_fileName;

    if(!filePath.isEmpty()){

        if(!filePath.endsWith(".jpg")){

            croped_fileName=filePath.section(".",0,0);

            croped_fileName += ".jpg";

        }

    }else{return;}

    std::string path = croped_fileName.toStdString();

    cv::imwrite(path,m_correctedImage);

}
