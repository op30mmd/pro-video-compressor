#ifndef WIN32_HELPERS_H
#define WIN32_HELPERS_H

#ifdef _WIN32
#include <windows.h>
#include <dwmapi.h>

// Define constants that might not be available in older Windows SDK
#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif

#ifndef DWMWA_WINDOW_CORNER_PREFERENCE
#define DWMWA_WINDOW_CORNER_PREFERENCE 33
#endif

#ifndef DWMWA_SYSTEMBACKDROP_TYPE
#define DWMWA_SYSTEMBACKDROP_TYPE 38
#endif

#ifndef DWMWCP_DONOTROUND
#define DWMWCP_DONOTROUND 1
#endif

#endif // _WIN32
#endif // WIN32_HELPERS_H 