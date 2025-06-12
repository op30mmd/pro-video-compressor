#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QStringList>
#include "ThemeManager.h" // Include the new manager

// Forward declarations
class QListWidget;
class QLineEdit;
class QPushButton;
class QComboBox;
class QSlider;
class QLabel;
class QProgressBar;
class QTextEdit;
class QSpinBox;
class QGroupBox;
class QCheckBox;
class QScrollArea;
class QMenu;
class QAction;
class QActionGroup;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(ThemeManager* themeManager, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddFilesClicked();
    void onSelectOutputDirectoryClicked();
    void onCompressClicked();
    void onCancelClicked();
    void onProcessReadyReadStandardError();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void updateCrfLabel(int value);
    void onAudioSettingsChanged();
    void onEncoderChanged(int index);

    // THE FIX: The slot now correctly accepts the QAction pointer
    void onThemeChanged(QAction* action);

private:
    void setupUi();
    void setupMenu();
    void setControlsEnabled(bool enabled);
    void startNextCompression();
    double getVideoDuration(const QString& filePath);
    bool checkFFmpegAvailability();
    void populateGpuEncoders();

    // --- Core Members ---
    ThemeManager* m_themeManager;
    QProcess *ffmpegProcess;
    QStringList fileQueue;
    QString currentInputFile;
    double currentFileDuration;
    bool isCompressionActive;

    // --- Menu Elements ---
    QMenu* m_themeMenu;
    QAction* m_systemThemeAction;
    QAction* m_nekoDarkThemeAction;
    QAction* m_classicLightThemeAction;
    QAction* m_monochromeDarkThemeAction; // New menu action

    // --- UI Elements ---
    QScrollArea* scrollArea;
    QListWidget* fileListWidget;
    QLineEdit* outputDirLineEdit;
    QPushButton* addFilesButton;
    QPushButton* selectOutputDirButton;
    QPushButton* compressButton;
    QPushButton* cancelButton;
    QProgressBar* progressBar;
    QTextEdit* logTextEdit;
    QLabel* statusLabel;

    QGroupBox* settingsGroupBox;
    QGroupBox* advancedVideoGroupBox;
    QGroupBox* audioGroupBox;
    QGroupBox* outputGroupBox;

    QLabel* encoderLabel;
    QComboBox* encoderComboBox;
    QComboBox* presetComboBox;
    QComboBox* scaleComboBox;

    QLabel* crfLabel;
    QSlider* crfSlider;
    QSpinBox* crfSpinBox;

    QLabel* tuneLabel;
    QComboBox* tuneComboBox;
    QComboBox* pixelFormatComboBox;

    QCheckBox* stripAudioCheckBox;
    QLabel* audioCodecLabel;
    QComboBox* audioCodecComboBox;
    QLabel* audioBitrateLabel;
    QSlider* audioBitrateSlider;
    QSpinBox* audioBitrateSpinBox;

    QComboBox* outputContainerComboBox;
    QLineEdit* outputSuffixLineEdit;
};

#endif // MAINWINDOW_H
