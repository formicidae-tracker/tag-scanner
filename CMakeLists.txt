cmake_policy(SET CMP0048 NEW)

project(fort-tag-scanner VERSION 0.1.0
	                     LANGUAGES C CXX)

cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

find_package(OpenCV REQUIRED core imgcodecs imgproc videoio)

find_package(PkgConfig REQUIRED)

find_package(FortMyrmidon REQUIRED)
find_package(FortTags REQUIRED)
find_package(Eigen3 REQUIRED)

pkg_check_modules(APRILTAG apriltag REQUIRED)

include_directories(${FORT_MYRMIDON_INCLUDE_DIRS}
                    ${FORT_TAGS_INCLUDE_DIRS}
                    ${EIGEN3_INCLUDE_DIRS}
                    ${APRILTAG_INCLUDE_DIRS})


add_subdirectory(src)
