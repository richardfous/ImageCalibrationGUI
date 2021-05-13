
#pragma once

#include <opencv2/opencv.hpp>

#include <list>
#include <memory>
#include <utility>
#include <vector>

/// \class PointManager
/// \brief Class to store point for Homography calculation.
///
/// Class PointManager is used to store pairs of image and
/// mapping points, that can be passed to class Homography
/// to calculate homography matrix. Each pair is stored inside
/// struct UserPoint. New instance of PointManager can either
/// be created using constructor or methods, that contain
/// predefined mapping points. Scale and offset can also be
/// specified when creating new instance. All predefined methods
/// contain points, that are derived from actual sizes of sport
/// fields and are specified in meters. If homography matrix is
/// calculated with predefined points and without scale the output
/// image can appear small. This can be solved by setting the scale value
/// larger than one. Offset can be used to specify how many pixels
/// will be shown beyond the given boundaries in mapping points.
///
/// New UserPoint can be added by calling method addUserPoint
/// and removed by calling method removeUserPoint. Position of
/// image point can be improved by calling method improvePoints.
///

class PointManager {

    public:

        /// \struct UserPoint is used to store pair of image and mapping points.
        struct UserPoint {

            cv::Point2f imagePoint;

            cv::Point2f mappingPoint;

        };

        /// PointManager constructor
        /// \param mappingPoints contains all mapping points that can be paired with image points.
        /// \param offset defines offset in both x a y axis.
        /// \param scale defines scale of each point in mapping points.
        PointManager(std::vector<cv::Point2f> mappingPoints, cv::Point2f offset = {}, cv::Point2f scale = { 1.0f, 1.0f });

        /// PointManager destructor
        virtual ~PointManager() = default;

        /// This method is used to add pairs of image and mapping points to PointManager instance.
        /// \param imagePoint coordinates of points in real image.
        /// \param mappingPoints coordinates of coresponding image point in 2D view.
        /// \returns pointer to the last pair in list.
        UserPoint* addUserPoint(cv::Point2f imagePoint, cv::Point2f mappingPoint);

        /// Remove UserPoint
        /// \param point pointer to a existing UserPoint in list of user points.
        void removeUserPoint(UserPoint* point);

        /// Remove all user points.
        void clearUserPoints();

        /// Computes pixel density, which can be used to calculate distance in image.
        /// \returns pixel density based on points in user points list.
        float computePixelDensity();

        /// This method is used to obtain image points and mapping points from existing user points.
        /// \param imagePoints vector into which image points will be inserted.
        /// \param mappingPoints vector into which mapping points will be inserted.
        void copyImageMappingPoints(std::vector<cv::Point2f>& imagePoints, std::vector<cv::Point2f>& mappingPoints) const;

        /// Improve image points position based on given input image.
        /// \param image matrix containing image that will be used to improve points.
        /// \param searchWindowSize specifies a size of window, which is used to improve points. Larger number may have negative impact on performance.
        void improvePoints(cv::Mat image, int searchWindowSize = 20);

        /// Get all mapping points that were inserted in constructor of class PointManager.
        /// \returns vector containing mapping points.
        const std::vector<cv::Point2f>& getMappingPoints() const;

        /// \returns offset value.
        const cv::Point2f& getOffset() const;

        /// \returns scale value.
        const cv::Point2f& getScale() const;

        /// \returns list containing all user points.
        const std::list<UserPoint>& getUserPoints() const;

        /// \returns window size.
        const cv::Size& getWindowSize() const;

        /// Create new instance of PointManager with predefined points for badminton field mapping.
        /// \param scale defines scale of each point in mapping points.
        /// \param offset defines offset in both x a y axis.
        /// \returns pointer to a new instance of PointManager.
        static std::unique_ptr<PointManager> createForBadminton(cv::Point2f scale = { 1.0f, 1.0f }, cv::Point2f offset = {});

        /// Create new instance of PointManager with predefined points for basketball field mapping.
        /// \param scale defines scale of each point in mapping points.
        /// \param offset defines offset in both x a y axis.
        /// \returns pointer to a new instance of PointManager.
        static std::unique_ptr<PointManager> createForBasketball( cv::Point2f scale = { 1.0f, 1.0f }, cv::Point2f offset = {});

        /// Create new instance of PointManager with no predefined mapping points, that can be used to store custom points.
        /// \returns pointer to a new instance of PointManager.
        static std::unique_ptr<PointManager> createCustom();

        /// Create new instance of PointManager with predefined points for football field mapping.
        /// \param width specifies width of football field.
        /// \param height specifies height of football field.
        /// \param scale defines scale of each point in mapping points.
        /// \param offset defines offset in both x a y axis.
        /// \returns pointer to a new instance of PointManager.
        static std::unique_ptr<PointManager> createForFootball(float width, float height,  cv::Point2f scale = { 1.0f, 1.0f }, cv::Point2f offset = {});

        /// Create new instance of PointManager with predefined points for hockey field mapping.
        /// \param scale defines scale of each point in mapping points.
        /// \param offset defines offset in both x a y axis.
        /// \returns pointer to a new instance of PointManager.
        static std::unique_ptr<PointManager> createForHockey(cv::Point2f scale = { 1.0f, 1.0f }, cv::Point2f offset = {});

        /// Create new instance of PointManager with predefined points for tennis field mapping.
        /// \param scale defines scale of each point in mapping points.
        /// \param offset defines offset in both x a y axis.
        /// \returns pointer to a new instance of PointManager.
        static std::unique_ptr<PointManager> createForTennis(cv::Point2f scale = { 1.0f, 1.0f }, cv::Point2f offset = {});

        /// Create new instance of PointManager with predefined points for volleyball field mapping.
        /// \param scale defines scale of each point in mapping points.
        /// \param offset defines offset in both x a y axis.
        /// \returns pointer to a new instance of PointManager.
        static std::unique_ptr<PointManager> createForVolleyball(cv::Point2f scale = { 1.0f, 1.0f }, cv::Point2f offset = {});

    protected:

        std::list<UserPoint> m_userPoints;

        std::vector<cv::Point2f> m_mappingPoints;

        cv::Point2f m_offset;

        cv::Point2f m_scale;

        cv::Size m_windowSize;

    private:

        /// Compute the size of mapping window.
        void computeWindowSize();

};
