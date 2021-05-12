
#include "pointManager.hpp"

#include <algorithm>

PointManager::PointManager(std::vector<cv::Point2f> mappingPoints, cv::Point2f scale, cv::Point2f offset)
    : m_offset { std::move(offset) }
    , m_scale { std::move(scale) }
{

    for (cv::Point2f& point : mappingPoints) {
        point.x = point.x * scale.x + offset.x;
        point.y = point.y * scale.y + offset.y;
    }

    m_mappingPoints = std::move(mappingPoints);

    computeWindowSize();

}

PointManager::UserPoint* PointManager::addUserPoint(cv::Point2f imagePoint, cv::Point2f mappingPoint) {

    m_userPoints.push_back({ std::move(imagePoint), std::move(mappingPoint) });

    return &m_userPoints.back();

}

void PointManager::removeUserPoint(UserPoint* point) {

    for (auto i = m_userPoints.begin(), end = m_userPoints.end(); i != end; ++i)
        if (&*i == point) {
            m_userPoints.erase(i);
            break;
        }

}

void PointManager::clearUserPoints() {

    m_userPoints = std::list<UserPoint>();

}

float PointManager::computePixelDensity() {

    if (m_userPoints.size() < 2)
        return {};

    auto iterator = m_userPoints.begin();

    const UserPoint& pointFrom = *iterator;
    const UserPoint& pointTo = *++iterator;

    double size = cv::norm(pointTo.mappingPoint - pointFrom.mappingPoint) / cv::norm(pointTo.imagePoint - pointFrom.imagePoint);

    return static_cast<float>(size/m_scale.x);

}

void PointManager::copyImageMappingPoints(std::vector<cv::Point2f>& imagePoints, std::vector<cv::Point2f>& mappingPoints) const {

    imagePoints.clear();
    imagePoints.reserve(m_userPoints.size());

    mappingPoints.clear();
    mappingPoints.reserve(m_userPoints.size());

    for (const UserPoint& point : m_userPoints) {
        imagePoints.emplace_back(point.imagePoint);
        mappingPoints.emplace_back(point.mappingPoint);
    }

}

void PointManager::improvePoints(cv::Mat image, int searchWindowSize) {

    if (image.empty() || m_userPoints.empty()) {
        return;
    }

    std::vector<cv::Point2f> imagePoints;

    imagePoints.reserve(m_userPoints.size());

    for (const UserPoint& point : m_userPoints) {
        imagePoints.emplace_back(point.imagePoint);
    }

    cv::Mat gray;

    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::cornerSubPix(gray, imagePoints, { searchWindowSize, searchWindowSize }
                   , { -1, -1 }, { cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 30, 0.001 });

    int i = 0;

    for (auto iterator = m_userPoints.begin(), end = m_userPoints.end(); iterator != end; ++iterator) {
        iterator->imagePoint = imagePoints[i];
        i++;
    }

}

const std::vector<cv::Point2f>& PointManager::getMappingPoints() const {

    return m_mappingPoints;

}

const cv::Point2f& PointManager::getOffset() const {

    return m_offset;

}

const cv::Point2f& PointManager::getScale() const {

    return m_scale;

}

const std::list<PointManager::UserPoint>& PointManager::getUserPoints() const {

    return m_userPoints;

}

const cv::Size& PointManager::getWindowSize() const {

    return m_windowSize;

}

std::unique_ptr<PointManager> PointManager::createForBadminton( cv::Point2f scale, cv::Point2f offset) {

    std::vector<cv::Point2f> mappingPoints {

        { 0.0f, 0.0f },
        { 0.76f, 0.0f },
        { 4.72f, 0.0f },
        { 6.7f, 0.0f },
        { 8.68f, 0.0f },
        { 12.64f, 0.0f },
        { 13.4f, 0.0f },

        { 0.0f, 0.46f },
        { 0.76f, 0.46f },
        { 4.72f, 0.46f },
        { 8.68f, 0.46f },
        { 12.64f, 0.46f },
        { 13.4f, 0.46f },

        { 0.0f, 3.05f },
        { 0.76f, 3.05f },
        { 4.72f, 3.05f },
        { 8.68f, 3.05f },
        { 12.64f, 3.05f },
        { 13.4f, 3.05f },

        { 0.0f, 5.64f },
        { 0.76f, 5.64f },
        { 4.72f, 5.64f },
        { 8.68f, 5.64f },
        { 12.64f, 5.64f },
        { 13.4f, 5.64f },

        { 0.0f, 6.1f },
        { 0.76f, 6.1f },
        { 4.72f, 6.1f },
        { 6.7f, 6.1f },
        { 8.68f, 6.1f },
        { 12.64f, 6.1f },
        { 13.4f, 6.1f }

    };

    return std::make_unique<PointManager>(std::move(mappingPoints), std::move(scale), std::move(offset));

}

