
#include "homography.hpp"

#include "pointManager.hpp"

Homography::Homography()
    : m_homographyMatrix { cv::Mat::eye({ 3, 3 }, CV_32F) }
    , m_inverseHomographyMatrix { m_homographyMatrix.inv() }
{
}

Homography::Homography(const PointManager& pointManager)
{
    computeHomographyMatrix(pointManager);
}

void Homography::computeHomographyMatrix(const PointManager& pointManager) {

    std::vector<cv::Point2f> imagePoints;
    std::vector<cv::Point2f> mappingPoints;

    pointManager.copyImageMappingPoints(imagePoints, mappingPoints);

    setHomographyMatrix(cv::findHomography(imagePoints, mappingPoints));

}

cv::Mat Homography::getHomographyMatrix() const {

    return m_homographyMatrix;

}

cv::Mat Homography::getInverseHomographyMatrix() const {

    return m_inverseHomographyMatrix;

}

void Homography::setHomographyMatrix(cv::Mat matrix) {

    if(matrix.empty()){

        m_homographyMatrix = cv::Mat::eye({ 3, 3 }, CV_32F);

    }else{
        m_homographyMatrix = matrix.clone();
    }
    m_inverseHomographyMatrix = m_homographyMatrix.inv();

}
