# SmartHHC.pro   2018-11-21 15:58:47
# CMakeLists.txt 2020-12-05

cmake_minimum_required(VERSION 3.14)

project(smarthhc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# External configuration CMake scripts
#set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if (ANDROID)
    set (SMARTHHC_DEVICE Bluetooth)
else()
    set (SMARTHHC_DEVICE SerialPort)
endif()

find_package (QT NAMES Qt6 Qt5)
find_package (Qt${QT_VERSION_MAJOR} COMPONENTS Widgets ${SMARTHHC_DEVICE} LinguistTools REQUIRED)
find_package (Qt${QT_VERSION_MAJOR}LinguistTools)

add_definitions(-DQT_DEPRECATED_WARNINGS)

set (RESOURCE_FILES
    resources/resources.qrc
)
set (SOURCE_FILES
    src/ui/CurveWidget.h
    src/ui/CurveEditor.h
    src/ui/CurveToolBar.h
    src/ui/CurvePage.h
    src/ui/PageAbout.h
    src/ui/PageConnection.h
    src/ui/PageHome.h
    src/ui/PageTerminal.h
    src/ui/MainWindow.h
    src/settings.h
    src/device.h
    src/setup.h
    src/ui/CurveWidget.cpp
    src/ui/CurveEditor.cpp
    src/ui/CurvePage.cpp
    src/ui/CurveToolBar.cpp
    src/ui/PageAbout.cpp
    src/ui/PageConnection.cpp
    src/ui/PageHome.cpp
    src/ui/PageTerminal.cpp
    src/ui/MainWindow.cpp
    src/device.cpp
    src/main.cpp
)
if (ANDROID)
    list (APPEND SOURCE_FILES
        src/bluetooth.h
        src/bluetooth.cpp)
else()
    list (APPEND SOURCE_FILES
        src/serial.h
        src/serial.cpp)
endif()
set (UI_FILES
    src/ui/PageAbout.ui
    src/ui/PageConnection.ui
    src/ui/PageHome.ui
    src/ui/PageTerminal.ui
)
source_group ("Resource Files" FILES ${RESOURCE_FILES})
source_group ("UI Files"       FILES ${UI_FILES})

set (TS_FILES resources/translations/${PROJECT_NAME}_it_IT.ts)
qt5_create_translation (QM_FILES
    ${SOURCE_FILES}
    ${UI_FILES}
    ${TS_FILES}
)
add_executable (${PROJECT_NAME}
    ${QM_FILES}
    ${RESOURCE_FILES}
    ${SOURCE_FILES}
    ${UI_FILES}
)
add_subdirectory(external/smartwidgets EXCLUDE_FROM_ALL)

target_include_directories (${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/external/smartwidgets/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries (${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::${SMARTHHC_DEVICE}
    smartwidgets
)
if (UNIX AND NOT APPLE)
    include (GNUInstallDirs)
    install (TARGETS ${PROJECT_NAME}
        DESTINATION ${CMAKE_INSTALL_BINDIR})
    install (DIRECTORY resources/icons/posix/
        DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor)
    install (FILES resources/linux/${PROJECT_NAME}.appdata.xml
        DESTINATION ${CMAKE_INSTALL_DATADIR}/metainfo)
    install (FILES resources/linux/${PROJECT_NAME}.desktop
        DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
    install (FILES ${QM_FILES}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}/translations)
endif()
