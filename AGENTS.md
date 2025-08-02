## AGENTS.md

The project is a cross-platform desktop application built using the **Qt framework** and **C++17**.

  * **`src/`**: Contains all the C++ source code for the application.
      * `main.cpp`: The application's entry point. It initializes the Qt application and the main window.
      * `mainwindow.h` / `mainwindow.cpp`: Implements the main user interface and its logic.
      * `ffmpeg_manager.h`: Defines the class responsible for managing `ffmpeg` processes for video compression.
      * `compression_settings.h`: Defines the data structure for holding user-selected compression settings.
  * **`.github/workflows/`**: Contains GitHub Actions workflows for continuous integration and release builds.
  * **`CMakeLists.txt`**: The main build script for the project, used by CMake to generate platform-specific build files.
  * **`videocompressor.svg`**: The application icon, used in packaging.

-----

## Build and Development Commands

The project uses **CMake** for building. The general steps are to configure the project and then build it.

**Dependencies:**

  * **Qt 6.5+**: Required for the application framework.
  * **FFmpeg**: Must be installed and available in the system's `PATH` for the application to function correctly. The build workflow installs it via `apt` on Linux, `brew` on macOS, and `choco` on Windows.

**Local Build Commands:**

1.  **Configure CMake**:

    ```bash
    cmake . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=/path/to/Qt
    ```

      * Replace `/path/to/Qt` with the actual path to your Qt installation.

2.  **Build with CMake**:

    ```bash
    cmake --build build --config Release
    ```

-----

## Code Style and Conventions

  * **Framework**: Use **Qt conventions** for signal/slot connections, memory management (parent-child relationships), and UI development.
  * **Language**: Code is written in **C++17**.
  * **UI Separation**: Keep UI logic in `MainWindow` and separate the core video processing logic into other classes like `FFmpegManager`.
  * **Asynchronous Operations**: Use `QProcess` for long-running tasks like video compression to avoid blocking the UI thread.
  * **User Feedback**: Provide continuous feedback to the user via progress bars and log messages, especially during compression.

-----

## Architecture and Design

The application follows a straightforward GUI architecture:

1.  **MainWindow**: Serves as the primary user interface. It is responsible for gathering user input (video files, output directory, compression settings) and displaying progress and logs.
2.  **FFmpeg Interaction**: The `MainWindow` uses a `QProcess` object (`ffmpegProcess`) to execute `ffmpeg` and `ffprobe` command-line tools. It builds the command-line arguments based on the user's settings.
3.  **Settings Management**: A `CompressionSettings` struct is used to pass the various encoding options from the UI to the compression logic in a clean and organized way.
4.  **Hardware Acceleration**: The application probes for available hardware encoders (NVIDIA, AMD, Intel) using `ffmpeg` and dynamically populates the UI with these options.
