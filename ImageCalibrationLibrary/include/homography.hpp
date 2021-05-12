
#pragma once

#include <string>
#include <vector>

#include <opencv2/opencv.hpp>

/// \class Homography
/// \brief Class used for calculating and storing homography matrix.
///
/// Class Homography is used for storing and calculating
/// homography. Homography matrix is calculated from image
/// and mapping points, that are obtained from PointManager
/// class. Instance of homography class is used to draw
/// objects which can be found in drawables. Once homography
/// matrix is calculated, it can be reused for the same camera
/// position, if all parametrs like zoom etc. remain the same.
/// Instance of Homography can also be created by using default
/// constructor and then homography matrix can be inserted
/// using setHomographyMatrix method. Once homography matrix
/// is set, it can be retrieved by calling getHomography
/// matrix. Inverse homography matrix can be rerieved by
/// calling method getInverseHomographyMatrix.
///

class PointManager;

class Homography final {

    public:

        /// Default constructor.
        Homography();

        /// Homography constructor from instance of PointManager class. This constructor automatically computes homography matrix.
        /// \param pointManager PointManager instance.
        Homography(const PointManager& pointManager);

        /// This method computes homography matrix from instance of PointManager.
        /// \param pointManager PointManager instance.
        void computeHomographyMatrix(const PointManager& pointManager);

        /// \returns homography matrix.
        cv::Mat getHomographyMatrix() const;

        /// \returns inverse homography matrix.
        cv::Mat getInverseHomographyMatrix() const;

        /// Set existing homography matrix.
        /// \param matrix homography matrix.
        void setHomographyMatrix(cv::Mat matrix);

    private:

        cv::Mat m_homographyMatrix;

        cv::Mat m_inverseHomographyMatrix;

};
