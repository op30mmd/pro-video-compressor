#include "MainWindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QFont>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

// --- Definitive Native Style Approach ---
// This code explicitly asks Qt to use the best native style for the OS.
// On Windows, it will attempt to use the "windows11" or "windowsvista"
// style, which correctly handles dark mode and all control drawing.
#ifdef Q_OS_WIN
    QStringList styles = QStyleFactory::keys();
    if (styles.contains("windows11", Qt::CaseInsensitive)) {
        a.setStyle(QStyleFactory::create("windows11"));
    } else if (styles.contains("windowsvista", Qt::CaseInsensitive)) {
        a.setStyle(QStyleFactory::create("windowsvista"));
    }
#endif
    // On other platforms (Linux, macOS), Qt will automatically use their native styles.


    // --- Set Custom Font ---
    // This will apply "Samsung Sans" to the entire application at a specific size.
    // Note: The font must be installed on the system for this to work.
    QFont font("Samsung Sans", 10);
    QApplication::setFont(font);


    MainWindow w;
    w.setWindowTitle("Pro Video Compressor");
    w.setMinimumSize(600, 800);
    w.show();

    return a.exec();
}
