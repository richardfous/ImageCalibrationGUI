
#pragma once

#include "drawable.hpp"

#include "utils.hpp"

/// \class Image
/// \brief Class Line can be used for inserting image into another image.
///
/// Class Rectangle can be used for inserting image
/// into another image. New instance can be created by using
/// constructor or by using static method create,
/// which returns pointer to this instance. This class
/// inherits atributes from class Drawable, such as transparency.
/// Two points are needed to be able to insert image
/// into another image. Those points can set in create method
/// or by using set methods. Image, that will be inserted
/// can also be rotated, by selecting appropriate angle
/// in enum Rotation. Default image rotation is based on 
/// position of first and second point. First point corresponds
/// to the top left corner of the image and second point 
/// corresponds to the bottom right corner of the imge.
/// Final image is then scaled and placed inbetween two 
/// input points. The image can be with or without alpha 
/// channel. If the input image has alpha channel, olny the 
/// non-transparent pixels will be drawn.If instance is 
/// created by using constructor, method draw needs to be 
/// called, in order to render the object.
///
/// After the object is created, it can be rendered into image
/// by calling method addDrawble from the class Renderer.
///

class Image : public Drawable {

    public:
        /// Rotation is used to specify input image rotation.
        enum class Rotation {

            _0, _90, _180, _270

        };

        /// Image constructor.
        /// \param homography instance of class Homography with homography matrix inserted.
        explicit Image(std::shared_ptr<Homography> homography);

        /// Method for drawing object.
        /// \param context instance of Context class.
        virtual void draw(Context& context) override;

        /// \returns first point used to draw object.
        virtual const cv::Point2f& getFrom() const;

        /// \returns image that will be placed onto background.
        virtual cv::Mat getImage() const;

        /// \returns input image rotoation.
        virtual Rotation getRotation() const;

        /// \returns second point used to draw object.
        virtual const cv::Point2f& getTo() const;

        /// Method for setting first point.
        /// \param from first point.
        virtual void setFrom(cv::Point2f from);

        /// Method for setting input image.
        /// \param image matrix containing input image.
        virtual void setImage(cv::Mat image);

        /// Method for setting the input image rotation.
        /// \param rotation type of rotation.
        virtual void setRotation(Rotation rotation);

        /// Method for setting second point.
        /// \param to second point.
        virtual void setTo(cv::Point2f to);

        /// Create new instance of Image class.
        /// \param homography instance of class Homography with homography matrix inserted.
        /// \param from first object point.
        /// \param to second object point.
        /// \param sourceImage image to be inserted.
        /// \param rotation type of rotation.
        /// \returns pointer to the newly created instance.
        static std::unique_ptr<Image> create(std::shared_ptr<Homography> homography, cv::Point2f from, cv::Point2f to, cv::Mat sourceImage, Rotation rotation = Rotation::_0);

    protected:

        std::vector<std::vector<cv::Point>> m_points;

        cv::Mat m_image;

        cv::Point2f m_from;

        cv::Point2f m_to;

        Rotation m_rotation = Rotation::_0;

};
