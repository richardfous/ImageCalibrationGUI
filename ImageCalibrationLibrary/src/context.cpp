
#include "context.hpp"

#include "drawable.hpp"

Context::Context(cv::Size size)
    : m_size { std::move(size) }
    , m_image { m_size.height, m_size.width, CV_8UC4, { 255, 255, 255, 0 } }
{
}

void Context::clear() {

    m_image = cv::Mat(m_image.rows, m_image.cols, m_image.type(), {255,255,255,0});

}

void Context::draw(Drawable& drawable) {

    drawable.draw(*this);

}

cv::Mat Context::getImage() const {

    return m_image;

}

const cv::Size& Context::getSize() const {

    return m_size;

}
