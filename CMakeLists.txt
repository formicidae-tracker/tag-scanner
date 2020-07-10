cmake_policy(SET CMP0048 NEW)

project(fort-tag-scanner VERSION 0.1.0
	                     LANGUAGES C CXX)

cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

find_package(Qt5 COMPONENTS Widgets REQUIRED)




add_subdirectory(src)
