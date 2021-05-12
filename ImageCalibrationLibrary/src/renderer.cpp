
#include "renderer.hpp"

#include "utils.hpp"

#include <opencv2/opencv.hpp>

#include <algorithm>

void Renderer::render() {

	// Render the background image.
    if(m_backgroundImage.empty()){

        m_backgroundImage = m_context->getImage();

    }else{

     m_outputImage = m_backgroundImage.clone();

    }

	if (!m_context)
		return;

	// Render the drawables.
	m_context->clear();

	for (std::unique_ptr<Drawable>& drawable : m_drawables) {
		drawable->draw(*m_context);
	}

	// Overlay the output image with the resulting context.
	blendImages(m_outputImage, m_context->getImage());

}

Drawable* Renderer::addDrawable(std::unique_ptr<Drawable> drawable) {

	m_drawables.emplace_back(std::move(drawable));

	return m_drawables.back().get();

}

void Renderer::clearDrawables() {

	m_drawables.clear();

}

std::vector<std::unique_ptr<Drawable>>& Renderer::getDrawables() {

    return m_drawables;

}

void Renderer::removeDrawable(Drawable* drawable) {

	auto iterator = std::find_if(m_drawables.begin(), m_drawables.end(), [drawable](const std::unique_ptr<Drawable>& object) {

		return object.get() == drawable;

	});

	if (iterator != m_drawables.end()) {
		m_drawables.erase(iterator);
	}

}

cv::Mat Renderer::getBackgroundImage() const {

	return m_backgroundImage;

}

cv::Mat Renderer::getOutputImage(bool includeBackground) const {

	return includeBackground ? m_outputImage : m_context ? m_context->getImage() : cv::Mat(m_outputImage.rows, m_outputImage.cols, CV_8UC4, { 0, 0, 0, 255 });

}

void Renderer::setBackgroundImage(cv::Mat image) {

    if(image.empty()){
        return;
    }

	m_backgroundImage.zeros(image.rows, image.cols, CV_8UC4);
    m_outputImage.zeros(image.rows, image.cols, CV_8UC4);

	cv::cvtColor(image, m_backgroundImage, cv::COLOR_BGR2BGRA);

	m_context = std::make_unique<Context>(cv::Size(image.cols, image.rows));

}
