#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "menuform.h"
#include "calibrationform.h"
#include "homographyform.h"
#include "drawingform.h"
#include "calibrationdata.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    MenuForm *menuform = nullptr;

    CalibrationForm *calibForm = nullptr;

    HomographyForm *homogForm = nullptr;

    DrawingForm *drawForm = nullptr;

    CalibrationData *calibData = nullptr;

    void resizeEvent(QResizeEvent *event);

public:

    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private slots:

    void showCalibrationForm();

    void showHomographyForm();

    void showDrawingForm();

    void showMenuForm();

    void setMainWindowName(std::string projectName);


private:

    Ui::MainWindow *ui;


};
#endif // MAINWINDOW_H
