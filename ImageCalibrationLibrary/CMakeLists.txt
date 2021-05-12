
cmake_minimum_required(VERSION 3.0)

project(imageCalibrationLibrary)

include_directories(
    "include"
    "3rdparty/opencv/include"
)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-pedantic -Wall -Wextra)

file(GLOB_RECURSE IMAGECALIBRATIONLIBRARY_SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

add_library(imageCalibrationLibrary STATIC ${IMAGECALIBRATIONLIBRARY_SOURCES})
