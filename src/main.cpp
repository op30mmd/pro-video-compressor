#include "mainwindow.h"
#include <QApplication>
#include "ThemeManager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Create the theme manager
    ThemeManager themeManager;

    // Apply the default theme on startup
    themeManager.applyTheme(ThemeManager::System);

    // --- THE FIX ---
    // Pass the themeManager instance to the MainWindow constructor.
    MainWindow w(&themeManager);

    w.setWindowTitle("Pro Video Compressor");
    w.setMinimumSize(600, 800);
    w.show();

    return a.exec();
}
