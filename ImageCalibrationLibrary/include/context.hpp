
#pragma once

#include <opencv2/opencv.hpp>

class Drawable;

/// \class Context
/// \brief Context is used for displaying drawables.
///
/// Istance of this class is created when instance of
/// Renderer is created. Every drawable that is created
/// is drawn on context, which is transparent. Context
/// containing drawables is then merged with background
/// image in class Renderer.
///

class Context {

    public:

        /// Context constructor.
        /// \param size of new context.
        explicit Context(cv::Size size);

        ///Default destructor.
        virtual ~Context() = default;

        /// Method used for erasing all content in context.
        void clear();

        /// This method is used for drawing drawables.
        /// \param drawable instance of drawable.
        void draw(Drawable& drawable);

        /// \returns context.
        cv::Mat getImage() const;

        /// \returns size of context.
        const cv::Size& getSize() const;

    private:

        cv::Size m_size;

        cv::Mat m_image;

};
