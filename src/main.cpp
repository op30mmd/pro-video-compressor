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

    // Set the style based on the platform
    QString style = QStyleFactory::create("Fusion") ? "Fusion" : "Windows";
    if (QStyleFactory::keys().contains("windowsvista")) {
        style = "windowsvista";  // Use Windows Vista style if available (modern Windows look)
    }
    QApplication::setStyle(style);

    // This now works because the constructor is back to the default
    MainWindow w;
    w.setWindowTitle("Pro Video Compressor");
    w.setMinimumSize(600, 800);
    w.show();

    return a.exec();
}
