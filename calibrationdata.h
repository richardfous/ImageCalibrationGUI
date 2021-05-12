#ifndef CALIBRATIONDATA_H
#define CALIBRATIONDATA_H

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

class CalibrationData
{
public:

    CalibrationData();

    void setHomographyMatrix(cv::Mat homographyMatrix);

    void setDistoritonCoefficient(double coefficient);

    void setScaleCoefficient(double coefficient);

    void setProjectName(std::string projectName);

    void setProjectPath(std::string projectPath);

    void setPixelDensity(float pixelDensity);

    void setWindowSize(cv::Size windowSize);

    cv::Mat getHomographyMatrix();

    double getDistoritonCoefficient();

    double getScaleCoefficient();

    std::string getProjectName();

    std::string getProjectPath();

    float getPixelDensity();

    cv::Size getWindowSize();

    void saveCalibrationData();

    void loadCalibrationData(std::string path);

private:

     cv::Mat m_homography;

     double m_distoritonCoefficient = 0.0;

     double m_scaleCoefficient = 1.0;

     std::string m_projectName;

     std::string m_projectPath;

     float m_pixelDensity = 0.0f;

     cv::Size m_windowSize;

};


#endif // CALIBRATIONDATA_H
