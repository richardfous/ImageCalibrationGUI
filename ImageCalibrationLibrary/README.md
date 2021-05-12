# ImageCalibrationLibrary
Library for camera perspective calibration and drawing into calibrated image.

## Dependencies
This library requires opencv library be installed. For more information about opencv installation visit [opencv webiste](https://opencv.org).
Minimum required version of Cmake is 3.0 with C++17 standard. More information about cmake can be obtained at [cmake webiste](https://cmake.org).

## Installation
This library can be installed using cmake. You need to either specify location of installed opencv in CMakeLists.txt or insert opencv library into 3rd party folder, which should be place in the root folder of this library. This library can be compiled on Windows, Linux and MacOS. To compile this library on Windows, you need to install cmake application and then follow this [guide](https://cmake.org/runningcmake/). To compile this library on Linux and MacOS, you can use the following command: 

```
mkdir build && cd build && cmake .. && make
```

This creates a library file in the build folder, which you can then insert with the content of include folder into your own application. 

If you are planning on using this library with QtCreator, all you need to do is copy the ImageCalibrationLibrary folder into your existing code, and then include the .pri file in your .pro file.

```
include(ImageCalibrationLibrary/ImageCalibrationLibrary.pri)
```

## Examples
If you want to calculate homography of given image, you need to specify pairs of image and mapping points (called user points). The minimum number of pairs to be able to calculate homography is 4. Class Homography takes point manager as a input for homography calculation. First you need to create an intance of PointManager class and then fill it with user user points. This can be achieved by either using create method if the sport you are looking for is present in this library or by using createCustom, which allows you to insert your own mapping points. 

```
//Create instance of class PointManager.
std::unique_ptr<PointManager> m_pointManager = PointManager::create... ;

//Add user points to existing PointManager 
m_pointManager->addUserPoint(m_imagePoint, m_mappingPoint);

```
To calculate homography you need to crate new instance of class Homography with pointer to the existing PointManager as a argument, which will calculate the homography from user points in PointManager.
```
//Create instance of class Homography
std::shared_ptr<Homography> m_homography = std::make_shared<Homography>(*m_pointManager);

//Obtain the calculated homography matrix.
m_homography->getHomographyMatrix();
```

After you obtain homography matrix, you can use this matrix to draw different objects in drawbles to given image:
```
//Create instance of renderer, which will be used to store and render different objects. 
Renderer m_renderer;

//Create new instance of object and set different parameters (horizontal line in this example).
std::unique_ptr<Line> line = Line::create(m_homography, Line::Type::Horizontal, { m_imagePoints[0].x, m_imagePoints[0].y }, m_pointManager->getWindowSize);
line->setAlpha(m_alpha);
line->setColor(m_color);
line->setThickness(m_thickness);

//Add new object to image (horizontal line in this example).
m_renderer.addDrawable(std::move(line));
```
Finally you can render all objects in renderer to image or retrieve all objects on transparent background.
```
//Render all objects in renderer.
m_renderer.render();

//Get output image, with or without background (determined by bool argument).
m_renderer.getOutputImage();
```

## Issues
If you find any issues with this module, feel free to open a GitHub issue in this repository. 
