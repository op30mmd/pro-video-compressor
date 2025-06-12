#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QSlider>
#include <QLabel>
#include <QProgressBar>
#include <QTextEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QGroupBox>
#include <QSpinBox>
#include <QRegularExpression>
#include <QStandardPaths>
#include <QScrollBar>
#include <QCheckBox>
#include <QScrollArea>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ffmpegProcess(nullptr),
    currentFileDuration(0.0),
    isCompressionActive(false)
{
    if (!checkFFmpegAvailability()) {
        QMessageBox::critical(this, "Error", "FFmpeg not found. Please make sure ffmpeg and ffprobe are installed and in your system's PATH.");
    }
    setupUi();
    populateGpuEncoders();
    ffmpegProcess = new QProcess(this);
    connect(ffmpegProcess, &QProcess::readyReadStandardError, this, &MainWindow::onProcessReadyReadStandardError);
    connect(ffmpegProcess, &QProcess::finished, this, &MainWindow::onProcessFinished);
}

MainWindow::~MainWindow()
{
    if (ffmpegProcess && ffmpegProcess->state() != QProcess::NotRunning) {
        ffmpegProcess->kill();
        ffmpegProcess->waitForFinished();
    }
}

void MainWindow::setupUi()
{
    this->setObjectName("MainWindow");
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QHBoxLayout *centeringLayout = new QHBoxLayout(centralWidget);
    centeringLayout->addStretch();

    scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setObjectName("scrollArea");

    QWidget *contentWidget = new QWidget();
    contentWidget->setObjectName("contentWidget");
    QVBoxLayout *mainLayout = new QVBoxLayout(contentWidget);
    mainLayout->setSpacing(15);

    scrollArea->setWidget(contentWidget);
    scrollArea->setMaximumWidth(800);

    QGroupBox *fileGroupBox = new QGroupBox("Input & Output");
    QVBoxLayout *fileLayout = new QVBoxLayout(fileGroupBox);
    addFilesButton = new QPushButton("Add Video Files...");
    fileLayout->addWidget(addFilesButton);
    fileListWidget = new QListWidget();
    fileListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    fileLayout->addWidget(fileListWidget);
    QHBoxLayout* outputDirLayout = new QHBoxLayout();
    selectOutputDirButton = new QPushButton("Output Directory...");
    outputDirLineEdit = new QLineEdit();
    outputDirLineEdit->setPlaceholderText("Select an output folder");
    outputDirLineEdit->setReadOnly(true);
    outputDirLayout->addWidget(selectOutputDirButton);
    outputDirLayout->addWidget(outputDirLineEdit);
    fileLayout->addLayout(outputDirLayout);

    auto createGridLayout = [](QGroupBox* box) {
        auto layout = new QGridLayout(box);
        layout->setColumnStretch(1, 1);
        layout->setColumnMinimumWidth(0, 140);
        return layout;
    };

    settingsGroupBox = new QGroupBox("Compression Settings");
    auto settingsGrid = createGridLayout(settingsGroupBox);

    encoderComboBox = new QComboBox();
    encoderComboBox->addItems({"CPU (libx264)", "CPU (libhevc)"});
    settingsGrid->addWidget(new QLabel("Encoder:"), 0, 0);
    settingsGrid->addWidget(encoderComboBox, 0, 1);

    presetComboBox = new QComboBox();
    settingsGrid->addWidget(new QLabel("Preset:"), 1, 0);
    settingsGrid->addWidget(presetComboBox, 1, 1);

    scaleComboBox = new QComboBox();
    scaleComboBox->addItems({"Original", "1920x1080 (1080p)", "1280x720 (720p)"});
    settingsGrid->addWidget(new QLabel("Resolution:"), 2, 0);
    settingsGrid->addWidget(scaleComboBox, 2, 1);

    QHBoxLayout *crfLayout = new QHBoxLayout();
    crfSlider = new QSlider(Qt::Horizontal);
    crfSlider->setRange(0, 51);
    crfSlider->setValue(23);
    crfSpinBox = new QSpinBox();
    crfSpinBox->setRange(0, 51);
    crfSpinBox->setValue(23);
    crfLayout->addWidget(crfSlider);
    crfLayout->addWidget(crfSpinBox);
    crfLabel = new QLabel("Quality (CRF):");
    settingsGrid->addWidget(crfLabel, 3, 0);
    settingsGrid->addLayout(crfLayout, 3, 1);

    advancedVideoGroupBox = new QGroupBox("Advanced Settings");
    auto advancedVideoGrid = createGridLayout(advancedVideoGroupBox);
    tuneLabel = new QLabel("Tune:");
    tuneComboBox = new QComboBox();
    tuneComboBox->addItems({"none", "film", "animation", "grain", "stillimage", "fastdecode", "zerolatency"});
    advancedVideoGrid->addWidget(tuneLabel, 0, 0);
    advancedVideoGrid->addWidget(tuneComboBox, 0, 1);
    pixelFormatComboBox = new QComboBox();
    pixelFormatComboBox->addItems({"yuv420p (Compatibility)", "yuv444p (Full Chroma)"});
    advancedVideoGrid->addWidget(new QLabel("Pixel Format:"), 1, 0);
    advancedVideoGrid->addWidget(pixelFormatComboBox, 1, 1);

    audioGroupBox = new QGroupBox("Audio Settings");
    auto audioGrid = createGridLayout(audioGroupBox);
    stripAudioCheckBox = new QCheckBox("Strip Audio (No Audio Track)");
    audioGrid->addWidget(stripAudioCheckBox, 0, 0, 1, 2);
    audioCodecComboBox = new QComboBox();
    audioCodecComboBox->addItems({"AAC (Advanced Audio Coding)", "Opus", "Copy (Keep Original)"});
    audioCodecLabel = new QLabel("Audio Codec:");
    audioGrid->addWidget(audioCodecLabel, 1, 0);
    audioGrid->addWidget(audioCodecComboBox, 1, 1);
    QHBoxLayout *audioBitrateLayout = new QHBoxLayout();
    audioBitrateSlider = new QSlider(Qt::Horizontal);
    audioBitrateSlider->setRange(64, 320);
    audioBitrateSlider->setValue(192);
    audioBitrateSpinBox = new QSpinBox();
    audioBitrateSpinBox->setRange(64, 320);
    audioBitrateSpinBox->setValue(192);
    audioBitrateLayout->addWidget(audioBitrateSlider);
    audioBitrateLayout->addWidget(audioBitrateSpinBox);
    audioBitrateLabel = new QLabel("Audio Bitrate (kbps):");
    audioGrid->addWidget(audioBitrateLabel, 2, 0);
    audioGrid->addLayout(audioBitrateLayout, 2, 1);

    outputGroupBox = new QGroupBox("Output Settings");
    auto outputGrid = createGridLayout(outputGroupBox);
    outputContainerComboBox = new QComboBox();
    outputContainerComboBox->addItems({"MP4", "MKV"});
    outputGrid->addWidget(new QLabel("Container:"), 0, 0);
    outputGrid->addWidget(outputContainerComboBox, 0, 1);
    outputSuffixLineEdit = new QLineEdit("_compressed");
    outputGrid->addWidget(new QLabel("Filename Suffix:"), 1, 0);
    outputGrid->addWidget(outputSuffixLineEdit, 1, 1);

    QHBoxLayout *controlLayout = new QHBoxLayout();
    compressButton = new QPushButton("Compress");
    compressButton->setObjectName("compressButton");
    cancelButton = new QPushButton("Cancel");
    cancelButton->setObjectName("cancelButton");
    cancelButton->setEnabled(false);
    controlLayout->addStretch();
    controlLayout->addWidget(compressButton);
    controlLayout->addWidget(cancelButton);

    progressBar = new QProgressBar();
    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    progressBar->setTextVisible(true);
    logTextEdit = new QTextEdit();
    logTextEdit->setMinimumHeight(150);
    logTextEdit->setReadOnly(true);
    logTextEdit->setFontFamily("monospace");
    statusLabel = new QLabel("Ready");

    mainLayout->addWidget(fileGroupBox);
    mainLayout->addWidget(settingsGroupBox);
    mainLayout->addWidget(advancedVideoGroupBox);
    mainLayout->addWidget(audioGroupBox);
    mainLayout->addWidget(outputGroupBox);
    mainLayout->addLayout(controlLayout);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(new QLabel("Current File Progress:"));
    mainLayout->addWidget(progressBar);
    mainLayout->addWidget(new QLabel("FFmpeg Log:"));
    mainLayout->addWidget(logTextEdit);
    mainLayout->addWidget(statusLabel);

    centeringLayout->addWidget(scrollArea);
    centeringLayout->addStretch();

    connect(addFilesButton, &QPushButton::clicked, this, &MainWindow::onAddFilesClicked);
    connect(selectOutputDirButton, &QPushButton::clicked, this, &MainWindow::onSelectOutputDirectoryClicked);
    connect(compressButton, &QPushButton::clicked, this, &MainWindow::onCompressClicked);
    connect(cancelButton, &QPushButton::clicked, this, &MainWindow::onCancelClicked);
    connect(crfSlider, &QSlider::valueChanged, crfSpinBox, &QSpinBox::setValue);
    connect(crfSpinBox, &QSpinBox::valueChanged, crfSlider, &QSlider::setValue);
    connect(stripAudioCheckBox, &QCheckBox::toggled, this, &MainWindow::onAudioSettingsChanged);
    connect(audioCodecComboBox, &QComboBox::currentIndexChanged, this, &MainWindow::onAudioSettingsChanged);
    connect(audioBitrateSlider, &QSlider::valueChanged, audioBitrateSpinBox, &QSpinBox::setValue);
    connect(audioBitrateSpinBox, &QSpinBox::valueChanged, audioBitrateSlider, &QSlider::setValue);
    connect(encoderComboBox, &QComboBox::currentIndexChanged, this, &MainWindow::onEncoderChanged);
    onAudioSettingsChanged();
    onEncoderChanged(0);
}

