#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QFont>
#include <QFontDatabase>

// A comprehensive, professional stylesheet to guarantee a consistent look.
const QString STYLESHEET = R"(
    #MainWindow, #contentWidget, QScrollArea {
        background-color: #2b2b2b;
        border: none;
    }
    QGroupBox {
        background-color: #3c3c3c;
        border: 1px solid #555555;
        border-radius: 8px;
        margin-top: 1em;
        font-weight: bold;
        color: #f0f0f0;
        padding: 15px;
    }
    QGroupBox::title {
        subcontrol-origin: margin;
        subcontrol-position: top left;
        padding: 0 10px;
        left: 10px;
        color: #cccccc;
    }
    QLabel {
        color: #cccccc;
        font-weight: normal;
    }
    QPushButton {
        background-color: #555555;
        color: #f0f0f0;
        border: 1px solid #666666;
        padding: 8px 16px;
        border-radius: 4px;
        font-weight: bold;
    }
    QPushButton:hover {
        background-color: #6a6a6a;
    }
    QPushButton:pressed {
        background-color: #4a4a4a;
    }
    QPushButton:disabled {
        background-color: #404040;
        color: #888888;
        border-color: #555555;
    }
    #compressButton {
        background-color: #0078d7;
        color: white;
    }
    #cancelButton {
        background-color: #444444;
        color: #f0f0f0;
    }
    QLineEdit, QTextEdit, QSpinBox, QComboBox {
        background-color: #2b2b2b;
        border-radius: 4px;
        padding: 6px;
        color: #f0f0f0;
        border: 1px solid #555555;
    }
    QLineEdit:focus, QTextEdit:focus, QSpinBox:focus, QComboBox:focus {
        border-color: #0078d7;
    }
    QListWidget {
        background-color: #252525;
        border: 1px solid #555555;
        border-radius: 4px;
        color: #f0f0f0;
    }
    QComboBox QAbstractItemView {
        background-color: #3c3c3c;
        border: 1px solid #666666;
        color: #f0f0f0;
        selection-background-color: #0078d7;
    }
    QSlider::groove:horizontal {
        background: #555555;
        height: 4px;
        border-radius: 2px;
    }
    QSlider::handle:horizontal {
        background: #0078d7;
        width: 18px;
        height: 18px;
        margin: -7px 0;
        border-radius: 9px;
    }
    QProgressBar {
        border: 1px solid #555555;
        border-radius: 4px;
        text-align: center;
        color: #f0f0f0;
        background-color: #3c3c3c;
    }
    QProgressBar::chunk {
        background-color: #0078d7;
        border-radius: 4px;
    }
    QScrollBar:vertical {
        border: none;
        background: #2b2b2b;
        width: 12px;
        margin: 0;
    }
    QScrollBar::handle:vertical {
        background: #555555;
        min-height: 25px;
        border-radius: 6px;
    }
)";

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Apply the custom stylesheet directly. This bypasses all native theming issues.
    a.setStyleSheet(STYLESHEET);

    // Set a clean, professional font.
    QFont font("Segoe UI", 9);
    QApplication::setFont(font);

    MainWindow w;
    w.setWindowTitle("Pro Video Compressor");
    w.setMinimumSize(600, 800);
    w.show();

    return a.exec();
}
