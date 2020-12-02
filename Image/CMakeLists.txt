cmake_minimum_required(VERSION 3.14)

project(Image LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

add_library(Image STATIC
Drawer.cpp  Image.cpp  ImgStreamer.cpp  PixelPainter.cpp
include/Drawer.h  include/Image.h  include/ImgStreamer.h  include/PixelPainter.h
)

target_link_libraries(Image PRIVATE Qt${QT_VERSION_MAJOR}::Core)

target_compile_definitions(Image PRIVATE IMAGE_LIBRARY)
