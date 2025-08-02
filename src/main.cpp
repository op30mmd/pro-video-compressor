#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QFile>
#include <QTextStream>
#include <QFont>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Enable High-DPI scaling
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    // Use the Fusion style as a base
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    // Set a modern font
    QFont font("Segoe UI", 9);
    QApplication::setFont(font);

    // Load and apply the Fluent stylesheet
    QFile file(":/src/fluent.qss");
    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream stream(&file);
        a.setStyleSheet(stream.readAll());
        file.close();
    } else {
        qWarning("Could not open fluent.qss stylesheet.");
    }

    MainWindow w;
    w.setWindowTitle("Pro Video Compressor");
    w.setMinimumSize(600, 800);
    w.show();

    return a.exec();
}
