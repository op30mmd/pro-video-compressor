#include "mainwindow.h"
#include "win32_helpers.h"
#include <QApplication>
#include <QStyleFactory>
#include <QPalette>
#include <QColor>
#include <QFont>
#include <QSettings>
#include <QOperatingSystemVersion>

int main(int argc, char *argv[])
{
    // Enable modern DPI scaling before creating QApplication
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication a(argc, argv);

    // Enable modern style hints
    QApplication::setAttribute(Qt::AA_DontUseNativeMenuBar, false);
    QApplication::setAttribute(Qt::AA_DontShowIconsInMenus, false);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    // Force the application to use the Windows Vista style (modern Windows style)
    if (QStyleFactory::keys().contains("WindowsVista")) {
        QApplication::setStyle(QStyleFactory::create("WindowsVista"));
    } else if (QStyleFactory::keys().contains("Windows")) {
        QApplication::setStyle(QStyleFactory::create("Windows"));
    }

    // This now works because the constructor is back to the default
    MainWindow w;
    w.setWindowTitle("Pro Video Compressor");
    w.setMinimumSize(600, 800);

#ifdef _WIN32
    // Enable WinUI 3.0 Mica material if on Windows 11
    if (QOperatingSystemVersion::current() >= QOperatingSystemVersion::Windows10) {
        // Set the window background to be transparent
        w.setAttribute(Qt::WA_TranslucentBackground);
        // Enable blur behind the window
        HWND hwnd = (HWND)w.winId();
        BOOL value = TRUE;
        DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &value, sizeof(value));
        if (QOperatingSystemVersion::current() >= QOperatingSystemVersion::Windows11) {
            // Enable Mica material
            DWORD cornerValue = DWMWCP_DONOTROUND;
            DwmSetWindowAttribute(hwnd, DWMWA_WINDOW_CORNER_PREFERENCE, &cornerValue, sizeof(cornerValue));
            
            int backdropType = DWMSBT_MAINWINDOW;
            DwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, &backdropType, sizeof(backdropType));
        }
    }
#endif

    w.show();
    return a.exec();
}
