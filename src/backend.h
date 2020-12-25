#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QSettings>
#include <QString>
#include <QThread>

#include "rigcontrol.h"

class Backend : public QObject
{
    Q_OBJECT
public:
    explicit Backend(QObject *parent = nullptr);
    ~Backend();

    double rxFrequency() const;
    double txFrequency() const;

    Q_PROPERTY(double rxFrequency READ rxFrequency NOTIFY rxFrequencyChanged)
    Q_PROPERTY(double txFrequency READ txFrequency NOTIFY txFrequencyChanged)
    Q_PROPERTY(double rxFrequencySetpoint READ rxFrequencySetpoint NOTIFY rxFrequencySetpointChanged)
    Q_PROPERTY(double offset READ offset WRITE setOffset NOTIFY offsetChanged)
    Q_PROPERTY(double txStartFrequency READ txStartFrequency WRITE setTxStartFrequency NOTIFY txStartFrequencyChanged)
    Q_PROPERTY(double rxStartFrequency READ rxStartFrequency WRITE setRxStartFrequency NOTIFY rxStartFrequencyChanged)
    Q_PROPERTY(int txModel READ txModel WRITE setTxModel NOTIFY txModelChanged)
    Q_PROPERTY(QString txPort READ txPort WRITE setTxPort  NOTIFY txPortChanged)
    Q_PROPERTY(QString rxPort READ rxPort WRITE setRxPort NOTIFY rxPortChanged)
    Q_PROPERTY(int rxModel READ rxModel WRITE setRxModel NOTIFY rxModelChanged)
    Q_PROPERTY(bool rxAudioMuteEnabled READ rxAudioMuteEnabled WRITE setRxAudioMuteEnabled NOTIFY rxAudioMuteEnabledChanged)
    Q_PROPERTY(QString onPttCommand READ onPttCommand WRITE setOnPttCommand NOTIFY onPttCommandChanged)
    Q_PROPERTY(QString onReturnToRxCommand READ onReturnToRxCommand WRITE setOnReturnToRxCommand NOTIFY onReturnToRxCommandChanged)
    Q_PROPERTY(bool trackingEnabled READ trackingEnabled WRITE setTrackingEnabled NOTIFY trackingEnabledChanged)

    Q_INVOKABLE bool init();

    double offset() const;
    void setOffset(double offset);

    double txStartFrequency() const;
    void setTxStartFrequency(double txStartFrequency);

    double rxStartFrequency() const;
    void setRxStartFrequency(double rxStartFrequency);

    int txModel() const;
    void setTxModel(int txModel);

    QString txPort() const;
    void setTxPort(const QString &txPort);

    QString rxPort() const;
    void setRxPort(const QString &rxPort);

    int rxModel() const;
    void setRxModel(const int rxModel);

    double rxFrequencySetpoint() const;
    void setRxFrequencySetpoint(double rxFrequencySetpoint);

    void setRxAudioMute(bool mute);

    bool rxAudioMuteEnabled() const;
    void setRxAudioMuteEnabled(bool rxAudioMuteEnabled);

    QString onPttCommand() const;
    void setOnPttCommand(const QString &onPttCommand);

    QString onReturnToRxCommand() const;
    void setOnReturnToRxCommand(const QString &onReturnToRxCommand);

    bool trackingEnabled() const;
    void setTrackingEnabled(bool trackingEnabled);

signals:
    void rxFrequencyChanged(double frequency);
    void txFrequencyChanged(double frequency);
    void rxFrequencySetpointChanged(double frequency);
    void offsetChanged(double offset);
    void txStartFrequencyChanged(double frequency);
    void rxStartFrequencyChanged(double frequency);
    void txModelChanged(int model);
    void txPortChanged(QString port);
    void rxPortChanged(QString port);
    void rxModelChanged(int model);
    void rxAudioMuteEnabledChanged(bool mute);
    void onPttCommandChanged(QString command);
    void onReturnToRxCommandChanged(QString command);
    void trackingEnabledChanged(bool enabled);

public slots:
    void handleTxFrequencyChange(double frequency);
    void updateReceiverFrequency();

    void incrementOffset(double increment);

private:
    void setRxFrequency(double rxFrequency);
    void setTxFrequency(double txFrequency);

    void cleanup();

    int mTxModel = 0;
    QString mTxPort;
    QString mRxPort;
    int mRxModel = 0;

    double mRxFrequency = 0;
    double mTxFrequency = 0;
    double mRxFrequencySetpoint;
    double mOffset = 0;

    double mTxStartFrequency = 432050000;
    double mRxStartFrequency = 10489550000;

    bool mRxAudioMuteEnabled;
    QString mOnPttCommand;
    QString mOnReturnToRxCommand;

    bool mTrackingEnabled = false;

    RigControl *mReceiver;
    RigControl *mTransmitter;

    QThread *mRigThread = nullptr;

    QSettings mSettings;
};

#endif // BACKEND_H
