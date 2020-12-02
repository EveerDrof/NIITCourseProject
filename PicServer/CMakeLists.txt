cmake_minimum_required(VERSION 3.5)
project(PicServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(
    /home/michael/Libs/C++/boost_1_74_0
    /home/michael/Projects/QT/Loader/include
    /home/michael/Projects/QT/Image/include
    )
link_directories(
    /home/michael/Libs/C++/boost_1_74_0/stage/lib
    /home/michael/Projects/QT/build-Loader-Desktop-Debug
    /home/michael/Projects/QT/build-Image-Desktop-Debug
    )
add_executable(PicServer main.cpp)
target_link_libraries(PicServer boost_coroutine boost_chrono boost_thread boost_system pthread Loader Image)
