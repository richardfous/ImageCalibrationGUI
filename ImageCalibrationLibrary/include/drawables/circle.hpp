
#pragma once

#include "drawable.hpp"

#include <memory>

/// \class Circle
/// \brief Class Circle can be used for inserting circle into image.
///
/// Class Circle can be used for inserting circle into
/// image. New instance can be created by using
/// constructor or by using static method create,
/// which returns pointer to this instance. This class
/// inherits atributes from class Drawable, such as
/// color, thickness and transparency. A single point
/// is needed to be able to insert circle into image.
/// To specity the size of the inserted circle, attribute
/// radius needs to be set. If instance is created by
/// using constructor, method draw needs to be called,
/// in order to render the object.
///
/// After the object is created, it can be rendered into image
/// by calling method addDrawble from the class Renderer.
///

class Circle : public Drawable {

    public:

        /// Circle constructor.
        /// \param homography instance of class Homography with homography matrix inserted.
        explicit Circle(std::shared_ptr<Homography> homography);

        /// Method for drawing object.
        /// \param context instance of Context class.
        virtual void draw(Context& context) override;

        /// \returns point used to draw object.
        virtual const cv::Point2f& getPoint() const;

        /// \returns radius of inserted circle.
        virtual int getRadius() const;

        /// Method for setting circle point.
        /// \param point center point of circle.
        virtual void setPoint(cv::Point2f point);

        /// Method for setting circle radius.
        /// \param radius radius of circle.
        virtual void setRadius(int radius);

        /// Create new instance of Circle class.
        /// \param homography instance of class Homography with homography matrix inserted.
        /// \param point center of circle.
        /// \param radius size of circle.
        /// \returns pointer to the newly created instance.
        static std::unique_ptr<Circle> create(std::shared_ptr<Homography> homography, cv::Point2f point, int radius);

    protected:

        /// Method for calculating new object points.
        /// \param size of context.
        virtual void updatePoints(const cv::Size& size);

        std::vector<std::vector<cv::Point>> m_points;

        cv::Point2f m_point;

        int m_radius = 1;

};
