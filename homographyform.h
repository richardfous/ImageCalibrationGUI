#ifndef HOMOGRAPHYFORM_H
#define HOMOGRAPHYFORM_H

#include <QWidget>
#include <QLabel>
#include <QDebug>
#include <QMouseEvent>
#include <QFileDialog>
#include <utility>
#include <QMessageBox>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
#include <memory>

#include "imagelabel.h"
#include "mappinglabel.h"
#include "calibrationdata.h"

#include "ImageCalibrationLibrary/include/homography.hpp"
#include "ImageCalibrationLibrary/include/pointManager.hpp"
#include "ImageCalibrationLibrary/include/utils.hpp"

namespace Ui {
class homographyForm;
}

class HomographyForm : public QWidget
{
    Q_OBJECT

    ImageLabel* m_imageWidget = nullptr;

    MappingLabel* m_mappingWidget = nullptr;

public:

    explicit HomographyForm(QWidget *parent = nullptr, CalibrationData *calibData = nullptr);

    void disconnectSignals();

    void connectSignals();

    ~HomographyForm();

private slots:

    void on_loadImageButton_clicked();

    void on_mainMenuButton_clicked();

    void on_saveHomographyButton_clicked();

    void on_selectSportBox_activated(int index);

    void storeMappingPoint();

    void storeImagePoint();

    void clearMappingPoint();

    void on_sizeButton_clicked();

    void on_clearPointsButton_clicked();

    void on_calculateHomographyButton_clicked();

    void on_improvePointsButton_clicked();

    void on_showBirdsEyeBox_stateChanged(int arg1);

    void on_undistortButton_clicked();

signals:

    void signalShowMenuForm();

private:

    CalibrationData *m_calibrationData;

    Ui::homographyForm *ui;

    cv::Mat m_homographyMatrix;

    std::unique_ptr<PointManager> m_pointManager;

    std::shared_ptr<Homography> m_homography;

    cv::Point2f m_mappingPoint = {-1,-1};

    cv::Point2f m_imagePoint = {-1,-1};

    std::vector<cv::Point2f> m_imagePoints;

    std::vector<cv::Point2f> m_mappingPoints;

    std::vector<cv::Point2f> m_customPoints;

    float m_pointsScaleFactor = 0.0f;

protected:

    void fillComboBox();

    void hideSizeForm();

    void mousePressEvent(QMouseEvent *event);

    bool checkPointsAxis(std::vector<cv::Point2f> inputPoints);

};

#endif // HOMOGRAPHYFORM_H
