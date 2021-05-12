
#include "drawables/rectangle.hpp"

#include "context.hpp"
#include "homography.hpp"

Rectangle::Rectangle(std::shared_ptr<Homography> homography, Type type)
    : Drawable { std::move(homography) }
    , m_type { type }
{
}

void Rectangle::draw(Context& context) {

    updatePoints(context.getSize());

    if (m_points.empty())
        return;

    cv::drawContours(context.getImage(), m_points, 0, { m_color[0], m_color[1], m_color[2], 255.0 * m_alpha }, m_thickness, cv::LINE_AA);

}

const cv::Point2f& Rectangle::getFrom() const {

    return m_from;

}

const cv::Point2f& Rectangle::getTo() const {

    return m_to;

}

Rectangle::Type Rectangle::getType() const {

    return m_type;

}

void Rectangle::setFrom(cv::Point2f from) {

    m_from = std::move(from);

}

void Rectangle::setTo(cv::Point2f to) {

    m_to = std::move(to);

}

void Rectangle::setType(Type type) {

    m_type = type;

}

std::unique_ptr<Rectangle> Rectangle::create(std::shared_ptr<Homography> homography, Type type, cv::Point2f from, cv::Point2f to) {

    auto rectangle = std::make_unique<Rectangle>(std::move(homography), type);

    rectangle->setFrom(std::move(from));
    rectangle->setTo(std::move(to));

    return rectangle;;

}

void Rectangle::updatePoints(const cv::Size& size) {

    std::vector<cv::Point2f> points { m_from, m_to };

    cv::perspectiveTransform(points, points, m_homography->getHomographyMatrix());

    cv::Mat temp(size.height, size.width, CV_8UC3, { 0, 0, 0 });

    if(m_type == Type::Square){

       int squareSize;

       if(std::abs(m_from.x - m_to.x) < std::abs(m_from.y - m_to.y)){

           squareSize = std::abs(m_from.x - m_to.x);

       }else{

           squareSize = std::abs(m_from.y - m_to.y);

       }

       if(points[0].x < points[1].x){

           if(points[0].y < points[1].y){

               points[1].x = points[0].x + squareSize;
               points[1].y = points[0].y + squareSize;

           }else {

               points[1].x = points[0].x + squareSize;
               points[1].y = points[0].y - squareSize;

           }

           }else {

               if(points[0].y < points[1].y){

                   points[1].x = points[0].x - squareSize;
                   points[1].y = points[0].y + squareSize;

               }else {

                   points[1].x = points[0].x - squareSize;
                   points[1].y = points[0].y - squareSize;

               }

       }

    }

    cv::rectangle(temp, points[0], points[1], { 0.0, 0.0, 255.0 }, 1, cv::LINE_AA);

    cv::warpPerspective(temp, temp, m_homography->getInverseHomographyMatrix(), size);

    cv::cvtColor(temp, temp, cv::COLOR_BGR2GRAY);

    m_points.clear();

    try {

        cv::findContours(temp, m_points, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    }
    catch (...) {
    }

}
