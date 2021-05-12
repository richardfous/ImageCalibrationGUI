
#pragma once

#include "context.hpp"
#include "drawable.hpp"

#include <memory>
#include <vector>

/// \class Renderer
/// \brief Class to store and render drawables.
///
/// Class Renderer contains instances of class Drawable and is
/// used to render these instances to an output image, which can
/// exported without background (as .png file) or can be placed
/// on a user specified background image. Final image can be
/// obtained by calling getOutputImage method, where the bool
/// variable specifies if the returned image should be with or
/// without background image.
///
/// New instance of class Drawable is stored in m_drawables by
/// calling method addDrawable. All drawables can be rendered by
/// calling method render.
///
/// When an instance of renderer is created a setBackgroundImage
/// method needs to be called to set a background image that
/// will be used to create the final image.

class Renderer final {

    public:

        /// Render all drawables to image.
        void render();

        /// Add drawable into renderer.
        /// \param drawable pointer to drawable instance.
        /// \returns pointer to the last object in list of drawables.
        Drawable* addDrawable(std::unique_ptr<Drawable> drawable);

        /// Remove all drawables from renderer.
        void clearDrawables();

        /// This method returns vector of pointers, that can be
        /// used to get data from each drawable instance and
        /// also enables user to edit each drawble.
        /// \returns vector containing drawables.
        std::vector<std::unique_ptr<Drawable>>& getDrawables();

        /// Removes single drawable from list of drawables.
        /// \param drawable pointer to drawable.
        void removeDrawable(Drawable* drawable);

        /// Returns background image that is for rendering.
        /// \returns matrix containing background image.
        cv::Mat getBackgroundImage() const;

        /// Returns image that contains rendered drawables.
        /// \param includeBackground if set to true, lines will be rendered into inserted image.
        /// \returns matrix containing either background image or objects on alpha background.
        cv::Mat getOutputImage(bool includeBackground = true) const;

        /// Sets the background image, which is used for rendering.
        /// this method has to be called right after instance of
        /// Renderer class is created.
        /// \param image matrix, containing input image.
        void setBackgroundImage(cv::Mat image);

    private:

        std::unique_ptr<Context> m_context;

        cv::Mat m_backgroundImage;

        cv::Mat m_outputImage;

        std::vector<std::unique_ptr<Drawable>> m_drawables;

};
