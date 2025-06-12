cmake_minimum_required(VERSION 3.16)
project(VideoCompressor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Widgets Svg)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(VideoCompressor
    resources.qrc
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/ThemeManager.cpp
    src/ThemeManager.h
)

set_target_properties(VideoCompressor PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)

target_link_libraries(VideoCompressor PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Svg
)

install(TARGETS VideoCompressor
    BUNDLE DESTINATION .
    RUNTIME DESTINATION .
)
