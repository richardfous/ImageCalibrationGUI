
#include "drawables/line.hpp"

#include "context.hpp"
#include "homography.hpp"

#include <opencv2/opencv.hpp>

#include <cmath>

Line::Line(std::shared_ptr<Homography> homography, Type type, cv::Size windowSize)
    : Drawable { std::move(homography) }
    , m_type { type }
    , m_windowSize { windowSize }
{
}

void Line::draw(Context& context) {

    m_contextSize = context.getSize();
    updatePoints(m_windowSize);

    if (m_points.size() < 2)
        return;

    cv::line(context.getImage(), m_points[0], m_points[1], { m_color[0], m_color[1], m_color[2], 255.0 * m_alpha }, m_thickness, cv::LINE_AA);

}

float Line::getOffset() const {

    return m_offset;

}

const cv::Point2f& Line::getPoint() const {

    return m_point;

}

Line::Type Line::getType() const {

    return m_type;

}

void Line::setOffset(float offset) {

    m_offset = offset;

}

void Line::setPoint(cv::Point2f point) {

    m_point = std::move(point);

}

void Line::setType(Type type) {

    m_type = type;

}

std::unique_ptr<Line> Line::create(std::shared_ptr<Homography> homography, Type type, cv::Point2f point, cv::Size windowSize, float offset) {

    auto line = std::make_unique<Line>(std::move(homography), type, windowSize);

    line->setOffset(offset);
    line->setPoint(std::move(point));

    return line;

}

void Line::updatePoints(const cv::Size& size) {

    std::vector<cv::Point2f> points { m_point, m_point };

    cv::perspectiveTransform(points, points, m_homography->getHomographyMatrix());

    switch (m_type) {

        case Type::Horizontal:

            points[0].x = m_offset;
            points[1].x = static_cast<float>(size.width) - m_offset;
            break;

        case Type::Vertical:

            points[0].y = m_offset;
            points[1].y = static_cast<float>(size.height) - m_offset;
            break;

        default:

            break;

    }

    cv::perspectiveTransform(points, points, m_homography->getInverseHomographyMatrix());

    if(m_type == Type::Horizontal){

        if(std::abs(std::round(points[0].x)) > m_contextSize.width && std::abs(std::round(points[0].y)) > m_contextSize.height){

            cv::Point2f fixedPoint = calculateLinesIntersection({0,0,0,m_contextSize.height},{static_cast<int>(m_point.x),static_cast<int>(m_point.y),static_cast<int>(points[1].x),static_cast<int>(points[1].y)});
            points[0].x = fixedPoint.x;
            points[0].y = fixedPoint.y;

        }else if(std::abs(std::round(points[1].x)) > m_contextSize.width && std::abs(std::round(points[1].y)) > m_contextSize.height){

            cv::Point2f fixedPoint = calculateLinesIntersection({m_contextSize.width,0,m_contextSize.width,m_contextSize.height},{static_cast<int>(points[0].x),static_cast<int>(points[0].y),static_cast<int>(m_point.x),static_cast<int>(m_point.y)});
            points[1].x = fixedPoint.x;
            points[1].y = fixedPoint.y;

        }
    }

    m_points = { { std::round(points[0].x), std::round(points[0].y) }
               , { std::round(points[1].x), std::round(points[1].y) } };

}

cv::Point2f Line::calculateLinesIntersection(cv::Vec4i firstLine, cv::Vec4i secondLine){

    float a1,b1,c1,a2,b2,c2,pA,pB,pC,xIntercept,yIntercept;

    a1 = (float)firstLine[1] - (float)firstLine[3];
    b1 = (float)firstLine[2] - (float)firstLine[0];
    c1 = (float)firstLine[0]*firstLine[3] - (float)firstLine[2]*firstLine[1];

    a2 = (float)secondLine[1] - (float)secondLine[3];
    b2 = (float)secondLine[2] - (float)secondLine[0];
    c2 = (float)secondLine[0]*secondLine[3] - (float)secondLine[2]*secondLine[1];

    pA = (float)b1*c2 - (float)b2*c1;
    pB = (float)a2*c1 - (float)a1*c2;
    pC = (float)a1*b2 - (float)a2*b1;

    if(pC != 0){

        xIntercept = (float)pA/pC;
        yIntercept = (float)pB/pC;

    }else{

        return {0,0};

    }

    return {xIntercept,yIntercept};

}