void MainWindow::populateGpuEncoders() {
    logTextEdit->append("Probing for available hardware encoders...");

    const QMap<QString, QString> friendlyNames = {
        {"h264_qsv", "Intel (h264_qsv)"}, {"hevc_qsv", "Intel (hevc_qsv)"},
        {"h264_nvenc", "NVIDIA (h264_nvenc)"}, {"hevc_nvenc", "NVIDIA (hevc_nvenc)"},
        {"h264_amf", "AMD (h264_amf)"}, {"hevc_amf", "AMD (hevc_amf)"}
    };

    auto testAndAddEncoder = [&](const QString& hwAccelMethod, const QString& initDevice, const QStringList& encoders) {
        QProcess probeProcess;
        // Run a silent test to see if this hardware acceleration method can be initialized
        probeProcess.start("ffmpeg", QStringList() << "-hide_banner" << "-v" << "error" << "-init_hw_device" << initDevice);

        if (probeProcess.waitForFinished(5000) && probeProcess.exitCode() == 0) {
            logTextEdit->append(QString("-> OK: Successfully initialized '%1'.").arg(initDevice));
            for (const QString& encoder : encoders) {
                QVariantMap data;
                data["encoder"] = encoder;
                data["hwaccel"] = hwAccelMethod;
                data["init_device"] = initDevice;
                encoderComboBox->addItem(friendlyNames.value(encoder, encoder), data);
            }
            return true;
        } else {
            // --- DEBUGGING ---
            // If the probe fails, print the reason to the log.
            QString errorOutput = QString::fromLocal8Bit(probeProcess.readAllStandardError());
            logTextEdit->append(QString("-> FAIL: Could not initialize '%1'. Reason:").arg(initDevice));
            logTextEdit->append(errorOutput.isEmpty() ? "Process timed out or no output." : errorOutput);
            return false;
        }
    };

    // Test for NVIDIA
    if(!testAndAddEncoder("cuda", "cuda", {"h264_nvenc", "hevc_nvenc"})) {
        logTextEdit->append("   (NVIDIA NVENC not available or driver issue)");
    }

    // Test for AMD
    if(!testAndAddEncoder("d3d11va", "d3d11va", {"h264_amf", "hevc_amf"})) {
        logTextEdit->append("   (AMD AMF not available or driver issue)");
    }

    // Test for Intel QSV (most specific first)
    if(!testAndAddEncoder("qsv", "qsv=d3d11", {"h264_qsv", "hevc_qsv"})) {
        logTextEdit->append("   (Intel QSV on D3D11 not available, trying legacy...)");
        if (!testAndAddEncoder("qsv", "qsv", {"h264_qsv", "hevc_qsv"})) {
            logTextEdit->append("   (Legacy Intel QSV also not available)");
        }
    }

    logTextEdit->append("Hardware probe finished.");
}


