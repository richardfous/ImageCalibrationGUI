#ifndef DRAWINGFORM_H
#define DRAWINGFORM_H

#include <QWidget>
#include <QFileDialog>
#include <QColor>
#include <QColorDialog>
#include <QTableWidget>
#include <QHeaderView>
#include <QBrush>
#include <QSignalMapper>
#include <QInputDialog>
#include <QMessageBox>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include "imagelabel.h"
#include "calibrationdata.h"

#include "ImageCalibrationLibrary/include/utils.hpp"
#include "ImageCalibrationLibrary/include/renderer.hpp"
#include "ImageCalibrationLibrary/include/drawable.hpp"

#include "ImageCalibrationLibrary/include/drawables/circle.hpp"
#include "ImageCalibrationLibrary/include/drawables/line.hpp"
#include "ImageCalibrationLibrary/include/drawables/image.hpp"
#include "ImageCalibrationLibrary/include/drawables/rectangle.hpp"

namespace Ui {
class drawingForm;
}

class DrawingForm : public QWidget
{
    Q_OBJECT

public:

    explicit DrawingForm(QWidget *parent = nullptr, CalibrationData *calibData = nullptr);

    void disconnectSignals();

    void connectSignals();

    ~DrawingForm();

private slots:

    void on_loadImageButton_clicked();

    void on_menuButton_clicked();

    void storeImagePoint();

    void clearImagePoint();

    void on_correctDistortionButton_clicked();

    void on_objectSelectorComboBox_activated(int index);

    void on_setColorButton_clicked();

    void on_renderButton_clicked();

    void on_insertImageButton_clicked();

    void on_clearButton_clicked();

    void on_saveImageButton_clicked();

    void on_saveAlphaButton_clicked();

    void on_measureDistanceButton_clicked();

    void cellDoubleClicked(int row,int column);

signals:

    void signalShowMenuForm();

protected:

    void fillComboBox();

    void displayColor(cv::Scalar color, float m_alpha);

    void insertTableRow(QString type, cv::Scalar color, float alpha, int thickness);

private:

    CalibrationData *m_calibrationData;

    Ui::drawingForm *ui;

    ImageLabel* m_imageWidget = nullptr;

    QTableWidget* m_historyTable = nullptr;

    std::shared_ptr<Homography> m_homography;

    cv::Mat m_inputImage;

    cv::Mat m_imageToInsert;

    std::vector<cv::Point2f> m_imagePoints;

    Renderer m_renderer;

    cv::Scalar m_color = {0,0,255};

    float m_alpha = 1.0f;

};

#endif // DRAWINGFORM_H
