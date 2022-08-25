#include "ofonomanager.h"

Q_LOGGING_CATEGORY(OFONO2, "telephony [OfonoManager]")

OfonoManager::OfonoManager(QObject *parent) : QObject(parent),
      m_dbusConnection(QDBusConnection::connectToBus(QDBusConnection::SystemBus, "hud_ofono")),
      m_manager("org.ofono","/", m_dbusConnection),
      m_batteryCharge(), m_signalStrength(0),m_online(false)
{
}

void OfonoManager::init() {
    qDBusRegisterMetaType<ObjectPathProperties>();
    qDBusRegisterMetaType<ObjectPathPropertiesList>();

    connect(&m_callTimeTimer, &QTimer::timeout, this, &OfonoManager::updateCallTime);
    connect(&m_manager, &org::ofono::Manager::ModemAdded, this, &OfonoManager::ModemAdded);
    connect(&m_manager, &org::ofono::Manager::ModemRemoved, this, &OfonoManager::ModemRemoved);
}

QStringList OfonoManager::getModems() {
    QDBusPendingReply<ObjectPathPropertiesList> dbusCall = m_manager.GetModems();
    dbusCall.waitForFinished();

    QStringList ret;
    if(dbusCall.isError()){
        qCWarning(OFONO2)  << "Error OfonoManager::getModems :"<< dbusCall.error().message();
        return ret;
    }

    ObjectPathPropertiesList val = dbusCall.value();

    for (int i = 0; i < val.size(); ++i) {
        ObjectPathProperties prop = val[i];
        ret << prop.path.path();
    }

    return ret;
}
void OfonoManager::loadProperties() {
    if(m_networkregistration) {
        QDBusPendingReply<QVariantMap> networkregistrationPropertiesCall = m_networkregistration->GetProperties();
        networkregistrationPropertiesCall.waitForFinished();
        QVariantMap networkregistrationProperties = networkregistrationPropertiesCall.value();

        m_networkName = networkregistrationProperties["Name"].toString();
        m_signalStrength = networkregistrationProperties["Strength"].toInt();
        emit networkNameChanged();
        emit signalStrengthChanged();
    }

    if(m_handsfree){
        QDBusPendingReply<QVariantMap> handsfreePropertiesCall = m_handsfree->GetProperties();
        handsfreePropertiesCall.waitForFinished();
        QVariantMap handsfreeProperties = handsfreePropertiesCall.value();

        m_batteryCharge = handsfreeProperties["BatteryChargeLevel"].toInt();
        emit batteryChargeChanged();
    }
    if(m_voicecallmanager){
        QDBusPendingReply<ObjectPathPropertiesList> callsCall = m_voicecallmanager->GetCalls();
        callsCall.waitForFinished();
        ObjectPathPropertiesList calls = callsCall.value();

        if(calls.size() > 0) {
            CallAdded(calls[0].path, calls[0].properties);
        }
    }
    m_online = true;
    emit onlineChanged();
}
int OfonoManager::setModem(QString path) {
    if(!getModems().contains(path)){
        qCWarning(OFONO2)  << ": No modem found";
        return 0;
    }
    m_activeDevicePath = path;

    m_modem.reset(new org::ofono::Modem("org.ofono", path ,m_dbusConnection));
    m_networkregistration.reset(new org::ofono::NetworkRegistration("org.ofono", path ,m_dbusConnection));
    m_handsfree.reset(new org::ofono::Handsfree("org.ofono", path ,m_dbusConnection));
    m_voicecallmanager.reset(new org::ofono::VoiceCallManager("org.ofono", path ,m_dbusConnection));

    connect(m_modem.data(), &org::ofono::Modem::PropertyChanged, this, &OfonoManager::ModemPropertyChanged);
    connect(m_networkregistration.data(), &org::ofono::NetworkRegistration::PropertyChanged, this, &OfonoManager::NetworkRegistrationPropertyChanged);
    connect(m_modem.data(), &org::ofono::Modem::PropertyChanged, this, &OfonoManager::HandsfreePropertyChanged);
    connect(m_voicecallmanager.data(), &org::ofono::VoiceCallManager::CallAdded, this, &OfonoManager::CallAdded);
    connect(m_voicecallmanager.data(), &org::ofono::VoiceCallManager::CallRemoved, this, &OfonoManager::CallRemoved);

    loadProperties();
    return 1;
}
int OfonoManager::setDefaultModem(QString path) {
    m_deviceDefaultPath = path;
    if(!getModems().contains(m_deviceDefaultPath)){
        qCWarning(OFONO2)  << ": No modem found";
        return 0;
    }
    setModem(m_deviceDefaultPath);
    return 1;
}

