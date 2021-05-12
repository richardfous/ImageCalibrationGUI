
#pragma once

#include <opencv2/opencv.hpp>

class Homography;

/// This function is used to blend 2 images containing alpha channel.
/// \param destinationImage matrix containing 1st image to blend. Output of this function is written into this matrix.
/// \param sourceImage matrix containing 2nd image to blend.
void blendImages(cv::Mat destinationImage, cv::Mat sourceImage);

/// Converts BGR image to BGRA.
/// \param inputImage without alpha channel.
/// \returns image with added alpha channel.
cv::Mat convertBGRtoBGRA(cv::Mat inputImage);

/// Transform image into birds eye view.
/// \param homography homography matrix which can be obtained from Homography class.
/// \param image matrix containing image that will be transformed.
/// \param windowSize size of the mapping window, this can be obtained from PointManager using method calculateWindowSize and getWindowSize.
/// \returns matrix containing transformed input.
cv::Mat computeBirdsEyeView(cv::Mat homography, cv::Mat image, const cv::Size& windowSize);

/// Function to remove barrel and pincushion distortion.
/// \param input matrix containing distorted image.
/// \param k distortion coefficient, set k positiove to remove barrel distortion and negative to remove pincushion distortion.
/// \param scale is used to scale the input image to get rid of black background, which is caused by removing distortion.
/// \returns matrix containing undistorted and scaled image.
cv::Mat undistort(cv::Mat input, double k, double scale);