void MainWindow::onEncoderChanged(int index)
{
    QVariantMap data = encoderComboBox->itemData(index).toMap();
    QString encoder = data.value("encoder").toString();

    bool isCpu = encoder.isEmpty();

    crfLabel->setEnabled(isCpu);
    crfSlider->setEnabled(isCpu);
    crfSpinBox->setEnabled(isCpu);
    tuneLabel->setEnabled(isCpu);
    tuneComboBox->setEnabled(isCpu);

    presetComboBox->clear();
    if (isCpu) {
        presetComboBox->addItems({"ultrafast", "superfast", "veryfast", "faster", "fast", "medium", "slow", "slower", "veryslow"});
        presetComboBox->setCurrentText("medium");
    } else if (encoder.contains("nvenc")) {
        presetComboBox->addItems({"default", "slow", "medium", "fast", "hp", "hq", "bd", "ll", "llhq", "llhp", "lossless", "losslesshp"});
        presetComboBox->setCurrentText("slow");
    } else if (encoder.contains("amf")) {
        presetComboBox->addItems({"speed", "balanced", "quality"});
        presetComboBox->setCurrentText("balanced");
    } else if (encoder.contains("qsv")) {
        presetComboBox->addItems({"veryfast", "faster", "fast", "medium", "slow", "slower", "veryslow"});
        presetComboBox->setCurrentText("medium");
    }
}

