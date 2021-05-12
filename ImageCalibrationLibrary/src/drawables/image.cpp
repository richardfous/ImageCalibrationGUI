
#include "drawables/image.hpp"

#include "context.hpp"
#include "homography.hpp"

Image::Image(std::shared_ptr<Homography> homography)
    : Drawable { std::move(homography) }
{
}

void Image::draw(Context& context) {

    cv::Mat rotatedImage = m_image.clone();

    switch (m_rotation) {

        case Rotation::_90:

            cv::rotate(rotatedImage, rotatedImage, cv::ROTATE_90_CLOCKWISE);
            break;

        case Rotation::_180:

            cv::rotate(rotatedImage, rotatedImage, cv::ROTATE_180);
            break;

        case Rotation::_270:

            cv::rotate(rotatedImage, rotatedImage, cv::ROTATE_90_COUNTERCLOCKWISE);
            break;

        default:

            break;

    }

    cv::Mat mask(context.getSize(), CV_8UC1, cv::Scalar(0));

    std::vector<cv::Point2f> transformedPoints { m_from, m_to };

    cv::perspectiveTransform(transformedPoints, transformedPoints, m_homography->getHomographyMatrix());

    std::vector<cv::Point2f> imageHomographyPoints {

        { transformedPoints[0].x, transformedPoints[0].y },
        { transformedPoints[1].x, transformedPoints[0].y },
        { transformedPoints[1].x, transformedPoints[1].y },
        { transformedPoints[0].x, transformedPoints[1].y }

    };

    cv::Size size(static_cast<int>(std::ceil(std::abs(transformedPoints[1].x - transformedPoints[0].x)))
                , static_cast<int>(std::ceil(std::abs(transformedPoints[1].y - transformedPoints[0].y))));

    cv::resize(rotatedImage, rotatedImage, size);

    if(transformedPoints[1].x > transformedPoints[0].x){

        if(transformedPoints[0].y > transformedPoints[1].y){

            cv::rotate(rotatedImage, rotatedImage, cv::ROTATE_90_COUNTERCLOCKWISE);

        }

    }else{

        if(transformedPoints[0].y > transformedPoints[1].y){

            cv::rotate(rotatedImage, rotatedImage, cv::ROTATE_180);

        }else{

            cv::rotate(rotatedImage, rotatedImage, cv::ROTATE_90_CLOCKWISE);

        }

    }

    cv::Point position = {static_cast<int>(std::min(transformedPoints[0].x,transformedPoints[1].x)),static_cast<int>(std::min(transformedPoints[0].y,transformedPoints[1].y))};



    if (rotatedImage.channels() == 4){
        // Picture with alpha channel
        cv::Mat channels[4];

        cv::split(rotatedImage, channels);

        cv::Mat imageShape;

        std::vector<cv::Mat> alphaChannel;

        alphaChannel.push_back(channels[3]);

        cv::merge(alphaChannel, imageShape);

        imageShape.copyTo(mask({ position.x, position.y, rotatedImage.cols, rotatedImage.rows }));

        cv::warpPerspective(mask, mask, m_homography->getInverseHomographyMatrix(), context.getSize());

        cv::GaussianBlur(mask, mask, { 25, 25 }, 0.0);

        cv::threshold(mask, mask, 128.0, 255.0, cv::THRESH_BINARY);

    }else {
        // Picture without alpha channel
        cv::perspectiveTransform(imageHomographyPoints, imageHomographyPoints, m_homography->getInverseHomographyMatrix());

        std::vector<cv::Point> tempWarpedPoints {

            { static_cast<int>(std::ceil(imageHomographyPoints[0].x)), static_cast<int>(std::ceil(imageHomographyPoints[0].y)) },
            { static_cast<int>(std::ceil(imageHomographyPoints[1].x)), static_cast<int>(std::ceil(imageHomographyPoints[1].y)) },
            { static_cast<int>(std::ceil(imageHomographyPoints[2].x)), static_cast<int>(std::ceil(imageHomographyPoints[2].y)) },
            { static_cast<int>(std::ceil(imageHomographyPoints[3].x)), static_cast<int>(std::ceil(imageHomographyPoints[3].y)) }

        };

        cv::fillConvexPoly(mask, tempWarpedPoints, { 255.0, 255.0, 255.0 });
    }

    if(rotatedImage.channels() == 3){

        rotatedImage = convertBGRtoBGRA(rotatedImage).clone();

    }

    cv::Mat warpedImage(context.getSize(), rotatedImage.type(), { 0, 0, 0, 0 });

    rotatedImage.copyTo(warpedImage({ position.x, position.y, rotatedImage.cols, rotatedImage.rows }));

    cv::warpPerspective(warpedImage, warpedImage, m_homography->getInverseHomographyMatrix(), context.getSize());

    cv::Mat background = context.getImage().clone();

    warpedImage.copyTo(background,mask);

    cv::addWeighted(background, m_alpha, context.getImage(), 1.0 - m_alpha, 0.0, context.getImage());

}

const cv::Point2f& Image::getFrom() const {

    return m_from;

}

cv::Mat Image::getImage() const {

    return m_image;

}

Image::Rotation Image::getRotation() const {

    return m_rotation;

}

const cv::Point2f& Image::getTo() const {

    return m_to;

}

void Image::setFrom(cv::Point2f from) {

    m_from = std::move(from);

}

void Image::setImage(cv::Mat image) {

    m_image = image.clone();

}

void Image::setRotation(Rotation rotation) {

    m_rotation = rotation;

}

void Image::setTo(cv::Point2f to) {

    m_to = std::move(to);

}

std::unique_ptr<Image> Image::create(std::shared_ptr<Homography> homography, cv::Point2f from, cv::Point2f to, cv::Mat sourceImage, Rotation rotation) {

    auto image = std::make_unique<Image>(std::move(homography));

    image->setFrom(std::move(from));
    image->setImage(sourceImage);
    image->setRotation(rotation);
    image->setTo(std::move(to));

    return image;

}
