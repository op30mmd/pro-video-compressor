#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
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
#include <QFrame>
#include <QScrollArea>

// Reverted to the default constructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ffmpegProcess(new QProcess(this))
{
    setupUi();
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

    QVBoxLayout *mainVLayout = new QVBoxLayout(centralWidget);
    mainVLayout->setContentsMargins(10, 10, 10, 10); // Add some margins

    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    QWidget *contentWidget = new QWidget();
    scrollArea->setWidget(contentWidget);

    QVBoxLayout *contentVLayout = new QVBoxLayout(contentWidget);
    contentVLayout->setSpacing(15);


    QGroupBox *fileGroupBox = new QGroupBox("Input & Output");
    QFormLayout *fileLayout = new QFormLayout(fileGroupBox);
    fileLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    addFilesButton = new QPushButton(QIcon::fromTheme("document-open"), " Add Video Files...");
    QPushButton *removeFilesButton = new QPushButton(QIcon::fromTheme("edit-delete"), " Remove Selected");
    QHBoxLayout *addRemoveLayout = new QHBoxLayout();
    addRemoveLayout->addWidget(addFilesButton);
    addRemoveLayout->addWidget(removeFilesButton);
    fileLayout->addRow(addRemoveLayout);
    fileListWidget = new QListWidget();
    fileListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    fileLayout->addRow(fileListWidget);
    selectOutputDirButton = new QPushButton(QIcon::fromTheme("folder-open"), " Output Directory...");
    outputDirLineEdit = new QLineEdit();
    outputDirLineEdit->setPlaceholderText("Select an output folder");
    outputDirLineEdit->setReadOnly(true);
    fileLayout->addRow(selectOutputDirButton, outputDirLineEdit);

    

    settingsGroupBox = new QGroupBox("Compression Settings");
    QFormLayout *settingsLayout = new QFormLayout(settingsGroupBox);
    settingsLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    encoderLabel = new QLabel("Encoder:");
    encoderComboBox = new QComboBox();
    encoderComboBox->addItems({"CPU (libx264)", "CPU (libhevc)"});
    settingsLayout->addRow(encoderLabel, encoderComboBox);

    presetComboBox = new QComboBox();
    settingsLayout->addRow(new QLabel("Preset:"), presetComboBox);

    scaleComboBox = new QComboBox();
    scaleComboBox->addItems({"Original", "1920x1080 (1080p)", "1280x720 (720p)"});
    settingsLayout->addRow(new QLabel("Resolution:"), scaleComboBox);

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
    settingsLayout->addRow(crfLabel, crfLayout);

    advancedVideoGroupBox = new QGroupBox("Advanced Settings");
    QFormLayout *advancedVideoLayout = new QFormLayout(advancedVideoGroupBox);
    advancedVideoLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    tuneLabel = new QLabel("Tune:");
    tuneComboBox = new QComboBox();
    tuneComboBox->addItems({"none", "film", "animation", "grain", "stillimage", "fastdecode", "zerolatency"});
    advancedVideoLayout->addRow(tuneLabel, tuneComboBox);
    pixelFormatComboBox = new QComboBox();
    pixelFormatComboBox->addItems({"yuv420p (Compatibility)", "yuv444p (Full Chroma)"});
    advancedVideoLayout->addRow(new QLabel("Pixel Format:"), pixelFormatComboBox);

    audioGroupBox = new QGroupBox("Audio Settings");
    QFormLayout *audioLayout = new QFormLayout(audioGroupBox);
    audioLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    audioLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    stripAudioCheckBox = new QCheckBox("Strip Audio (No Audio Track)");
    audioLayout->addRow(stripAudioCheckBox);
    audioCodecComboBox = new QComboBox();
    audioCodecComboBox->addItems({"AAC (Advanced Audio Coding)", "Opus", "Copy (Keep Original)"});
    audioCodecLabel = new QLabel("Audio Codec:");
    audioLayout->addRow(audioCodecLabel, audioCodecComboBox);
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
    audioLayout->addRow(audioBitrateLabel, audioBitrateLayout);

    outputGroupBox = new QGroupBox("Output Settings");
    QFormLayout *outputLayout = new QFormLayout(outputGroupBox);
    outputLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    outputContainerComboBox = new QComboBox();
    outputContainerComboBox->addItems({"MP4", "MKV"});
    outputLayout->addRow(new QLabel("Container:"), outputContainerComboBox);
    outputSuffixLineEdit = new QLineEdit("_compressed");
    outputLayout->addRow(new QLabel("Filename Suffix:"), outputSuffixLineEdit);

    QHBoxLayout *controlLayout = new QHBoxLayout();
    compressButton = new QPushButton("Compress");
    compressButton->setObjectName("compressButton");
    compressButton->setEnabled(false);
    cancelButton = new QPushButton("Cancel");
    cancelButton->setObjectName("cancelButton");
    cancelButton->setEnabled(false);
    controlLayout->addStretch();
    controlLayout->addWidget(compressButton);
    controlLayout->addWidget(cancelButton);

    progressBar = new QProgressBar();
    logTextEdit = new QTextEdit();
    logTextEdit->setMinimumHeight(150);
    logTextEdit->setReadOnly(true);
    logTextEdit->setFontFamily("monospace");
    statusLabel = new QLabel("Ready");

    contentVLayout->addWidget(fileGroupBox);
    contentVLayout->addWidget(settingsGroupBox);
    contentVLayout->addWidget(advancedVideoGroupBox);
    contentVLayout->addWidget(audioGroupBox);
    contentVLayout->addWidget(outputGroupBox);
    contentVLayout->addLayout(controlLayout);
    contentVLayout->addSpacing(10);
    contentVLayout->addWidget(new QLabel("Current File Progress:"));
    contentVLayout->addWidget(progressBar);
    contentVLayout->addWidget(new QLabel("FFmpeg Log:"));
    contentVLayout->addWidget(logTextEdit, 1);
    contentVLayout->addWidget(statusLabel);

    

    connect(addFilesButton, &QPushButton::clicked, this, &MainWindow::onAddFilesClicked);
    connect(removeFilesButton, &QPushButton::clicked, this, &MainWindow::onRemoveFilesClicked);
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
    int gpusFound = 0;

    const QMap<QString, QString> friendlyNames = {
        {"h264_qsv", "Intel (h264_qsv)"}, {"hevc_qsv", "Intel (hevc_qsv)"},
        {"h264_nvenc", "NVIDIA (h264_nvenc)"}, {"hevc_nvenc", "NVIDIA (hevc_nvenc)"},
        {"h264_amf", "AMD (h264_amf)"}, {"hevc_amf", "AMD (hevc_amf)"}
    };

    auto testAndAddEncoder = [&](const QString& hwAccelMethod, const QString& initDevice, const QStringList& encoders) {
        QProcess probeProcess;
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
            gpusFound++;
            return true;
        } else {
            QString errorOutput = QString::fromLocal8Bit(probeProcess.readAllStandardError());
            logTextEdit->append(QString("-> FAIL: Could not initialize '%1'. Reason:").arg(initDevice));
            logTextEdit->append(errorOutput.isEmpty() ? "Process timed out or no output." : errorOutput);
            return false;
        }
    };

    if(!testAndAddEncoder("cuda", "cuda", {"h264_nvenc", "hevc_nvenc"})) {
        logTextEdit->append("   (NVIDIA NVENC not available or driver issue)");
    }

    if(!testAndAddEncoder("d3d11va", "d3d11va", {"h264_amf", "hevc_amf"})) {
        logTextEdit->append("   (AMD AMF not available, trying D3D11VA...)");
        if(!testAndAddEncoder("amf", "amf", {"h264_amf", "hevc_amf"})) {
            logTextEdit->append("   (AMD AMF/D3D11VA also not available or driver issue)");
        }
    }

    if(!testAndAddEncoder("qsv", "qsv=d3d11", {"h264_qsv", "hevc_qsv"})) {
        logTextEdit->append("   (Intel QSV on D3D11 not available, trying legacy...)");
        if (!testAndAddEncoder("qsv", "qsv", {"h264_qsv", "hevc_qsv"})) {
            logTextEdit->append("   (Legacy Intel QSV also not available)");
        }
    }

    logTextEdit->append("Hardware probe finished.");

    if (encoderComboBox->count() <= 2) {
        logTextEdit->append("No working hardware encoders detected. GPU options will be hidden.");
        encoderLabel->hide();
        encoderComboBox->hide();
    }
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

