#include "ThemeManager.h"
#include <QApplication>
#include <QFile>
#include <QStyleFactory>
#include <QPalette>
#include <QColor>

#ifdef Q_OS_WIN
#include <QSettings>
#endif

ThemeManager::ThemeManager(QObject *parent)
    : QObject(parent), m_currentTheme(System)
{
}

ThemeManager::Theme ThemeManager::currentTheme() const
{
    return m_currentTheme;
}

void ThemeManager::applyTheme(Theme theme)
{
    m_currentTheme = theme;

    if (theme == System)
    {
#ifdef Q_OS_WIN
        QStringList styles = QStyleFactory::keys();
        if (styles.contains("windows11", Qt::CaseInsensitive)) {
            QApplication::setStyle(QStyleFactory::create("windows11"));
        } else if (styles.contains("windowsvista", Qt::CaseInsensitive)) {
            QApplication::setStyle(QStyleFactory::create("windowsvista"));
        }
#else
        QApplication::setStyle(QStyleFactory::create("Fusion"));
#endif

        qApp->setStyleSheet("");
        applyPalette(isSystemDarkMode());
    }
    else
    {
        QApplication::setStyle(QStyleFactory::create("Fusion"));

        if (theme == NekoDark) {
            loadAndApplyStyleSheet(":/themes/NekoDark.qss");
        } else if (theme == ClassicLight) {
            loadAndApplyStyleSheet(":/themes/ClassicLight.qss");
        } else if (theme == MonochromeDark) { // New logic to load the theme
            loadAndApplyStyleSheet(":/themes/MonochromeDark.qss");
        }
    }
}

bool ThemeManager::isSystemDarkMode() const
{
#ifdef Q_OS_WIN
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", QSettings::NativeFormat);
    return settings.value("AppsUseLightTheme", 1).toInt() == 0;
#else
    return qApp->palette().color(QPalette::Window).lightness() < 128;
#endif
}

void ThemeManager::loadAndApplyStyleSheet(const QString& path)
{
    QFile file(path);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        qApp->setStyleSheet(file.readAll());
        file.close();
    }
}

void ThemeManager::applyPalette(bool isDark)
{
    QPalette palette;
    if (isDark) {
        palette.setColor(QPalette::Window, QColor(37, 37, 37));
        palette.setColor(QPalette::WindowText, Qt::white);
        palette.setColor(QPalette::Base, QColor(25, 25, 25));
        palette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
        palette.setColor(QPalette::ToolTipBase, Qt::white);
        palette.setColor(QPalette::ToolTipText, Qt::white);
        palette.setColor(QPalette::Text, Qt::white);
        palette.setColor(QPalette::Button, QColor(53, 53, 53));
        palette.setColor(QPalette::ButtonText, Qt::white);
        palette.setColor(QPalette::BrightText, Qt::red);
        palette.setColor(QPalette::Link, QColor(42, 130, 218));
        palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        palette.setColor(QPalette::HighlightedText, Qt::black);
    } else {
        qApp->setPalette(QPalette());
        return;
    }
    qApp->setPalette(palette);
}
