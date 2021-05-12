#ifndef MENUFORM_H
#define MENUFORM_H

#include <QWidget>
#include <QFileDialog>
#include "calibrationdata.h"

namespace Ui {
class menuForm;
}

class MenuForm : public QWidget
{
    Q_OBJECT

public:

    explicit MenuForm(QWidget *parent = nullptr, CalibrationData *calibData = nullptr);

    ~MenuForm();

    void checkDataStatus();

private slots:

    void on_distortionCalibrationButton_clicked();

    void on_perspectiveCalibration_clicked();

    void on_drawingButton_clicked();

    void on_loadProjectButton_clicked();

    void on_saveProjectButton_clicked();

signals:

    void signalShowCalibrationForm();

    void signalShowDrawingForm();

    void signalShowHomographyForm();

    void signalSetMainWindowName(std::string);

private:

    CalibrationData *m_calibrationData;

    Ui::menuForm *ui;

};

#endif // MENUFORM_H
