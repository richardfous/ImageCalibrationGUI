
#pragma once

#include "drawable.hpp"

/// \class Rectangle
/// \brief Class Rectangle can be used for drawing squares and rectangles.
///
/// Class Rectangle can be used for drawing squares and
/// rectangles. New instance can be created by using
/// constructor or by using static method create,
/// which returns pointer to this instance. This class
/// inherits atributes from class Drawable, such as color
/// thickness and transparency. Two points are needed to draw
/// square or rectangle. Those points can set in create method
/// or by using set methods. If type rectangle is selected,
/// the rectangle is drawn inbetween those two points. If type
/// Square is selected a x coordinate of the second point is
/// used to calculate size of the square side. If instance is
/// created by using constructor, method draw needs to be
/// called, in order to render the object.
///
/// After the object is created, it can be rendered into image
/// by calling method addDrawble from the class Renderer.
///


class Rectangle : public Drawable {

    public:
        /// Type is used to specify if object is rectangle or square.
        enum class Type {

            Square, Rectangle

        };

        /// Rectangle constructor.
        /// \param homography instance of class Homography with homography matrix inserted.
        /// \param type type of drawn object.
        explicit Rectangle(std::shared_ptr<Homography> homography, Type type);

        /// Method for drawing object.
        /// \param context instance of Context class.
        virtual void draw(Context& context) override;

        /// \returns first point used to draw object.
        virtual const cv::Point2f& getFrom() const;

        /// \returns second point used to draw object.
        virtual const cv::Point2f& getTo() const;

        /// \returns object type.
        virtual Type getType() const;

        /// Method for setting first point.
        /// \param from first point.
        virtual void setFrom(cv::Point2f from);

        /// Method for setting second point.
        /// \param to second point.
        virtual void setTo(cv::Point2f to);

        /// Method for setting object type.
        /// \param type object type.
        virtual void setType(Type type);

        /// Create new instance of Rectangle class.
        /// \param homography instance of class Homography with homography matrix inserted.
        /// \param type type of drawn object.
        /// \param from first object point.
        /// \param to second object point.
        /// \returns pointer to the newly created instance.
        static std::unique_ptr<Rectangle> create(std::shared_ptr<Homography> homography, Type type, cv::Point2f from, cv::Point2f to);

    protected:

        /// Method for calculating new object points.
        /// \param size of context.
        virtual void updatePoints(const cv::Size& size);

        std::vector<std::vector<cv::Point>> m_points;

        Type m_type;

        cv::Point2f m_from;

        cv::Point2f m_to;

};
