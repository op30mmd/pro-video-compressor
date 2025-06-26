
#ifndef COMPRESSION_SETTINGS_H
#define COMPRESSION_SETTINGS_H

#include <QString>
#include <QVariantMap>

struct CompressionSettings {
    // Video Settings
    QVariantMap encoderData;
    QString preset;
    QString resolution;
    int crfValue;
    QString tune;
    QString pixelFormat;

    // Audio Settings
    bool stripAudio;
    QString audioCodec;
    int audioBitrate;

    // Output Settings
    QString container;
    QString suffix;
};

#endif // COMPRESSION_SETTINGS_H