void OfonoManager::CallAdded(const QDBusObjectPath &path, const QVariantMap &properties) {
    m_voicecall.reset(new org::ofono::VoiceCall("org.ofono", path.path(), m_dbusConnection));
    connect(m_voicecall.data(), &org::ofono::VoiceCall::PropertyChanged, this, &OfonoManager::CallPropertyChanged);

    m_callState = properties["State"].toString();
    m_caller = properties["LineIdentification"].toString();

    if(properties.contains("StartTime")) {
        startCallTimer(properties["StartTime"].toString());
    }

    qCWarning(OFONO2)  << ": Call Added";

    emit callStateChanged();
    emit callerChanged();
    emit showOverlay();
}

void OfonoManager::CallRemoved(const QDBusObjectPath &path) {
    qCWarning(OFONO2)  << ": Call Removed";
    emit hideOverlay();
    emit callFinished();
    stopCallTimer();
    m_voicecall.clear();
}

void OfonoManager::CallPropertyChanged(const QString &property, const QDBusVariant &value) {
    QVariant variantValue = value.variant();
    if(property == "State"){
        if(variantValue == "disconnected"){
            stopCallTimer();
        }
        m_callState = value.variant().toString();
        emit callStateChanged();
    } else if(property == "StartTime") {
        startCallTimer(value.variant().toString());
    }
}

void OfonoManager::ModemAdded(const QDBusObjectPath &path, const QVariantMap &properties) {
    if(path.path() == m_deviceDefaultPath){
        setModem(path.path());
    }
}
void OfonoManager::disconnectDevice() {
    emit hideOverlay();
    m_batteryCharge = 0;
    m_signalStrength = 0;
    m_networkName.clear();
    m_online = false;
    emit batteryChargeChanged();
    emit signalStrengthChanged();
    emit networkNameChanged();
    emit onlineChanged();
}
void OfonoManager::ModemRemoved(const QDBusObjectPath &path) {
    if(path.path() == m_activeDevicePath){
        disconnectDevice();
    }
}

void OfonoManager::ModemPropertyChanged(const QString &property, const QDBusVariant &value) {
    QVariant variantValue = value.variant();
    if(property == "Online") {
        if(!value.variant().toBool()){
            disconnectDevice();
        }
    }
}

void OfonoManager::HandsfreePropertyChanged(const QString &property, const QDBusVariant &value) {
    QVariant variantValue = value.variant();
    if(property == "BatteryChargeLevel") {
        m_batteryCharge = variantValue.toInt();
        emit batteryChargeChanged();
    }
}

void OfonoManager::NetworkRegistrationPropertyChanged(const QString &property, const QDBusVariant &value) {
    QVariant variantValue = value.variant();
    if(property == "Name") {
        m_networkName = variantValue.toString();
        emit networkNameChanged();

    } else if(property == "Strength") {
        m_signalStrength = variantValue.toInt();
        emit signalStrengthChanged();
    }
}
void OfonoManager::updateCallTime() {
    qint64 elapsed = m_callStartTime.msecsTo(QDateTime::currentDateTime());
    QTime time(0,0);
    time = time.addMSecs(elapsed);
    m_callTime = time.toString(Qt::TextDate);

    emit callTimeChanged();
}

void OfonoManager::startCallTimer(QString time) {
    m_callStartTime = QDateTime::fromString(time, Qt::ISODate);
    updateCallTime();
    m_callTimeTimer.start(1000);
}

void OfonoManager::stopCallTimer() {
    m_callTimeTimer.stop();
    m_callTime = "";
    emit callTimeChanged();
}

void OfonoManager::answerCall() {
    if(m_voicecall){
        m_voicecall->Answer();
    }
}
void OfonoManager::hangupCall() {
    if(m_voicecall){
        m_voicecall->Hangup();
    } else if(m_voicecallmanager) {
        m_voicecallmanager->HangupAll();
    }
}

void OfonoManager::dialNumber(QString number) {
    if(m_voicecallmanager) {
        m_voicecallmanager->Dial(number, "");
    }
}

void OfonoManager::activateVoiceControl() {
    if(m_handsfree) {
        m_handsfree->SetProperty("VoiceRecognition", QDBusVariant(true));
    }
}
