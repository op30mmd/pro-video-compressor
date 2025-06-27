#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QPalette>
#include <QColor>
#include <QFont>

int main(int argc, char *argv[])
{
        QApplication a(argc, argv);

    // Enable High-DPI scaling
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    // Set the application style. On Windows, use the native theme for a platform-integrated
    // look and feel. On other platforms, use the "Fusion" style for a clean, consistent appearance.
#ifdef Q_OS_WIN
    QApplication::setStyle("windowsvista");
#else
    QApplication::setStyle("Fusion");
#endif

    // Set the application style. On Windows, use the native theme for a platform-integrated
    // look and feel. On other platforms, use the "Fusion" style for a clean, consistent appearance.
#ifdef Q_OS_WIN
    QApplication::setStyle("windowsvista");
#else
    QApplication::setStyle("Fusion");
#endif

    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(40, 40, 40));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    darkPalette.setColor(QPalette::Disabled, QPalette::Text, Qt::darkGray);
    darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, Qt::darkGray);

    QApplication::setPalette(darkPalette);

    QFont font("Segoe UI", 9);
    QApplication::setFont(font);

    // This now works because the constructor is back to the default
    MainWindow w;
    w.setWindowTitle("Pro Video Compressor");
    w.setMinimumSize(600, 800);
    w.show();

    return a.exec();
}
