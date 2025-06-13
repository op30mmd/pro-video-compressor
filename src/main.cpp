#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QFile>
#include <QFont>

// A professional, self-contained stylesheet that is guaranteed to work.
const QString STYLESHEET = R"(
    * {
        background-color: #282c34;
        color: #abb2bf;
        border: none;
        font-family: "Segoe UI";
        font-size: 9pt;
    }
    QScrollArea {
        background-color: transparent;
    }
    QGroupBox {
        background-color: #21252b;
        border: 1px solid #181a1f;
        border-radius: 8px;
        margin-top: 1em;
        font-weight: bold;
        padding: 15px;
    }
    QGroupBox::title {
        subcontrol-origin: margin;
        subcontrol-position: top left;
        padding: 0 10px;
        left: 10px;
        color: #9da5b4;
    }
    QLabel {
        font-weight: normal;
    }
    QPushButton {
        background-color: #3a3f4b;
        color: #dbe0e8;
        padding: 8px 16px;
        border-radius: 4px;
        font-weight: bold;
    }
    QPushButton:hover {
        background-color: #4b5160;
    }
    QPushButton:pressed {
        background-color: #323640;
    }
    QPushButton:disabled {
        background-color: #21252b;
        color: #5c6370;
    }
    #compressButton {
        background-color: #61afef;
        color: #21252b;
    }
    #cancelButton {
        background-color: #e06c75;
        color: #21252b;
    }
    QLineEdit, QTextEdit, QSpinBox, QComboBox {
        background-color: #21252b;
        border-radius: 4px;
        padding: 6px;
        border: 1px solid #181a1f;
    }
    QLineEdit:focus, QTextEdit:focus, QSpinBox:focus, QComboBox:focus {
        border: 1px solid #61afef;
    }
    QListWidget {
        background-color: #21252b;
        border: 1px solid #181a1f;
        border-radius: 4px;
    }
    QComboBox QAbstractItemView {
        background-color: #21252b;
        border: 1px solid #61afef;
        selection-background-color: #3a3f4b;
    }
    QComboBox::drop-down {
        subcontrol-origin: padding;
        subcontrol-position: top right;
        width: 25px;
        border-left: none;
    }
    QComboBox::down-arrow {
        image: url(:/icons/down-arrow.svg);
    }
    QSpinBox::up-button, QSpinBox::down-button {
        subcontrol-origin: border;
        width: 20px;
        border-left: 1px solid #181a1f;
        background-color: #21252b;
    }
    QSpinBox::up-arrow {
        image: url(:/icons/up-arrow.svg);
        width: 10px;
        height: 10px;
    }
    QSpinBox::down-arrow {
        image: url(:/icons/down-arrow.svg);
        width: 10px;
        height: 10px;
    }
    QSlider::groove:horizontal {
        background: #3a3f4b;
        height: 8px;
        border-radius: 4px;
    }
    QSlider::handle:horizontal {
        background: #61afef;
        width: 18px;
        height: 18px;
        margin: -5px 0;
        border-radius: 9px;
    }
    QProgressBar {
        text-align: center;
        color: #ffffff;
        background-color: #3a3f4b;
        border-radius: 4px;
        font-weight: bold;
    }
    QProgressBar::chunk {
        background-color: #98c379;
        border-radius: 4px;
    }
    QCheckBox::indicator {
        width: 16px;
        height: 16px;
        background-color: #21252b;
        border: 1px solid #181a1f;
        border-radius: 4px;
    }
    QCheckBox::indicator:checked {
        background-color: #98c379;
    }
)";

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Set the Fusion style as a stable base for our custom stylesheet
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    // Apply our definitive custom stylesheet
    a.setStyleSheet(STYLESHEET);

    QFont font("Segoe UI", 9);
    QApplication::setFont(font);

    MainWindow w;
    w.setWindowTitle("Pro Video Compressor");
    w.setMinimumSize(600, 800);
    w.show();

    return a.exec();
}
