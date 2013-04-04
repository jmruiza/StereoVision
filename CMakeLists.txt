cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
# Project's name
project(StereoVision)
# Build Type
set(CMAKE_BUILD_TYPE "Debug")

# Auto Generate
# aux_source_directory(. SRC_LIST)
# add_executable(${PROJECT_NAME} ${SRC_LIST})

# Enable warnings
ADD_DEFINITIONS( -Wall)
ADD_DEFINITIONS(${QT_DEFINITIONS})

# By default only QtCore and QtGui modules are enabled
# other modules must be enabled like this:
SET( QT_USE_QTXML TRUE )
SET(QT_USE_QTOPENGL TRUE)

# This command finds Qt4 libraries and sets all required variables,
# note that it's Qt4, not QT4 or qt4
FIND_PACKAGE( Qt4 REQUIRED )
# Look for required libraries and tools
FIND_PACKAGE(OpenCV REQUIRED)

# Add some useful macros and variables
# (QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt4 ) that contains
# a path to CMake script)
INCLUDE( ${QT_USE_FILE} )
INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR} )
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
add_subdirectory(source)
add_subdirectory(source-test)

