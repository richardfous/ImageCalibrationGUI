#ifndef IMAGELABEL_H
#define IMAGELABEL_H

#include <QWidget>
#include <QLabel>
#include <QDebug>
#include <QMouseEvent>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

namespace Ui {
class imageLabel;
}

class ImageLabel : public QLabel
{
    Q_OBJECT

public:

    explicit ImageLabel(QWidget *parent = nullptr);

    ~ImageLabel();

    void resetImagePoint();

    cv::Point2f getImagePoint();

    void loadBackgroundImage(std::string path);

    void setBackgroundImage(cv::Mat inputImage);

    void changeDefaultBackgroundImage(cv::Mat inputImage);

    void deactivateImageClick();

    cv::Mat getBackgroundImage();

    void displayPoints(std::vector<cv::Point2f> pairedImagePoints = std::vector<cv::Point2f>());

public slots:

    void activateImageClick();

signals:

    void signalImagePoint();

    void signalClearImagePoint();

protected:

    void mousePressEvent(QMouseEvent*e);

private:

    Ui::imageLabel *ui;

    cv::Mat m_backgroundImage;

    QPixmap m_newPixmap;

    double m_aspectRationImage = 0;

    double m_aspectRationLabel = 0;

    double m_scaleFactor = 0;

    bool m_enabledClick = false;

    cv::Point2f m_imagePoint;

};

#endif // IMAGELABEL_H
