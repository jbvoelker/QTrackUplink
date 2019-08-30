#ifndef FREQUENCYSOURCE_H
#define FREQUENCYSOURCE_H

#include <QObject>
#include <QTimer>

#include <hamlib/rig.h>

class RigControl : public QObject
{
    Q_OBJECT
public:
    explicit RigControl(QObject *parent = nullptr);
    ~RigControl();

    double readFrequency();
    bool readTransmitting();

    bool setFrequency(double frequency);

    void cleanup();
    bool init(int hamlibModel, const QString &serialPort);
    bool init(const QString &ipAddress, quint16 port);
    Q_INVOKABLE void startFrequencyPolling(int interval);
    Q_INVOKABLE void stopFrequencyPolling();
    Q_INVOKABLE void startPttPolling(int interval);
    Q_INVOKABLE void stopPttPolling();

    bool transmitting() const;

signals:
    void frequencyChanged(double frequency);
    void transmittingChanged(bool transmitting);

public slots:

private:
    void attemptReconnect();

    QString mPort;
    int mModel = 0;

    RIG *mRig = nullptr;
    QTimer mFrequencyPollingTimer;
    QTimer mPttPollingTimer;
    double mFrequency = 0;
    bool mTransmitting = false;
};

#endif // FREQUENCYSOURCE_H
