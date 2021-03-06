# ImageCalibrationGUI
Application for camera perspective calibration and drawing into calibrated image. This application was created to show possible uses of ImageCalibrationLibrary, that can be found [here.](https://github.com/richardfous/ImageCalibrationLibrary)

## Dependencies
This application requires opencv library be installed. For more information about opencv installation visit [opencv webiste](https://opencv.org).
Qt 5.12.10 is required to build this appllication. More information about Qt can be found [here.](https://doc.qt.io/qt-5/build-sources.html)

## Installation
This application was developed in qt enviroment, so it is necessary to build it with qt library. This application was developed with Linux OS in mind, however, by following this [guide](https://wiki.qt.io/Build_Standalone_Qt_Application_for_Windows), you can create executable on Windows OS. To install this application on Linux you need to have qt installed in your system. If you have qt installed, you can build this application by running following commands in root directory of this application. 
```
qmake && make
```
This will created an executable, that can be opened by running command:
```
./ImageCalibrationGUI
```

## About the application
This application is divided into 4 section.
* Main menu - is used to display information about loaded data, and also acts as a junction point between other sections. You can load existing project or create new poject here. After loading or creating project, you can use other sections to perform different tasks.

* Distortion Calibration - this section is usefull for correcting barrel and pincushion distortion. Coefficient k is used to specify the amount of distortion, where positive value is used to correct barrel distortion and negative value is used to correct pincushion distortion. Scale is used to remove black background that can appear after distortion correction. Final coefficients can be saved using Save button, that automatically saves them to project file specified in main menu.

* Perspective calibration - is meant for computing homography matrix. After image is loaded, you can select type of sports field, that will be used as mapping points. If football is selected, width and height of football field must be specified. Scale factor affects size of mapping window, which is used for displaying birds eye view of inserted image. When selecting custom points, you need to specify your own points that will be used for mapping by means of x and y coordinates. When type is selected, you need to confirm your selection by pressing Set button. After this confirmation new image will appear, which can be used for mapping. When mapping point is selected, it will change its color to orange, signaling that this point is not paired to image. When mapping point is paired with image point it will change color to blue and image point will be displayed in image. If atleast 4 user points are inserted, method Improve position of points can be called to improve position of image points. Final homography matrix can be calculated by clicking button Calculate and then saved by clicking button Save. If mistake is made when selecting mapping point, this selection can be canceled by pressing right mouse button. 

* Draw - is used for drawing objects onto image. This section can be sellected only if valid homography matrix is saved in project. Input image can be loaded by pressing Load image button. After that, desired object type can be selected from dropdown menu. After selecting desired object, you can select position of given object in image by clicking on inserted image and render it by pressing render button. This new object will be shown in the table on the right side of the window. Different atributes of given object can be changed by double clicking appropriate cell in table. Final image can then be saved by clicking Save image button. Save alpha button is usefull if you need to obtain only created object on transparent background.

## Issues
If you find any issues with this module, feel free to open a GitHub issue in this repository. 
