cmake_policy(SET CMP0048 NEW)

project(fort-tag-scanner VERSION 0.1.0
	                     LANGUAGES C CXX)

cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

find_package(OpenCV REQUIRED core imgcodecs imgproc videoio)

find_package(FortMyrmidon 0.6.0 REQUIRED)

include_directories(${FORT_MYRMIDON_INCLUDE_DIRS})


add_subdirectory(src)