void MainWindow::updateCompressButtonState()
{
    bool filesSelected = fileListWidget->count() > 0;
    bool outputDirSelected = !outputDirLineEdit->text().isEmpty();
    compressButton->setEnabled(filesSelected && outputDirSelected);
}

void MainWindow::onAddFilesClicked()
{
    QStringList files = QFileDialog::getOpenFileNames(
        this,
        tr("Select Videos to Compress"),
        QDir::homePath(),
        tr("Video Files (*.mp4 *.mkv *.avi *.mov *.wmv *.flv)")
    );

    if (!files.isEmpty()) {
        fileListWidget->addItems(files);
        updateCompressButtonState();
    }
}

void MainWindow::onRemoveFilesClicked()
{
    QList<QListWidgetItem*> selectedItems = fileListWidget->selectedItems();
    foreach (QListWidgetItem* item, selectedItems) {
        delete fileListWidget->takeItem(fileListWidget->row(item));
    }
    updateCompressButtonState();
}

void MainWindow::onSelectOutputDirectoryClicked()
{
    const QString dir = QFileDialog::getExistingDirectory(
        this, "Select Output Directory", QStandardPaths::writableLocation(QStandardPaths::MoviesLocation)
        );
    if (!dir.isEmpty()) {
        outputDirLineEdit->setText(dir);
        updateCompressButtonState();
    }
}

