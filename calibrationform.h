#ifndef CALIBRATIONFORM_H
#define CALIBRATIONFORM_H

#include <QWidget>
#include <QFileDialog>
#include <QDebug>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include "calibrationdata.h"

#include "ImageCalibrationLibrary/include/utils.hpp"

namespace Ui {
class calibrationForm;
}

class CalibrationForm : public QWidget
{
    Q_OBJECT

public:

    explicit CalibrationForm(QWidget *parent = nullptr, CalibrationData *calibData = nullptr);

    ~CalibrationForm();

private slots:

    void on_loadcalibImageButton_clicked();

    void on_mainMenuButton_clicked();

    void on_saveCalibrationButton_clicked();

    void on_coefKSlider_sliderReleased();

    void on_scaleSlider_sliderReleased();

    void on_saveImageButton_pressed();

signals:

    void signalShowMenuForm();

private:

    CalibrationData *m_calibrationData;
    Ui::calibrationForm *ui;

    double m_coefficientK = 0.0;

    double m_scale = 1.0;

    cv::Mat m_backgroundImage;

    cv::Mat m_correctedImage;

protected:

    void setBackgroundImage(cv::Mat input);

};

#endif // CALIBRATIONFORM_H
