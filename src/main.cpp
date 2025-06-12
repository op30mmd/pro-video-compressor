#include "mainwindow.h" // FIX: Changed to lowercase to match filename
#include <QApplication>
#include <QStyleFactory>
#include <QFont>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#ifdef Q_OS_WIN
    QStringList styles = QStyleFactory::keys();
    if (styles.contains("windows11", Qt::CaseInsensitive)) {
        a.setStyle(QStyleFactory::create("windows11"));
    } else if (styles.contains("windowsvista", Qt::CaseInsensitive)) {
        a.setStyle(QStyleFactory::create("windowsvista"));
    }
#endif

    QFont font("Samsung Sans", 10);
    QApplication::setFont(font);

    MainWindow w;
    w.setWindowTitle("Pro Video Compressor");
    w.setMinimumSize(600, 800);
    w.show();

    return a.exec();
}
