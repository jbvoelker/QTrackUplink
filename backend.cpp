#include "backend.h"
#include "constants.h"

#include <QProcess>
#include <QSettings>

#include <QDebug>

Backend::Backend(QObject *parent)
    : QObject(parent)
    , mRigThread(new QThread(this))
{
    QObject::connect(this, &Backend::offsetChanged, this, &Backend::updateReceiverFrequency);
    QObject::connect(this, &Backend::txStartFrequencyChanged, this, &Backend::updateReceiverFrequency);
    QObject::connect(this, &Backend::rxStartFrequencyChanged, this, &Backend::updateReceiverFrequency);
    QObject::connect(this, &Backend::trackingEnabledChanged, this, &Backend::updateReceiverFrequency);

    mOffset = mSettings.value(Constants::offsetKey, Constants::defaultOffset).toDouble();
    mRxStartFrequency = mSettings.value(Constants::rxStartFrequencyKey, Constants::defaultRxStartFrequency).toDouble();
    mTxStartFrequency = mSettings.value(Constants::txStartFrequencyKey, Constants::defaultTxStartFrequency).toDouble();

    mTxModel = mSettings.value(Constants::txModelKey, Constants::defaulTxModel).toInt();
    mTxPort = mSettings.value(Constants::txSerialPortKey, Constants::defaultTxPort).toString();
    mRxPort = mSettings.value(Constants::rxPortKey, Constants::defaultRxPort).toString();
    mRxModel = mSettings.value(Constants::rxModelKey, Constants::defaultRxModel).toInt();

    mOnPttCommand = mSettings.value(Constants::onPttCommandKey, Constants::defaultOnPTTCommand).toString();
    mOnReturnToRxCommand = mSettings.value(Constants::onReturnToRxCommandKey, Constants::defaultOnReturnToRxCommand).toString();
    mRxAudioMuteEnabled = mSettings.value(Constants::rxAudioMuteEnabledKey, Constants::defaultRxAudioMuteEnabled).toBool();

    mTrackingEnabled = mSettings.value(Constants::trackingEnabledKey, Constants::defaultTrackingEnabled).toBool();
}

Backend::~Backend()
{
    cleanup();
}

double Backend::rxFrequency() const
{
    return mRxFrequency;
}

void Backend::setRxFrequency(double rxFrequency)
{
    if (rxFrequency != mRxFrequency) {
        mRxFrequency = rxFrequency;
        emit rxFrequencyChanged(mRxFrequency);
    }
}

double Backend::txFrequency() const
{
    return mTxFrequency;
}

void Backend::incrementOffset(double increment)
{
    setOffset(mOffset + increment);
}

bool Backend::init()
{
    cleanup();

    QSettings settings;

    mTransmitter = new RigControl();
    bool success = mTransmitter->init(mTxModel, mTxPort);

    mReceiver = new RigControl();
    success |= mReceiver->init(mRxModel, mRxPort);

    connect(mRigThread, &QThread::finished, mReceiver, &RigControl::deleteLater);
    connect(mRigThread, &QThread::finished, mTransmitter, &RigControl::deleteLater);

    if (mRxAudioMuteEnabled)
        mTransmitter->startPttPolling(Constants::pttPollingInterval);

    mTransmitter->moveToThread(mRigThread);
    mReceiver->moveToThread(mRigThread);

    mRigThread->start();

    QObject::connect(mTransmitter, &RigControl::frequencyChanged, this, &Backend::handleTxFrequencyChange);
    QObject::connect(mReceiver, &RigControl::frequencyChanged, this, &Backend::setRxFrequency);
    QObject::connect(this, &Backend::rxFrequencySetpointChanged, mReceiver, &RigControl::setFrequency);

    QObject::connect(mTransmitter, &RigControl::transmittingChanged, this, &Backend::setRxAudioMute);

    return success;
}

void Backend::handleTxFrequencyChange(double frequency)
{
    setTxFrequency(frequency);
    if (mTrackingEnabled)
        updateReceiverFrequency();
}

void Backend::updateReceiverFrequency()
{
    if (!qFuzzyIsNull(mTxFrequency)) {
        const double offsetFromTransponderStart = mTxFrequency - mTxStartFrequency;
        setRxFrequencySetpoint(offsetFromTransponderStart + mRxStartFrequency + mOffset);
    } else {
        setRxFrequencySetpoint(0);
    }
}

void Backend::setTxFrequency(double txFrequency)
{
    if (txFrequency != mTxFrequency) {
        mTxFrequency = txFrequency;
        emit txFrequencyChanged(mTxFrequency);
    }
}

double Backend::offset() const
{
    return mOffset;
}

void Backend::setOffset(double offset)
{
    if (offset != mOffset) {
        mOffset = offset;
        mSettings.setValue(Constants::offsetKey, offset);
        emit offsetChanged(mOffset);
    }
}

void Backend::cleanup()
{
    if (mRigThread->isRunning()) {
        mRigThread->quit();
        mRigThread->wait();
    }
}

