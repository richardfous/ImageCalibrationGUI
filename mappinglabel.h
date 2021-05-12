#ifndef MAPPINGLABEL_H
#define MAPPINGLABEL_H

#include <QWidget>
#include <QMouseEvent>
#include <QLabel>
#include <QDebug>
#include <QString>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>


namespace Ui {
class MappingLabel;
}

class MappingLabel : public QLabel
{
    Q_OBJECT

public:

    explicit MappingLabel(QWidget *parent = nullptr);

    ~MappingLabel();

    void setWidth(float width);

    void setHeight(float height);

    void setIsFootball(bool isFootball);

    void setEnabledClick(bool status);

    void setBackgroundImage(cv::Mat image);

    void setIsCustom(bool isCustom);

    void setPointScaleFactor(float scaleFactor);

    bool getEnabledClick();

    float getWidth();

    float getHeight();

    bool getIsFootball();

    bool getIsCustom();

    float getPointScaleFactor();

    cv::Point2f getMappingPoint();

    void displayBackgroundImage(cv::Mat inputImage);

    void resetMappingPoint();

    void setInputMappingPoints(std::vector<cv::Point2f> input);

    void loadFromQrc(QString qrc, bool firstLoad = false, int flag = cv::IMREAD_COLOR );

    void displayPoints(std::vector<cv::Point2f> pairedMappingPoints = std::vector<cv::Point2f>(), bool isSet = false);

protected:

    void mousePressEvent(QMouseEvent*e);

    void storeFootballPoint();

    cv::Point2f findCorrectPoint(cv::Point2f point, std::vector<cv::Point2f> vector);

signals:

    void signalActivateImageClick();

    void signalMappingPoint();

    void signalClearMappingPont();

private:

    Ui::MappingLabel *ui;

    cv::Mat m_backgroundImage;

    QPixmap m_newPixmap;

    float m_aspectRationImage;

    float m_aspectRationLabel;

    float m_scaleFactor;

    float m_width = 0.0f;

    float m_height = 0.0f;

    cv::Point2f m_mappingPoint;

    bool m_enabledClick = true;

    bool m_isFootball = false;

    bool m_isCustom = false;

    std::vector<cv::Point2f> m_footballPoints;

    std::vector<cv::Point2f> m_inputMappingPoints;

    float m_pointScaleFactor = 0.0f;

};

#endif // MAPPINGLABEL_H
