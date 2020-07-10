set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


set(SRC_FILES main.cpp
	          MainWindow.cpp)

set(HDR_FILES MainWindow.hpp)

set(UI_FILES MainWindow.ui)

set(RCC_FILES )


add_executable(fort-tag-scanner ${SRC_FILES}
                                ${HDR_FILES}
                                ${UI_FILES}
                                ${RCC_FILES}
                                )

target_link_libraries(fort-tag-scanner Qt5::Widgets)

set_target_properties(fort-tag-scanner PROPERTIES VERSION ${PROJECT_VERSION})

install(TARGETS fort-tag-scanner DESTINATION bin)
