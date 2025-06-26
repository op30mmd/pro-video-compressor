
#ifndef FFMPEG_MANAGER_H
#define FFMPEG_MANAGER_H

#include <QObject>
#include <QStringList>
#include <QProcess>
#include "compression_settings.h"

class FFmpegManager : public QObject
{
    Q_OBJECT

public:
    explicit FFmpegManager(QObject *parent = nullptr);
    ~FFmpegManager();

    void startCompression(const QString& inputFile, const QString& outputDir, const CompressionSettings& settings);
    void cancel();
    static bool checkFFmpegAvailability();
    static double getVideoDuration(const QString& filePath);

signals:
    void progress(int percentage);
    void logMessage(const QString& message);
    void finished(int exitCode, QProcess::ExitStatus exitStatus);
    void error(const QString& errorMessage);

private slots:
    void onProcessReadyReadStandardError();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    QStringList buildArguments(const QString& inputFile, const QString& outputFile, const CompressionSettings& settings);

    QProcess *process;
    double currentFileDuration;
};

#endif // FFMPEG_MANAGER_H
