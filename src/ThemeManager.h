#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QObject>
#include <QString>

class ThemeManager : public QObject
{
    Q_OBJECT

public:
    enum Theme {
        System,
        NekoDark,
        ClassicLight,
        MonochromeDark // New theme added
    };

    explicit ThemeManager(QObject *parent = nullptr);

    void applyTheme(Theme theme);
    Theme currentTheme() const;

private:
    bool isSystemDarkMode() const;
    void loadAndApplyStyleSheet(const QString& path);
    void applyPalette(bool isDark);

    Theme m_currentTheme;
};

#endif // THEMEMANAGER_H
