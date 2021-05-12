#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "menuform.h"
#include "calibrationform.h"
#include "homographyform.h"
#include "drawingform.h"
#include <opencv2/core.hpp>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    calibData = new CalibrationData();

    menuform = new MenuForm(this,calibData);
    calibForm = new CalibrationForm(this,calibData);
    homogForm = new HomographyForm(this,calibData);
    drawForm = new DrawingForm(this,calibData);

    ui->mainWindowWidget-> layout()->addWidget(menuform);
    ui->mainWindowWidget-> layout()->addWidget(calibForm);
    ui->mainWindowWidget-> layout()->addWidget(homogForm);
    ui->mainWindowWidget-> layout()->addWidget(drawForm);

    menuform->show();
    calibForm->hide();
    homogForm->hide();
    drawForm->hide();

    menuform->checkDataStatus();

    connect(menuform, &MenuForm::signalShowCalibrationForm, this, &MainWindow::showCalibrationForm);
    connect(menuform, &MenuForm::signalShowHomographyForm, this, &MainWindow::showHomographyForm);
    connect(menuform, &MenuForm::signalShowDrawingForm, this, &MainWindow::showDrawingForm);
    connect(calibForm, &CalibrationForm::signalShowMenuForm, this, &MainWindow::showMenuForm);
    connect(homogForm, &HomographyForm::signalShowMenuForm, this, &MainWindow::showMenuForm);
    connect(drawForm, &DrawingForm::signalShowMenuForm, this, &MainWindow::showMenuForm);
    connect(menuform, &MenuForm::signalSetMainWindowName, this, &MainWindow::setMainWindowName);

}

void MainWindow::resizeEvent(QResizeEvent *event){

    QMainWindow::resizeEvent(event);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete calibData;
}

void MainWindow::showCalibrationForm()
{
    menuform->hide();
    calibForm->show();
    homogForm->hide();
    drawForm->hide();


}

void MainWindow::showHomographyForm()
{
    menuform->hide();
    calibForm->hide();
    homogForm->show();
    drawForm->hide();
    drawForm->disconnectSignals();
    homogForm->connectSignals();
}

void MainWindow::showDrawingForm()
{
    menuform->hide();
    calibForm->hide();
    homogForm->hide();
    drawForm->show();
    drawForm->connectSignals();
    homogForm->disconnectSignals();
}

void MainWindow::showMenuForm()
{
    menuform->show();
    calibForm->hide();
    homogForm->hide();
    drawForm->hide();
    menuform->checkDataStatus();

}

void MainWindow::setMainWindowName(std::string projectName){

    this->setWindowTitle(QString::fromStdString("Image calibration - Project: " + projectName));

}





