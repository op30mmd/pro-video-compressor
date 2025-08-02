#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QPalette>
#include <QColor>
#include <QFont>

int main(int argc, char *argv[])
{
        QApplication a(argc, argv);

    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    // --- The Definitive Stable Theme ---
    // This uses Qt's robust, built-in "Fusion" style with a clean dark palette.
    // It is guaranteed to be stable and consistent on all platforms.
    QStyle *fusionStyle = QStyleFactory::create("Fusion");
    if (fusionStyle) {
        QApplication::setStyle(fusionStyle);
    } else {
        qWarning("Fusion style not available, falling back to default.");
    }

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

    // Custom stylesheet for a modern look
    QString styleSheet = R"(
        QSlider::groove:horizontal {
            border: 1px solid #5A5A5A;
            height: 4px;
            background: #393939;
            margin: 0px;
            border-radius: 2px;
        }

        QSlider::handle:horizontal {
            background: #42a5f5;
            border: 1px solid #42a5f5;
            width: 16px;
            height: 16px;
            margin: -6px 0;
            border-radius: 8px;
        }

        QSlider::groove:horizontal:hover {
            background: #4a4a4a;
        }

        QSlider::handle:horizontal:hover {
            background: #64b5f6;
            border: 1px solid #64b5f6;
        }

        QSlider::handle:horizontal:pressed {
            background: #2196f3;
            border: 1px solid #2196f3;
        }
    )";
    a.setStyleSheet(styleSheet);

    // This now works because the constructor is back to the default
    MainWindow w;
    w.setWindowTitle("Pro Video Compressor");
    w.setMinimumSize(600, 800);
    w.show();

    return a.exec();
}
