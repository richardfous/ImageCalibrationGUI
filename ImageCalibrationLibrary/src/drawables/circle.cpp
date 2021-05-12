
#include "drawables/circle.hpp"

#include "context.hpp"
#include "homography.hpp"

Circle::Circle(std::shared_ptr<Homography> homography)
    : Drawable { std::move(homography) }
{
}

void Circle::draw(Context& context) {

    updatePoints(context.getSize());

    if (m_points.empty())
        return;

    cv::drawContours(context.getImage(), m_points, 0, { m_color[0], m_color[1], m_color[2], 255.0 * m_alpha }, m_thickness, cv::LINE_AA);

}

const cv::Point2f& Circle::getPoint() const {

    return m_point;

}

int Circle::getRadius() const {

    return m_radius;

}

void Circle::setPoint(cv::Point2f point) {

    m_point = std::move(point);

}

void Circle::setRadius(int radius) {

    m_radius = radius;

}

std::unique_ptr<Circle> Circle::create(std::shared_ptr<Homography> homography, cv::Point2f point, int radius) {

    auto circle = std::make_unique<Circle>(std::move(homography));

    circle->setPoint(std::move(point));
    circle->setRadius(radius);

    return circle;

}

void Circle::updatePoints(const cv::Size& size) {

    std::vector<cv::Point2f> points { m_point };

    cv::perspectiveTransform(points, points, m_homography->getHomographyMatrix());

    cv::Mat temp(size.height, size.width, CV_8UC3, { 0, 0, 0 });

    cv::circle(temp, points[0], m_radius, { 0.0, 0.0, 255.0 }, 1, cv::LINE_AA);

    cv::warpPerspective(temp, temp, m_homography->getInverseHomographyMatrix(), size);

    cv::cvtColor(temp, temp, cv::COLOR_BGR2GRAY);

    cv::GaussianBlur(temp, temp, { 5, 5 }, 0.0);

    m_points.clear();

    try {

        cv::findContours(temp, m_points, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    }
    catch (...) {
    }

}
