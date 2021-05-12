
#pragma once

#include "drawable.hpp"
#include "homography.hpp"

#include <memory>

/// \class Line
/// \brief Class Line can be used for drawing horizontal and vertical lines.
///
/// Class Rectangle can be used for drawing horizontal
/// and vertical lines. New instance can be created by using
/// constructor or by using static method create,
/// which returns pointer to this instance. This class
/// inherits atributes from class Drawable, such as color
/// thickness and transparency. Only one point is needed to
/// be able to draw this type of object. This point can be
/// set in create method or by using set methods. Two new
/// points are created and those points are then enhanced
/// based on type selection to create desired line.If
/// instance is created by using constructor, method draw
/// needs to be called, in order to render the object.
///
/// After the object is created, it can be rendered into image
/// by calling method addDrawble from the class Renderer.
///

class Line : public Drawable {

    public:
        /// Type is used to specify if object is horizontal or vertical line.
        enum class Type {

            Horizontal, Vertical

        };

        /// Line constructor.
        /// \param homography instance of class Homography with homography matrix inserted.
        /// \param type type of drawn object.
        /// \param windowSize spcifies the size of mapping window. Can be obtained from PointManager.
        Line(std::shared_ptr<Homography> homography, Type type, cv::Size windowSize);

        /// Method for drawing object.
        /// \param context instance of Context class.
        virtual void draw(Context& context) override;

        /// \returns line offset.
        virtual float getOffset() const;

        /// \returns point used to draw object.
        virtual const cv::Point2f& getPoint() const;

        /// \returns object type.
        virtual Type getType() const;

        /// Method for setting line offset.
        /// \param offset line offset.
        virtual void setOffset(float offset);

        /// Method for setting point used to draw object.
        /// \param point object point.
        virtual void setPoint(cv::Point2f point);

        /// Method for setting object type.
        /// \param type object type.
        virtual void setType(Type type);

        /// Create new instance of Line class.
        /// \param homography instance of class Homography with homography matrix inserted.
        /// \param type type of drawn object.
        /// \param from object point.
        /// \param windowSize spcifies the size of mapping window. Can be obtained from PointManager.
        /// \param offset specify line offset.
        /// \returns pointer to the newly created instance.
        static std::unique_ptr<Line> create(std::shared_ptr<Homography> homography, Type type, cv::Point2f point,cv::Size windowSize, float offset = 0.0f);

    protected:

        /// Method for calculating new object points.
        /// \param size of mapping window.
        virtual void updatePoints(const cv::Size& size);

        /// Method for calculating line intersection to keep new points within image.
        /// \param firstLine specifies x a y coordinates of two points on a line.
        /// \param secondLine specifies x a y coordinates of two points on a line.
        /// \returns point of intersection of inserted lines.
        cv::Point2f calculateLinesIntersection(cv::Vec4i firstLine, cv::Vec4i secondLine);

        std::vector<cv::Point2f> m_points;

        cv::Point2f m_point;

        Type m_type;

        float m_offset = 0.0f;

        cv::Size m_windowSize;

        cv::Size m_contextSize;



};