void MainWindow::onAudioSettingsChanged()
{
    bool stripAudio = stripAudioCheckBox->isChecked();
    audioCodecLabel->setEnabled(!stripAudio);
    audioCodecComboBox->setEnabled(!stripAudio);
    bool copyAudio = (audioCodecComboBox->currentText().startsWith("Copy"));
    bool bitrateEnabled = !stripAudio && !copyAudio;
    audioBitrateLabel->setEnabled(bitrateEnabled);
    audioBitrateSlider->setEnabled(bitrateEnabled);
    audioBitrateSpinBox->setEnabled(bitrateEnabled);
}

void MainWindow::onAddFilesClicked()
{
    const QStringList files = QFileDialog::getOpenFileNames(
        this, "Select Video Files", QStandardPaths::writableLocation(QStandardPaths::MoviesLocation),
        "Video Files (*.mp4 *.mkv *.avi *.mov *.webm);;All Files (*)"
        );
    if (!files.isEmpty()) {
        fileListWidget->addItems(files);
        statusLabel->setText(QString("%1 file(s) added to the queue.").arg(files.count()));
    }
}

void MainWindow::onSelectOutputDirectoryClicked()
{
    const QString dir = QFileDialog::getExistingDirectory(
        this, "Select Output Directory", QStandardPaths::writableLocation(QStandardPaths::MoviesLocation)
        );
    if (!dir.isEmpty()) {
        outputDirLineEdit->setText(dir);
    }
}

void MainWindow::updateCrfLabel(int value) { /* Not used */ }

