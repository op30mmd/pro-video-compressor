#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QPalette>
#include <QColor>
#include <QFont>

int main(int argc, char *argv[])
{
    // Enable modern DPI scaling before creating QApplication
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication a(argc, argv);

    // Enable modern style hints
    QApplication::setAttribute(Qt::AA_DontUseNativeMenuBar, false);
    QApplication::setAttribute(Qt::AA_DontShowIconsInMenus, false);
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    // Set modern Windows style
    if (QStyleFactory::keys().contains("Windows")) {
        QApplication::setStyle(QStyleFactory::create("Windows"));
        qApp->setStyle("Windows");
    }

    // This now works because the constructor is back to the default
    MainWindow w;
    w.setWindowTitle("Pro Video Compressor");
    w.setMinimumSize(600, 800);
    w.show();

    return a.exec();
}
