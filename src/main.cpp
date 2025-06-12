#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QFont>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

// This is the correct and only code needed to enable native styling.
// The qt.conf file created by the workflow will ensure the necessary
// plugins are found and loaded by the executable.
#ifdef Q_OS_WIN
    QStringList styles = QStyleFactory::keys();
    if (styles.contains("windows11", Qt::CaseInsensitive)) {
        a.setStyle(QStyleFactory::create("windows11"));
    } else if (styles.contains("windowsvista", Qt::CaseInsensitive)) {
        a.setStyle(QStyleFactory::create("windowsvista"));
    }
#endif

    // Set the application font.
    QFont font("Samsung Sans", 10);
    QApplication::setFont(font);

    MainWindow w;
    w.setWindowTitle("Pro Video Compressor");
    w.setMinimumSize(600, 800);
    w.show();

    return a.exec();
}
