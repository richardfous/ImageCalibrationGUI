#include "menuform.h"
#include "ui_menuform.h"
#include "ui_mainwindow.h"

#include <iostream>

MenuForm::MenuForm(QWidget *parent, CalibrationData *calibData) :
    QWidget(parent),
    ui(new Ui::menuForm)
{
    m_calibrationData = calibData;
    ui->setupUi(this);

}

MenuForm::~MenuForm()
{
    delete ui;
}

void MenuForm::on_distortionCalibrationButton_clicked()
{
    emit signalShowCalibrationForm();
}

void MenuForm::on_perspectiveCalibration_clicked()
{
    emit signalShowHomographyForm();
}

void MenuForm::on_drawingButton_clicked()
{
    emit signalShowDrawingForm();
}

void MenuForm::on_loadProjectButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Select project file"), qApp->applicationDirPath(),"Text Files(*.txt)");
    std::string path = filePath.toStdString();

    if(QFile::exists(filePath))
    {
        QFileInfo fi(filePath);
        if(fi.suffix() == "txt"){
        m_calibrationData->setProjectPath(path);
        m_calibrationData->loadCalibrationData(path);
        }else{return;}
    }else{
        return;
    }

    checkDataStatus();

    emit signalSetMainWindowName(m_calibrationData->getProjectName());

    ui->saveProjectLine->setText(QString::fromStdString(m_calibrationData->getProjectName()));

}

void MenuForm::on_saveProjectButton_clicked()
{

    QString projectName = ui->saveProjectLine->text();
    std::string fileName;
    if(projectName.isEmpty()){
        fileName = "default";
    }else{fileName = projectName.toStdString();}

    m_calibrationData->setProjectName(fileName);

    QString filePath = QFileDialog::getExistingDirectory(this, tr("Select storage folder"), qApp->applicationDirPath());
    std::string path = filePath.toStdString();

    std::string pathToFile = path + "/" + fileName + ".txt";

    m_calibrationData->setProjectPath(pathToFile);

    m_calibrationData->saveCalibrationData();
    
    emit signalSetMainWindowName(m_calibrationData->getProjectName());

    checkDataStatus();

    ui->saveProjectLine->setText(QString::fromStdString(m_calibrationData->getProjectName()));


}

void MenuForm::checkDataStatus(){

    QPixmap redLed(":/Resources/red-light.png");
    QPixmap greenLed(":/Resources/green-light.png");

    if(m_calibrationData->getHomographyMatrix().empty()){

        ui->matrixStatusLight->setPixmap(redLed.scaled(ui->matrixStatusLight->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
        ui->drawingButton->setEnabled(false);
        ui->drawInfoLabel->setStyleSheet(QStringLiteral("QLabel{color: rgb(255, 0, 0);}"));
        ui->drawInfoLabel->setText("No homography matrix inserted.");

    }else{
        ui->matrixStatusLight->setPixmap(greenLed.scaled(ui->matrixStatusLight->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
        ui->drawingButton->setEnabled(true);
        ui->drawInfoLabel->setText("");
    }

    if(m_calibrationData->getDistoritonCoefficient() == 0){
        ui->coefficientsStatusLight->setPixmap(redLed.scaled(ui->matrixStatusLight->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }else{
         ui->coefficientsStatusLight->setPixmap(greenLed.scaled(ui->matrixStatusLight->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    if(m_calibrationData->getProjectName().empty()){

        ui->distortionCalibrationButton->setEnabled(false);
        ui->perspectiveCalibration->setEnabled(false);
        ui->loadInfoLabel->setStyleSheet(QStringLiteral("QLabel{color: rgb(255, 0, 0);}"));
        ui->loadInfoLabel->setText("No project loaded.");

    }else{

        ui->distortionCalibrationButton->setEnabled(true);
        ui->perspectiveCalibration->setEnabled(true);
        ui->loadInfoLabel->setText("");

    }

}
