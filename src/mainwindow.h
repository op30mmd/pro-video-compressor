#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QStringList>

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
    MainWindow(QWidget *parent = nullptr);
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

private:
    void setupUi();
    void setControlsEnabled(bool enabled);
    void startNextCompression();
    double getVideoDuration(const QString& filePath);
    bool checkFFmpegAvailability();
    void populateGpuEncoders();

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

    QLabel* encoderLabel; // Now a member variable
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

    // --- Backend Members ---
    QProcess *ffmpegProcess;
    QStringList fileQueue;
    QString currentInputFile;
    double currentFileDuration;
    bool isCompressionActive;
};

#endif // MAINWINDOW_H