void MainWindow::onCompressClicked()
{
    if(!checkFFmpegAvailability()){
        statusLabel->setText("Error: FFmpeg not found.");
        logTextEdit->append("Error: FFmpeg or ffprobe not found in system PATH. Please install FFmpeg.");
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
        args << "-crf" << QString::number(crfSpinBox->value());

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
            args << "-b:a" << QString::number(audioBitrateSpinBox->value()) + "k";
        }
    }

    args << outputFile;

    logTextEdit->append("Executing command: ffmpeg " + args.join(" ") + "\n\n");
    ffmpegProcess->start("ffmpeg", args);
}

void MainWindow::onProcessReadyReadStandardError()
{
    const QString output = QString::fromLocal8Bit((*ffmpegProcess).readAllStandardError());
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
    // Recursively find all child widgets and disable them, with specific exceptions.
    QList<QWidget*> widgets = this->findChildren<QWidget *>();
    foreach(QWidget *widget, widgets) {
        // A list of widgets to NEVER disable.
        if (widget == logTextEdit ||
            widget == statusLabel ||
            widget == progressBar ||
            widget == logTextEdit->verticalScrollBar() ||
            widget == centralWidget() ||
            qobject_cast<QFrame*>(widget) != nullptr)
        {
            continue;
        }
        // A list of widgets that are ALWAYS re-enabled and have their state
        // re-evaluated by other functions.
        if (widget == compressButton ||
            widget == cancelButton)
        {
            continue;
        }

        widget->setEnabled(enabled);
    }

    // Explicitly manage the state of the primary action buttons
    compressButton->setEnabled(enabled);
    cancelButton->setEnabled(!enabled);

    // If we are re-enabling controls, we must restore the dynamic states
    // based on current settings.
    if (enabled) {
        onEncoderChanged(encoderComboBox->currentIndex());
        onAudioSettingsChanged();
        updateCompressButtonState(); // Re-evaluate button state after enabling controls
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
        logTextEdit->append("Error: ffprobe timed out while getting duration for " + filePath + ".\n");
        return -1.0;
    }
    if (ffprobeProcess.exitCode() != 0) {
        logTextEdit->append("Error: ffprobe failed to get duration for " + filePath + ":\n");
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
