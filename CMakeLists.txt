cmake_policy(SET CMP0048 NEW)

project(
	fort-tag-scanner
	VERSION 0.2.2
	LANGUAGES C CXX
)

cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

find_package(
	Qt6
	COMPONENTS Widgets Multimedia
	REQUIRED
)

include(FetchContent)
FetchContent_Declare(
	myrmidon
	GIT_REPOSITORY https://github.com/formicidae-tracker/myrmidon
	GIT_TAG 23231f7e474961e2e3e56dbd907158c35956f968
)
FetchContent_MakeAvailable(myrmidon)

add_subdirectory(src)
