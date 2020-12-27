#include "rigcontrol.h"

#include "constants.h"

#include <QDebug>

RigControl::RigControl(QObject *parent)
    : QObject(parent)
    , mFrequencyPollingTimer(this)
    , mPttPollingTimer(this)
{
}

RigControl::~RigControl()
{
    qDebug() << "Destroy rig control";
    cleanup();
}

double RigControl::readFrequency()
{
    if (!mRig)
        return 0;

    freq_t currentFrequency = 0;

    int error = rig_get_freq(mRig, RIG_VFO_CURR, &currentFrequency);
    if (error != RIG_OK) {
        qDebug() << "Failed to read frequency";
        currentFrequency = 0;
        attemptReconnect();
    }

    if (!qFuzzyCompare(currentFrequency, mFrequency)) {
        mFrequency = currentFrequency;
        emit frequencyChanged(mFrequency);
        qDebug() << "Frequency changed to" << currentFrequency;
    }

    return currentFrequency;
}

bool RigControl::readTransmitting()
{
    if (!mRig)
        return 0;

    ptt_t currentPtt = RIG_PTT_OFF;

    int error = rig_get_ptt(mRig, RIG_VFO_CURR, &currentPtt);
    if (error != RIG_OK) {
        qDebug() << "Failed to read frequency";
        attemptReconnect();
    }

    const bool transmitting = currentPtt != RIG_PTT_OFF;

    if (transmitting != mTransmitting) {
        mTransmitting = transmitting;
        emit transmittingChanged(mTransmitting);
        qDebug() << "PTT changed to" << mTransmitting;
    }

    return transmitting;
}

bool RigControl::setFrequency(double frequency)
{
    if (!mRig)
        return false;

    if (qFuzzyIsNull(frequency))
        return false;

    qDebug() << "Set frequency" << frequency;

    int error = rig_set_freq(mRig, RIG_VFO_CURR, frequency);

    if (error != RIG_OK)
        attemptReconnect();

    return error == RIG_OK;
}

void RigControl::cleanup()
{
    if (mRig) {
        rig_close(mRig);
        rig_cleanup(mRig);
        mRig = nullptr;
    }

    emit frequencyChanged(0);

    stopFrequencyPolling();
}

bool RigControl::init(int hamlibModel, const QString &port)
{
    mModel = hamlibModel;
    mPort = port;

    cleanup();

    mRig = rig_init(hamlibModel);

    if (!mRig) {
        qWarning() << "Failed to create rig, unknown model";
        return false;
    }

    startFrequencyPolling(Constants::frequencyPollingInterval);

    rig_set_debug(RIG_DEBUG_ERR);

    strncpy(mRig->state.rigport.pathname, port.toLatin1().constData(), static_cast<size_t>(port.length()));

    int error = rig_open(mRig);

    if (error != RIG_OK) {
        qWarning() << "Failed to open rig:" << error;
        return false;
    }

    return true;
}

void RigControl::startFrequencyPolling(int interval)
{
    if (!mRig)
        return;

   QObject::connect(&mFrequencyPollingTimer, &QTimer::timeout, this, &RigControl::readFrequency, Qt::UniqueConnection);
   mFrequencyPollingTimer.setInterval(interval);
   mFrequencyPollingTimer.start();
}

void RigControl::stopFrequencyPolling()
{
    mFrequencyPollingTimer.stop();
}

void RigControl::startPttPolling(int interval)
{
    if (!mRig)
        return;

   QObject::connect(&mPttPollingTimer, &QTimer::timeout, this, &RigControl::readTransmitting, Qt::UniqueConnection);
   mPttPollingTimer.setInterval(interval);
   mPttPollingTimer.start();
}

void RigControl::stopPttPolling()
{
    mPttPollingTimer.stop();
}

void RigControl::attemptReconnect()
{
        qDebug() << "Attempt reconnect to" << mPort;
        init(mModel, mPort);
}

bool RigControl::transmitting() const
{
    return mTransmitting;
}
