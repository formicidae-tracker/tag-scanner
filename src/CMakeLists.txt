set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include_directories(${PROJECT_SOURCE_DIR}/src
                    ${PROJECT_BINARY_DIR}/src)


set(SRC_FILES main.cpp
              MainWindow.cpp
              CameraSettings.cpp
              ApriltagSettings.cpp
              Camera.cpp)

set(HDR_FILES MainWindow.hpp
              CameraSettings.hpp
              ApriltagSettings.hpp
              Camera.hpp)

set(UI_FILES MainWindow.ui
             CameraSettings.ui
             ApriltagSettings.ui)

set(RCC_FILES )


add_executable(fort-tag-scanner ${SRC_FILES}
                                ${HDR_FILES}
                                ${UI_FILES}
                                ${RCC_FILES}
                                )

target_link_libraries(fort-tag-scanner Qt5::Widgets
                                       ${OpenCV_LIBS}
                                       )

set_target_properties(fort-tag-scanner PROPERTIES VERSION ${PROJECT_VERSION})

install(TARGETS fort-tag-scanner DESTINATION bin)
