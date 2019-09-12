#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>

namespace Constants {
    const QString offsetKey = QLatin1String("offset");
    const QString rxStartFrequencyKey = QLatin1String("rxStartFrequency");
    const QString txStartFrequencyKey = QLatin1String("txStartFrequency");
    const QString txModelKey = QLatin1String("txModel");
    const QString txSerialPortKey = QLatin1String("txSerialPort");
    const QString rxPortKey = QLatin1String("rxPort");
    const QString rxModelKey = QLatin1String("rxModel");
    const QString rxAudioMuteEnabledKey = QLatin1String("rxAudioMuteEnabled");
    const QString onPttCommandKey = QLatin1String("onPttCommand");
    const QString onReturnToRxCommandKey = QLatin1String("onReturnToRxCommandKey");
    const QString trackingEnabledKey = QLatin1String("trackingEnabled");

    const double defaultOffset = 0.0;
    const double defaultRxStartFrequency = 10489550000.0;
    const double defaultTxStartFrequency = 432050000.0;
    const int defaulTxModel = 120; // FT-817/FT-818
    const QString defaultTxPort = QLatin1String("/dev/ttyUSB0");
    const int defaultRxModel = 2; // NET rigctld
    const QString defaultRxPort = QLatin1String("localhost:7356");
    const bool defaultRxAudioMuteEnabled = false;
    const int pttPollingInterval = 250;
    const int frequencyPollingInterval = 200;

    const QString defaultOnPTTCommand = QLatin1String("pactl set-sink-mute 0 1");
    const QString defaultOnReturnToRxCommand = QLatin1String("pactl set-sink-mute 0 0");

    const bool defaultTrackingEnabled = true;
}

#endif // CONSTANTS_H