void MainWindow::onCompressClicked()
{
    if (fileListWidget->count() == 0) {
        QMessageBox::warning(this, "No Files", "Please add at least one video file to the list.");
        return;
    }
    if (outputDirLineEdit->text().isEmpty()) {
        QMessageBox::warning(this, "No Output Directory", "Please select an output directory.");
        return;
    }
    if(!checkFFmpegAvailability()){
        QMessageBox::critical(this, "Error", "FFmpeg not found. Cannot start compression.");
        return;
    }
    fileQueue.clear();
    for (int i = 0; i < fileListWidget->count(); ++i) {
        fileQueue.append(fileListWidget->item(i)->text());
    }
    isCompressionActive = true;
    setControlsEnabled(false);
    logTextEdit->clear();
    startNextCompression();
}

void MainWindow::onCancelClicked()
{
    if (ffmpegProcess->state() == QProcess::Running) {
        logTextEdit->append("\n\n--- CANCELLING PROCESS ---\n");
        ffmpegProcess->kill();
        isCompressionActive = false;
    }
}

void MainWindow::startNextCompression()
{
    if (fileQueue.isEmpty() || !isCompressionActive) {
        statusLabel->setText("Compression finished.");
        setControlsEnabled(true);
        progressBar->setValue(0);
        isCompressionActive = false;
        return;
    }

    currentInputFile = fileQueue.takeFirst();
    QFileInfo fileInfo(currentInputFile);
    QString suffix = outputSuffixLineEdit->text();
    if (suffix.isEmpty()) suffix = "_compressed";
    QString container = outputContainerComboBox->currentText().toLower();
    QString outputFileName = fileInfo.completeBaseName() + suffix + "." + container;
    QString outputFile = QDir(outputDirLineEdit->text()).filePath(outputFileName);

    statusLabel->setText(QString("Compressing %1 of %2: %3")
                             .arg(fileListWidget->count() - fileQueue.count())
                             .arg(fileListWidget->count())
                             .arg(fileInfo.fileName()));

    logTextEdit->append(QString("--- Starting compression for: %1 ---\n").arg(currentInputFile));
    currentFileDuration = getVideoDuration(currentInputFile);
    if (currentFileDuration <= 0) {
        logTextEdit->append("Warning: Could not determine video duration. Progress bar may not be accurate.\n");
    }
    progressBar->setValue(0);

    QStringList args;

    QVariantMap data = encoderComboBox->itemData(encoderComboBox->currentIndex()).toMap();
    QString encoder = data.value("encoder").toString();
    QString hwaccel = data.value("hwaccel").toString();
    QString initDevice = data.value("init_device").toString();

    if (!initDevice.isEmpty()) {
        args << "-init_hw_device" << initDevice;
    }
    if (!hwaccel.isEmpty()) {
        args << "-hwaccel" << hwaccel;
        if(hwaccel == "qsv") {
            args << "-hwaccel_output_format" << hwaccel;
        }
    }

    args << "-y" << "-v" << "error" << "-stats" << "-i" << currentInputFile;

    if (encoder.isEmpty()) { // CPU
        QString codecText = encoderComboBox->currentText();
        if (codecText.contains("libx264")) args << "-c:v" << "libx264";
        else args << "-c:v" << "libhevc";

        args << "-preset" << presetComboBox->currentText();
        args << "-crf" << QString::number(crfSlider->value());

        QString tune = tuneComboBox->currentText();
        if (tune != "none") args << "-tune" << tune;

    } else { // GPU
        args << "-c:v" << encoder;
        args << "-preset" << presetComboBox->currentText();
        if (encoder.contains("nvenc") || encoder.contains("amf")) {
            args << "-cq" << "23";
        } else if (encoder.contains("qsv")) {
            args << "-global_quality" << "23";
        }
    }

    if (pixelFormatComboBox->currentIndex() == 0) args << "-pix_fmt" << "yuv420p";
    else args << "-pix_fmt" << "yuv444p";

    if (stripAudioCheckBox->isChecked()) {
        args << "-an";
    } else {
        QString audioCodec = audioCodecComboBox->currentText();
        if (audioCodec.startsWith("Copy")) args << "-c:a" << "copy";
        else {
            if (audioCodec.startsWith("AAC")) args << "-c:a" << "aac";
            else if (audioCodec.startsWith("Opus")) args << "-c:a" << "libopus";
            args << "-b:a" << QString::number(audioBitrateSlider->value()) + "k";
        }
    }

    args << outputFile;

    logTextEdit->append("Executing command: ffmpeg " + args.join(" ") + "\n\n");
    ffmpegProcess->start("ffmpeg", args);
}

