cmake_minimum_required(VERSION 3.16)
project(VideoCompressor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find only the necessary Qt components
find_package(Qt6 6.4 REQUIRED COMPONENTS Core Gui Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Add only the necessary source files to the executable
add_executable(VideoCompressor
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
)

# Set properties for each OS
set_target_properties(VideoCompressor PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)

# Link the executable against the required Qt libraries
target_link_libraries(VideoCompressor PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

# Standard install rules
install(TARGETS VideoCompressor
    BUNDLE DESTINATION .
    RUNTIME DESTINATION .
)
