#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QStringList>
#include <QPropertyAnimation>
#include <QEasingCurve>

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

class MainWindow : public QMainWindow
{
        Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddFilesClicked();
    void onRemoveFilesClicked();
    void onSelectOutputDirectoryClicked();
    void onCompressClicked();
    void onCancelClicked();
    void onProcessReadyReadStandardError();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onAudioSettingsChanged();
    void onEncoderChanged(int index);
    void onFFprobeFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    void setupUi();
    void setControlsEnabled(bool enabled);
    void startNextCompression();
    double getVideoDuration(const QString& filePath);
    bool checkFFmpegAvailability();
    void populateGpuEncoders();
    void updateCompressButtonState();

    // --- Core Members ---
    QProcess *ffmpegProcess;
    QStringList fileQueue;
    QString currentInputFile;
    double currentFileDuration;
    bool isCompressionActive;
    QPropertyAnimation *windowOpacityAnimation;
    QProcess *ffprobeProcess;

    // --- UI Elements ---
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
    QScrollArea* scrollArea;

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
