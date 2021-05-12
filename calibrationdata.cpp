#include "calibrationdata.h"

#include <iostream>

CalibrationData::CalibrationData()
{

    m_distoritonCoefficient = 0;
    m_scaleCoefficient = 0;

}


void CalibrationData::setHomographyMatrix(cv::Mat homographyMatrix){

    if(!homographyMatrix.empty()){
        m_homography = homographyMatrix;
    }

}

void CalibrationData::setDistoritonCoefficient(double coefficient){

    m_distoritonCoefficient = coefficient;

}

void CalibrationData::setScaleCoefficient(double coefficient){

    m_scaleCoefficient = coefficient;

}

void CalibrationData::setProjectName(std::string projectName){

    m_projectName = projectName;

}

void CalibrationData::setProjectPath(std::string projectPath){

    m_projectPath = projectPath;

}

void CalibrationData::setPixelDensity(float pixelDensity){

    m_pixelDensity = pixelDensity;

}

void CalibrationData::setWindowSize(cv::Size windowSize){

    m_windowSize = windowSize;

}

cv::Mat CalibrationData::getHomographyMatrix(){

        return m_homography;

}

double CalibrationData::getDistoritonCoefficient(){

    return m_distoritonCoefficient;

}

double CalibrationData::getScaleCoefficient(){

    return m_scaleCoefficient;

}

std::string CalibrationData::getProjectName(){

    return m_projectName;

}

std::string CalibrationData::getProjectPath(){

    return m_projectPath;

}

float CalibrationData::getPixelDensity(){

    return m_pixelDensity;

}

cv::Size CalibrationData::getWindowSize(){

    return m_windowSize;

}

void CalibrationData::saveCalibrationData(){

    if(m_projectPath.empty()){
        return;
    }
    cv::FileStorage fs(m_projectPath, cv::FileStorage::WRITE);
    fs << "ProjectName" << m_projectName;
    fs << "Homography" << m_homography;
    fs << "DistoritonCoefficient" << m_distoritonCoefficient;
    fs << "ScaleCoefficient" << m_scaleCoefficient;
    fs << "PixelDensity" << m_pixelDensity;
    fs << "WindowSize" << m_windowSize;
    fs.release();

}

void CalibrationData::loadCalibrationData(std::string path){

    cv::FileStorage fs(path, cv::FileStorage::READ);
    fs["ProjectName"] >> m_projectName;
    fs["Homography"] >> m_homography;
    fs["DistoritonCoefficient"] >> m_distoritonCoefficient;
    fs["ScaleCoefficient"] >> m_scaleCoefficient;
    fs["PixelDensity"] >> m_pixelDensity;
    fs["WindowSize"] >> m_windowSize;
    fs.release();

}

