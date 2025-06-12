cmake_minimum_required(VERSION 3.16)
project(VideoCompressor LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6 package and its components
find_package(Qt6 6.2 REQUIRED COMPONENTS Core Gui Widgets)

# Tell CMake to use Qt's MOC, UIC, and RCC tools automatically
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Set the CMAKE_PREFIX_PATH to help find Qt if it's not in a standard location.
# On Windows, you might need to set this explicitly, e.g.,
# cmake -DQt6_DIR=C:/Qt/6.5.0/msvc2019_64/lib/cmake/Qt6 ..
if(WIN32)
    message(STATUS "Windows detected. Ensure Qt6_DIR is set if Qt is not in the system PATH.")
endif()


# Add the executable target
# The WIN32 flag is the crucial fix. It creates a GUI executable instead of a console one.
if(WIN32)
    add_executable(VideoCompressor WIN32
        src/main.cpp
        src/MainWindow.cpp
        src/MainWindow.h
    )
else()
    add_executable(VideoCompressor
        src/main.cpp
        src/MainWindow.cpp
        src/MainWindow.h
    )
endif()


# Link the executable against the required Qt libraries
target_link_libraries(VideoCompressor PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

# Install rules (optional, but good practice)
install(TARGETS VideoCompressor
    RUNTIME DESTINATION bin
)