bool Backend::trackingEnabled() const
{
    return mTrackingEnabled;
}

void Backend::setTrackingEnabled(bool trackingEnabled)
{
    qDebug() << "Set tracking enabled to" << trackingEnabled;
    if (trackingEnabled != mTrackingEnabled) {
        mTrackingEnabled = trackingEnabled;
        mSettings.setValue(Constants::trackingEnabledKey, trackingEnabled);
        emit trackingEnabledChanged(mTrackingEnabled);
    }
}

QString Backend::onReturnToRxCommand() const
{
    return mOnReturnToRxCommand;
}

void Backend::setOnReturnToRxCommand(const QString &onReturnToRxCommand)
{
    if (onReturnToRxCommand != mOnReturnToRxCommand) {
        mOnReturnToRxCommand = onReturnToRxCommand;
        mSettings.setValue(Constants::onReturnToRxCommandKey, onReturnToRxCommand);
        emit onReturnToRxCommandChanged(mOnReturnToRxCommand);
    }
}

QString Backend::onPttCommand() const
{
    return mOnPttCommand;
}

void Backend::setOnPttCommand(const QString &onPttCommand)
{

    if (onPttCommand != mOnPttCommand) {
        mOnPttCommand = onPttCommand;
        mSettings.setValue(Constants::onPttCommandKey, onPttCommand);
        emit onPttCommandChanged(mOnPttCommand);
    }
}

bool Backend::rxAudioMuteEnabled() const
{
    return mRxAudioMuteEnabled;
}

void Backend::setRxAudioMuteEnabled(bool rxAudioMuteEnabled)
{
    if (rxAudioMuteEnabled != mRxAudioMuteEnabled) {
        mRxAudioMuteEnabled = rxAudioMuteEnabled;
        mSettings.setValue(Constants::rxAudioMuteEnabledKey, rxAudioMuteEnabled);
        emit rxAudioMuteEnabledChanged(rxAudioMuteEnabled);

        if (rxAudioMuteEnabled)
            QMetaObject::invokeMethod(mTransmitter, "startPttPolling", Qt::QueuedConnection, Q_ARG(int, Constants::pttPollingInterval));
        else
            QMetaObject::invokeMethod(mTransmitter, "stopPttPolling", Qt::QueuedConnection);
    }
}

double Backend::rxFrequencySetpoint() const
{
    return mRxFrequencySetpoint;
}

void Backend::setRxFrequencySetpoint(double txFrequencySetpoint)
{
    if (txFrequencySetpoint != mRxFrequencySetpoint) {
        mRxFrequencySetpoint = txFrequencySetpoint;
        emit rxFrequencySetpointChanged(mRxFrequencySetpoint);
    }
}

void Backend::setRxAudioMute(bool mute)
{
    if (!mRxAudioMuteEnabled)
        return;

    if (mute)
        QProcess::execute("pactl set-sink-mute 1 1");
    else
        QProcess::execute("pactl set-sink-mute 1 0");
}

int Backend::rxModel() const
{
    return mRxModel;
}

void Backend::setRxModel(const int rxModel)
{
    if (rxModel != mRxModel) {
        mRxModel = rxModel;
        mSettings.setValue(Constants::rxModelKey, rxModel);
        emit rxModelChanged(mRxModel);
    }
}

QString Backend::rxPort() const
{
    return mRxPort;
}

void Backend::setRxPort(const QString &rxPort)
{
    if (rxPort != mRxPort) {
        mRxPort = rxPort;
        mSettings.setValue(Constants::rxPortKey, rxPort);
        emit rxPortChanged(mRxPort);
    }
}

QString Backend::txPort() const
{
    return mTxPort;
}

void Backend::setTxPort(const QString &txPort)
{
    if (txPort != mTxPort) {
        mTxPort = txPort;
        mSettings.setValue(Constants::txSerialPortKey, mTxPort);
        emit txPortChanged(mTxPort);
    }
}

int Backend::txModel() const
{
    return mTxModel;
}

void Backend::setTxModel(int txModel)
{
    if (txModel != mTxModel) {
        mTxModel = txModel;
        mSettings.setValue(Constants::txModelKey, txModel);
        emit txModelChanged(mTxModel);
    }
}

double Backend::rxStartFrequency() const
{
    return mRxStartFrequency;
}

void Backend::setRxStartFrequency(double rxStartFrequency)
{
    if (rxStartFrequency != mRxStartFrequency) {
        mRxStartFrequency = rxStartFrequency;
        mSettings.setValue(Constants::rxStartFrequencyKey, rxStartFrequency);
        emit rxStartFrequencyChanged(mRxStartFrequency);
    }
}

double Backend::txStartFrequency() const
{
    return mTxStartFrequency;
}

void Backend::setTxStartFrequency(double txStartFrequency)
{
    if (txStartFrequency != mTxStartFrequency) {
        mTxStartFrequency = txStartFrequency;
        mSettings.setValue(Constants::txStartFrequencyKey, txStartFrequency);
        emit txStartFrequencyChanged(mTxStartFrequency);
    }
}
