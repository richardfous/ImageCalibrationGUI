
#include "drawable.hpp"

#include <algorithm>

Drawable::Drawable(std::shared_ptr<Homography> homography)
    : m_homography { std::move(homography) }
{
}

float Drawable::getAlpha() const {

    return m_alpha;

}

cv::Scalar Drawable::getColor() const {

    return m_color;

}

int Drawable::getThickness() const {

    return m_thickness;

}

void Drawable::setAlpha(float alpha) {

    m_alpha = std::min(std::max(alpha, 0.0f), 1.0f);

}

void Drawable::setColor(cv::Scalar color) {

    m_color = color;

}

void Drawable::setThickness(int thickness) {

    m_thickness = thickness;

}
