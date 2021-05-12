
#include "utils.hpp"

#include "homography.hpp"
#include "pointManager.hpp"

void blendImages(cv::Mat destinationImage, cv::Mat sourceImage) {

	for (int x = 0; x < destinationImage.cols; x++)
		for (int y = 0; y < destinationImage.rows; y++) {
			auto& destination = destinationImage.at<cv::Vec4b>(y, x);
			auto& source = sourceImage.at<cv::Vec4b>(y, x);

			float alpha = source[3] / 255.0f;

			// Merge the color channels.
			for (int i = 0; i < 4; i++)
				destination[i] = static_cast<uchar>((1.0 - alpha) * destination[i] + alpha * source[i]);
		}

}

cv::Mat convertBGRtoBGRA(cv::Mat inputImage){

    cv::Mat output;

    cv::Mat channels[3];

    cv::split(inputImage, channels);

    std::vector<cv::Mat> imageWithAlpha;

    cv::Mat alpha(inputImage.size(), CV_8UC1, cv::Scalar(255));

    imageWithAlpha.push_back(channels[0]);
    imageWithAlpha.push_back(channels[1]);
    imageWithAlpha.push_back(channels[2]);
    imageWithAlpha.push_back(alpha);

    cv::merge(imageWithAlpha,output);

    return output;

}

cv::Mat computeBirdsEyeView(cv::Mat homography, cv::Mat image, const cv::Size& windowSize) {

    if (image.empty() || homography.empty())
        return {};

    cv::Mat output;

    cv::warpPerspective(image, output, homography,windowSize);

    return output;

}

cv::Mat undistort(cv::Mat input, double k, double scale){

    cv::Mat undistortedImage(input.rows, input.cols, input.type(),cv::Scalar(0,0,0));

    double r;

    double undistX,undistY;

    double midX = input.cols/2;
    double midY = input.rows/2;

    for(int x = 0; x < input.cols; x++){

        for(int y = 0; y < input.rows; y++){

            r = std::pow((x-midX),2) + std::pow((y-midY),2);

            undistX = ((x-midX))/(1 - k*r)*scale;
            undistY = ((y-midY))/(1 - k*r)*scale;

            if ( std::round(undistX+midX) >= 0 && std::round(undistY+midY) >= 0 && std::round(undistX+midX) < input.cols && std::round(undistY+midY) < input.rows ){
            undistortedImage.at<cv::Vec3b>(y,x) = input.at<cv::Vec3b>(undistY+midY,undistX+midX);
            }


        }

    }

    return undistortedImage;

}