std::unique_ptr<PointManager> PointManager::createForBasketball(cv::Point2f scale, cv::Point2f offset ) {

    std::vector<cv::Point2f> mappingPoints {

        { 0.0f, 0.0f },
        { 8.325f, 0.0f },
        { 14.0f, 0.0f },
        { 19.675f, 0.0f },
        { 28.0f, 0.0f },
        { 0.0f, 0.9f },
        { 3.05f, 0.9f },
        { 24.95f, 0.9f },
        { 28.0f ,0.9f },

        { 0.0f, 5.05f },
        { 1.775f, 5.05f },
        { 2.65f, 5.05f },
        { 3.05f, 5.05f },
        { 3.9f, 5.05f },
        { 4.8f, 5.05f },
        { 5.8f, 5.05f },
        { 22.175f, 5.05f },
        { 23.2f, 5.05f },
        { 24.1f, 5.05f },
        { 24.95f, 5.05f },
        { 25.35f, 5.05f },
        { 26.225f, 5.05f },
        { 28.0f, 5.05f },

        { 5.8f, 5.75f },
        { 14.0f, 5.75f },
        { 22.2f, 5.75f },

        { 1.2f, 6.25f },
        { 26.8f, 6.25f },

        { 1.2f, 8.75f },
        { 26.8f, 8.75f },

        { 5.8f, 9.25f },
        { 14.0f, 9.25f },
        { 22.2f, 9.25f },

        { 0.0f, 9.95f },
        { 1.775f, 9.95f },
        { 2.65f, 9.95f },
        { 3.05f, 9.95f },
        { 3.9f, 9.95f },
        { 4.8f, 9.95f },
        { 5.8f, 9.95f },
        { 22.175f, 9.95f },
        { 23.2f, 9.95f },
        { 24.1f, 9.95f },
        { 24.95f, 9.95f },
        { 25.35f, 9.95f },
        { 26.225f, 9.95f },
        { 28.0f, 9.95f },

        { 0.0f, 14.1f },
        { 3.05f, 14.1f },
        { 24.95f, 14.1f },
        { 28.0f, 14.1f },

        { 0.0f, 15.0f },
        { 9.0f, 15.0f },
        { 14.0f, 15.0f },
        { 19.0f, 15.0f },
        { 28.0f, 15.0f }

    };

    return std::make_unique<PointManager>(std::move(mappingPoints), std::move(scale), std::move(offset));

}

std::unique_ptr<PointManager> PointManager::createCustom() {

    std::vector<cv::Point2f> mappingPoints {{}};

    return std::make_unique<PointManager>(std::move(mappingPoints));

}

std::unique_ptr<PointManager> PointManager::createForFootball(float width, float height, cv::Point2f scale, cv::Point2f offset) {

    std::vector<cv::Point2f> mappingPoints {

        { 0.0f, 0.0f },
        { 0.5f, 0.0f },
        { 9.15f, 0.0f },
        { width / 2.0f, 0.0f },
        { width - 9.15f ,0.0f },
        { width - 0.5f, 0.0f },
        { width, 0.0f },
        { 0.0f, 0.5f },
        { width, 0.5f },
        { 0.0f, 9.15f },
        { width, 9.15f },
        { 0.0f, ((height - 40.3f) / 2.0f) },
        { 16.5f, ((height - 40.3f) / 2.0f) },
        { width - 16.5f, ((height - 40.3f) / 2.0f) },
        { width, ((height - 40.3f) / 2.0f) },
        { width / 2.0f, ((height / 2.0f) - 9.15f) },
        { 0.0f, ((height - 18.3f) / 2.0f) },
        { 5.5f, ((height - 18.3f) / 2.0f) },
        { width - 5.5f, ((height - 18.3f) / 2.0f) },
        { width, ((height - 18.3f) / 2.0f) },
        { 11.0f, height / 2.0f },
        { width / 2.0f, height / 2.0f },
        { width - 11.0f, height / 2.0f },
        { 0.0f, (((height - 18.3f) / 2.0f) + 18.3f) },
        { 5.5f, (((height - 18.3f) / 2.0f) + 18.3f) },
        { width - 5.5f, (((height - 18.3f) / 2.0f) + 18.3f) },
        { width, (((height - 18.3f) / 2.0f) + 18.3f) },
        { width / 2.0f, ((height / 2.0f) + 9.15f) },
        { 0.0f, (((height - 40.3f) / 2.0f) + 40.3f)},
        { 16.5f, (((height - 40.3f) / 2.0f) + 40.3f) },
        { width - 16.5f, (((height - 40.3f) / 2.0f) + 40.3f) },
        { width, (((height - 40.3f) / 2.0f) + 40.3f) },
        { 0.0f, (height - 9.15f) },
        { width, (height - 9.15f) },
        { 0.0f, (height - 0.5f) },
        { width, (height - 0.5f) },
        { 0.0f, height },
        { 0.5f, height },
        { 9.15f, height },
        { width / 2.0f, height },
        { width - 9.15f, height },
        { width - 0.5f, height },
        { width, height }

    };

    return std::make_unique<PointManager>(std::move(mappingPoints), std::move(scale), std::move(offset));

}

