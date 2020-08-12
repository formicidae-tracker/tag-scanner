cmake_policy(SET CMP0048 NEW)

project(fort-tag-scanner VERSION 0.2.0
	                     LANGUAGES C CXX)

cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

find_package(Qt5 COMPONENTS Widgets Multimedia REQUIRED)

find_package(FortTags 1.3.0 REQUIRED)
find_package(FortMyrmidon 0.6.0 REQUIRED)

include_directories(${FORT_MYRMIDON_INCLUDE_DIRS}
                    ${FORT_TAGS_INCLUDE_DIRS})


add_subdirectory(src)
