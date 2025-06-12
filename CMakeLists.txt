cmake_minimum_required(VERSION 3.16)
project(VideoCompressor LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6 package and its components
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Widgets)

# Tell CMake to use Qt's MOC, UIC, and RCC tools automatically
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Add the executable target
add_executable(VideoCompressor
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
)

# --- The Definitive Fix ---
# This section correctly sets the properties for each OS,
# ensuring a proper .app bundle is created on macOS.
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

# Install rules for packaging
install(TARGETS VideoCompressor
    BUNDLE DESTINATION .
    RUNTIME DESTINATION .
)