void MainWindow::onProcessReadyReadStandardError()
{
    const QString output = QString::fromLocal8Bit(ffmpegProcess->readAllStandardError());
    logTextEdit->append(output);
    logTextEdit->verticalScrollBar()->setValue(logTextEdit->verticalScrollBar()->maximum());
    QRegularExpression re("time=(\\d{2}):(\\d{2}):(\\d{2})\\.(\\d{2})");
    QRegularExpressionMatchIterator i = re.globalMatch(output);
    QRegularExpressionMatch match;
    while (i.hasNext()) { match = i.next(); }
    if (match.hasMatch()) {
        double hours = match.captured(1).toDouble();
        double minutes = match.captured(2).toDouble();
        double seconds = match.captured(3).toDouble();
        double hundredths = match.captured(4).toDouble();
        double currentTime = (hours * 3600) + (minutes * 60) + seconds + (hundredths / 100.0);
        if (currentFileDuration > 0) {
            int progress = static_cast<int>((currentTime / currentFileDuration) * 100);
            progressBar->setValue(qMin(progress, 100));
        }
    }
}

void MainWindow::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::CrashExit) {
        logTextEdit->append("\n--- PROCESS CRASHED ---\n");
    } else if (exitCode != 0) {
        logTextEdit->append(QString("\n--- PROCESS FAILED with exit code %1 ---\n").arg(exitCode));
    } else {
        logTextEdit->append("\n--- PROCESS FINISHED SUCCESSFULLY ---\n");
        progressBar->setValue(100);
    }
    if (isCompressionActive) {
        startNextCompression();
    } else {
        statusLabel->setText("Compression cancelled.");
        setControlsEnabled(true);
        progressBar->setValue(0);
    }
}

void MainWindow::setControlsEnabled(bool enabled)
{
    scrollArea->widget()->setEnabled(enabled);
    compressButton->setEnabled(enabled);
    cancelButton->setEnabled(!enabled);
    if (enabled) {
        onEncoderChanged(encoderComboBox->currentIndex());
        onAudioSettingsChanged();
    }
}

double MainWindow::getVideoDuration(const QString& filePath)
{
    QProcess ffprobeProcess;
    QStringList args;
    args << "-v" << "error" << "-show_entries" << "format=duration"
         << "-of" << "default=noprint_wrappers=1:nokey=1" << filePath;
    ffprobeProcess.start("ffprobe", args);
    if (!ffprobeProcess.waitForFinished(10000)) {
        logTextEdit->append("ffprobe timed out while getting duration.\n");
        return -1.0;
    }
    if (ffprobeProcess.exitCode() != 0) {
        logTextEdit->append("ffprobe failed to get duration:\n");
        logTextEdit->append(QString::fromLocal8Bit(ffprobeProcess.readAllStandardError()));
        return -1.0;
    }
    QString output = QString::fromLocal8Bit(ffprobeProcess.readAllStandardOutput()).trimmed();
    bool ok;
    double duration = output.toDouble(&ok);
    return ok ? duration : -1.0;
}

bool MainWindow::checkFFmpegAvailability() {
    bool ffmpegFound = !QStandardPaths::findExecutable("ffmpeg").isEmpty();
    bool ffprobeFound = !QStandardPaths::findExecutable("ffprobe").isEmpty();
    return ffmpegFound && ffprobeFound;
}
