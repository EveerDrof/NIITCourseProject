cmake_minimum_required(VERSION 3.14)

project(Loader LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

include_directories(/home/michael/Libs/C++/boost_1_74_0 /home/michael/Projects/QT/Image/include)
link_directories( /home/michael/Projects/QT/build-Loader-Desktop-Debug)
add_library(Loader STATIC include/Loader.h Loader.cpp)

target_link_libraries(Loader PRIVATE Qt${QT_VERSION_MAJOR}::Core
    boost_coroutine boost_chrono boost_thread boost_system pthread Image)
target_compile_definitions(Loader PRIVATE LOADER_LIBRARY)