std::unique_ptr<PointManager> PointManager::createForHockey(cv::Point2f scale, cv::Point2f offset) {

    std::vector<cv::Point2f> mappingPoints {

        { 21.3f, 0.0f },
        { 30.0f, 0.0f },
        { 38.7f, 0.0f },

        { 4.0f, 1.007f },
        { 56.5f, 1.007f },

        { 9.15f, 3.5f },
        { 10.85f, 3.5f },
        { 49.15f, 3.5f },
        { 50.85f, 3.5f },

        { 10.0f, 8.0f },
        { 22.8f, 8.0f },
        { 37.2f, 8.0f },
        { 50.0f, 8.0f },

        { 30.0f, 10.5f },

        { 9.15f, 12.5f },
        { 10.85f, 12.5f },
        { 49.15f, 12.5f },
        { 50.85f, 12.5f },

        { 4.0f, 13.2f },
        { 56.0f, 13.2f },

        { 4.0f, 14.085f },
        { 56.0f, 14.085f },

        { 30.0f, 15.0f },

        { 4.0f, 15.915f },
        { 56.0f, 15.915f },

        { 4.0f, 16.8f },
        { 56.0f, 16.8f },

        { 9.15f, 17.5f },
        { 10.85f, 17.5f },
        { 49.15f, 17.5f },
        { 50.85f, 17.5f },

        { 30.0f, 19.5f },

        { 10.0f, 22.0f },
        { 22.8f, 22.0f },
        { 37.2f, 22.0f },
        { 50.0f, 22.0f },

        { 9.15f, 26.5f },
        { 10.85f, 26.5f },
        { 49.15f, 26.5f },
        { 50.85f, 26.5f },

        { 30.0f, 27.0f },

        { 4.0f, 28.993f },
        { 56.5f, 28.993f },

        { 21.3f, 30.0f },
        { 30.0f, 30.0f },
        { 38.7f, 30.0f }

    };

    return std::make_unique<PointManager>(std::move(mappingPoints), std::move(scale), std::move(offset));

}

std::unique_ptr<PointManager> PointManager::createForTennis(cv::Point2f scale, cv::Point2f offset) {

    std::vector<cv::Point2f> mappingPoints {

        { 0.0f, 0.0f },
        { 11.885f, 0.0f },
        { 23.77f, 0.0f },

        { 0.0f, 1.372f },
        { 5.50f, 1.372f },
        { 11.885f, 1.372f },
        { 18.27f, 1.372f },
        { 23.77f, 1.372f },

        { 5.50f, 5.485f },
        { 11.885f, 5.485f },
        { 18.270f, 5.485f },

        { 0.0f, 9.598f },
        { 5.50f, 9.598f },
        { 11.885f, 9.598f },
        { 18.270f, 9.598f },
        { 23.770f, 9.598f },

        { 0.0f, 10.970f },
        { 11.885f, 10.970f },
        { 23.770f, 10.970f }

    };

    return std::make_unique<PointManager>(std::move(mappingPoints), std::move(scale), std::move(offset));

}

std::unique_ptr<PointManager> PointManager::createForVolleyball(cv::Point2f scale, cv::Point2f offset) {

    std::vector<cv::Point2f> mappingPoints {

        { 0.0f, 0.0f },
        { 6.0f, 0.0f },
        { 9.0f, 0.0f },
        { 12.0f, 0.0f },
        { 18.0f, 0.0f },
        { 0.0f, 9.0f },
        { 6.0f, 9.0f },
        { 9.0f, 9.0f },
        { 12.0f, 9.0f },
        { 18.0f, 9.0f }

    };

    return std::make_unique<PointManager>(std::move(mappingPoints), std::move(scale), std::move(offset));

}

void PointManager::computeWindowSize() {

    if (m_mappingPoints.empty())
        return;

    m_windowSize.width = m_mappingPoints[0].x;
    m_windowSize.height = m_mappingPoints[0].y;

    for (const cv::Point2f& point : m_mappingPoints) {
        m_windowSize.width = std::max(static_cast<int>(point.x), m_windowSize.width);
        m_windowSize.height = std::max(static_cast<int>(point.y), m_windowSize.height);
    }

}
