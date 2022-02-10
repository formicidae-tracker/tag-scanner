set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include_directories(${PROJECT_SOURCE_DIR}/src
                    ${PROJECT_BINARY_DIR}/src)


set(SRC_FILES main.cpp
              MainWindow.cpp
              CameraSettings.cpp
              ApriltagSettings.cpp
              ApriltagDetector.cpp
              DetectionProcess.cpp
              DetectionView.cpp
              )

set(HDR_FILES MainWindow.hpp
              CameraSettings.hpp
              ApriltagSettings.hpp
              ApriltagDetector.hpp
              DetectionProcess.hpp
              DetectionView.hpp
              )

set(UI_FILES MainWindow.ui
             CameraSettings.ui
             ApriltagSettings.ui)

set(RCC_FILES )


add_executable(fort-tag-scanner ${SRC_FILES}
                                ${HDR_FILES}
                                ${UI_FILES}
                                ${RCC_FILES}
                                )

find_library(FORT_TIME_LIBRARY fort-time)

target_link_libraries(fort-tag-scanner Qt5::Widgets
                                       Qt5::Multimedia
                                       ${FORT_TAGS_LIBRARIES}
                                       ${FORT_MYRMIDON_LIBRARIES}
                                       ${FORT_TIME_LIBRARY}
                                       )

set_target_properties(fort-tag-scanner PROPERTIES VERSION ${PROJECT_VERSION})

install(TARGETS fort-tag-scanner DESTINATION bin)
