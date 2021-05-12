
#pragma once

#include <opencv2/core/core.hpp>

#include <memory>

class Context;

/// \class Drawable
/// \brief Base class for drawables.
///
/// Class Drawable is a base class for every type of
/// drawable specified in drawables. This class
/// contains attributes and methods, which are the same
/// for each drawable. There attributes are color, line
/// thickness, object transparency and homography matrix.
/// Every attribute has get and set method.
///

class Homography;

class Drawable {

    public:

        /// Drawable construtor.
        /// \param homography instance of homography.
        explicit Drawable(std::shared_ptr<Homography> homography);

        /// Drawable destrutor.
        virtual ~Drawable() = default;

        /// Draw object on context.
        /// \param context instance of context.
        virtual void draw(Context& context) = 0;

        /// \return transparency value.
        virtual float getAlpha() const;

        /// \returns object color.
        virtual cv::Scalar getColor() const;

        /// \returns thickness value.
        virtual int getThickness() const;

        /// Set object transparency.
        /// \param alpha transparency value.
        virtual void setAlpha(float alpha);

        /// Set object color.
        /// \param alpha color.
        virtual void setColor(cv::Scalar color);

        /// Set object thickness.
        /// \param alpha thickness value.
        virtual void setThickness(int thickness);

    protected:

        std::shared_ptr<Homography> m_homography;

        cv::Scalar m_color = { 0, 0, 0 };

        int m_thickness = 1;

        float m_alpha = 1.0f;

};
