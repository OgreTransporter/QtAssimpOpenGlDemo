cmake_minimum_required(VERSION 3.5)

project(QtAssimpOpenGlDemo VERSION 0.1 LANGUAGES CXX DESCRIPTION "An example project using QT and OpenGL")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_DEBUG_POSTFIX d)

# For CLion builds, point CMAKE_PREFIX_PATH to Qt6 install directory
list(APPEND CMAKE_PREFIX_PATH $ENV{HOME}/Code/Clones/Qt/6.3.1/gcc_64/)
find_package(Qt6 COMPONENTS OpenGLWidgets)
if (NOT Qt6_FOUND)
  message(SEND_ERROR "Unable to find Qt6 at CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
  message(FATAL_ERROR "Specify path to Qt6 with `cmake -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x.x/gcc_64 -S /path/to/qtk -B /path/to/qtk/build && cmake --build /path/to/qtk/build -j $(nprocs)`")
endif()

add_subdirectory(lib)
add_subdirectory(app)
