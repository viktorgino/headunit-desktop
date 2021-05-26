#include "telephonymanager.h"
#include  "qofonovoicecallmanager.h"
#include  "qofonovoicecall.h"

Q_LOGGING_CATEGORY(HEADUNIT, "telephony")
Q_LOGGING_CATEGORY(OFONO, "telephony [qoFono]")
Q_LOGGING_CATEGORY(BLUEZ, "telephony [BluezQt]")
TelephonyManager::TelephonyManager(QObject *parent) : QObject(parent), m_ofonoManager(this), m_ofonoModem(this),
    m_ofonoVoiceCallManager(this), m_ofonoHandsFree(this),  m_ofonoVoiceCall(this),
    m_bluez_manager(this), m_obexManager(this)
{
    m_contactsFolder = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/contacts";

    m_interfaceSettings.mediaStream = true;

    m_pluginSettings.eventListeners = QStringList() << "AndroidAuto::connected";

}
TelephonyManager::~TelephonyManager(){
//    delete m_activeDevice;
}

QObject *TelephonyManager::getContextProperty(){
    return this;
}

void TelephonyManager::init() {
    BluezQt::InitManagerJob *job = m_bluez_manager.init();
    job->start();
    connect(job, &BluezQt::InitManagerJob::result, this, &TelephonyManager::initBluez);

    connect(&m_ofonoManager, &QOfonoManager::availableChanged, this, &TelephonyManager::ofonoAvailableChanged);
    connect(&m_ofonoVoiceCallManager, &QOfonoVoiceCallManager::callAdded, this, &TelephonyManager::callAdded);
    connect(&m_ofonoVoiceCall, &QOfonoVoiceCall::stateChanged, this, &TelephonyManager::voiceCallStateChanged);

    connect(&m_obexManager, &BluezQt::ObexManager::sessionAdded, this, &TelephonyManager::obexPullPhonebook);

    connect(&m_bluez_manager, &BluezQt::Manager::deviceAdded, this, &TelephonyManager::deviceAdded);
    connect(&m_bluez_manager, &BluezQt::Manager::deviceRemoved, this, &TelephonyManager::deviceRemoved);
    emit contactsFolderChanged();
}

void TelephonyManager::initObex (BluezQt::InitObexManagerJob *job){
    disconnect(job, &BluezQt::InitObexManagerJob::result, this, &TelephonyManager::initObex);
    if(job->error() == BluezQt::Job::Error::NoError){
        qCDebug(BLUEZ)  << "obex manager started";
        if(m_activeDevice){
            getPhonebooks(m_activeDevice->address());
        }
    } else {
        qCWarning(BLUEZ)  << "initObex : "<< job->error() << job->errorText();
    }
}

void TelephonyManager::initBluez (BluezQt::InitManagerJob *job){
    disconnect(job, &BluezQt::InitManagerJob::result, this, &TelephonyManager::initBluez);
    if(job->error() == BluezQt::Job::Error::NoError){
        qCDebug(BLUEZ)  << "Manager Started";

        bluez_agent = new BluezAgent();
        BluezQt::PendingCall *agent_pcall = m_bluez_manager.registerAgent(bluez_agent);
        agent_pcall->waitForFinished();
        if(agent_pcall->error() != BluezQt::PendingCall::NoError){
            qCWarning(BLUEZ)  << "Error registerAgent  : " << agent_pcall->errorText();
        } else {
            qCDebug(BLUEZ)  << "Agent registered";
        }

        agent_pcall = m_bluez_manager.requestDefaultAgent(bluez_agent);
        agent_pcall->waitForFinished();
        if(agent_pcall->error() != BluezQt::PendingCall::NoError){
            qCWarning(BLUEZ)  << "Error requestDefaultAgent  : " << agent_pcall->errorText();
        } else {
            qCDebug(BLUEZ)  << "Agent set as default";
        }

        if(m_bluez_manager.adapters().size() > 0) {
            initAdapter(m_bluez_manager.adapters().at(0));
        }

        BluezQt::PendingCall * obexServiceStart = BluezQt::ObexManager::startService();
        obexServiceStart->waitForFinished();
        if(obexServiceStart->error() != BluezQt::PendingCall::NoError){
            qCWarning(BLUEZ)  << "Error : obexServiceStart  : " << obexServiceStart->errorText();
        } else {
            qCDebug(BLUEZ)  << "obex service started";
        }

        BluezQt::InitObexManagerJob *obexjob = m_obexManager.init();
        obexjob->start();
        connect(obexjob, &BluezQt::InitObexManagerJob::result, this, &TelephonyManager::initObex);
    } else {
        qCWarning(BLUEZ)  << "Error : bluezManagerStartResult : " << job->errorText();
        return;
    }
}

void TelephonyManager::initAdapter(BluezQt::AdapterPtr adapter) {
    m_bluez_adapter = adapter.data();
    connect(m_bluez_adapter, &BluezQt::Adapter::discoveringChanged, this, &TelephonyManager::deviceDiscoveringChanged);
    m_bluez_adapter->setName(m_settings["adapterName"].toString());
    m_bluez_adapter->setPowered(true);
    m_bluez_adapter->setDiscoverable(false);
    m_bluez_adapter->setPairable(false);
//    qCDebug(BLUEZ)  << "Starting discovery";
//    if(!m_bluez_adapter->isDiscovering()){
//        m_bluez_adapter->startDiscovery();
//    }
    //If the newly added device is trusted then disconnect from all other devices and connect to it

    for(BluezQt::DevicePtr p_device : m_bluez_manager.devices()) {
        if(p_device->isConnected()){
            if(!m_activeDevice){
                qCDebug(BLUEZ)  << "Connected device found: " << p_device->name();
                setBluezDevice(p_device.data());
            } else {
                qCDebug(BLUEZ)  << "Disconnecting from : " << p_device->name();
                p_device->disconnectFromDevice();
            }
        }
    }
    if(!m_activeDevice) {
        qCDebug(BLUEZ)  << "No device connected, trying to connect";
        connectToNextDevice();
    }

}

void TelephonyManager::initOfono(QString ubi){
    qCDebug(OFONO) << "Init oFono";
    if(m_ofonoManager.available()){
        QStringList modems = m_ofonoManager.modems();
        QString fullPath = "/hfp" + ubi;
        if(modems.contains(fullPath)){
            m_ofonoModem.setModemPath(fullPath);
            m_ofonoVoiceCallManager.setModemPath(m_ofonoModem.modemPath());
            m_ofonoHandsFree.setModemPath(m_ofonoModem.modemPath());
        } else {
            qCWarning(OFONO)  << ": No modem found";
        }
    } else {
        qCWarning(OFONO) << "Ofono not available";
    }
}

void TelephonyManager::updateAdapters(){
    m_adapters.clear();
    //    for(BluezQt::AdapterPtr adapter : m_bluez_manager.adapters()){
    //        m_adapters.append(adapter->systemName());
    //    }
    emit adaptersUpdated();
}

void TelephonyManager::ofonoAvailableChanged(bool available){
    qCDebug(OFONO) << "ofonoAvailableChanged : " << available;
    if(available && m_activeDevice){
        initOfono(m_activeDevice->ubi());
    }
}

void TelephonyManager::toggleVoice(){
    m_ofonoHandsFree.setVoiceRecognition(!m_ofonoHandsFree.voiceRecognition());
}

void TelephonyManager::callAdded(const QString &call)
{
    qCDebug(OFONO) << "call added : " << call;
    m_ofonoVoiceCall.setVoiceCallPath(call);
}
void TelephonyManager::voiceCallStateChanged(const QString &state){
    qCDebug(OFONO) << "voiceCallStateChanged : " << state;
    if(state == "incoming"){
        qCDebug(OFONO) << "Incoming call from name:" << m_ofonoVoiceCall.name()
                          << " | information:"<< m_ofonoVoiceCall.information()
                          << " | voiceCallPath:"<< m_ofonoVoiceCall.voiceCallPath()
                          << " | lineIdentification:"<< m_ofonoVoiceCall.lineIdentification()
                          << " | incomingLine:"<< m_ofonoVoiceCall.incomingLine()
                          << " | state:"<< m_ofonoVoiceCall.state()
                          << " | startTime:"<< m_ofonoVoiceCall.startTime();
        emit incomingCall(m_ofonoVoiceCall.name(), m_ofonoVoiceCall.lineIdentification(), m_ofonoVoiceCall.voiceCallPath());

        QVariantMap props;
        //        props.insert("caller", voiceCall->name());
        props.insert("caller", m_ofonoVoiceCall.lineIdentification());
        props.insert("callPath", m_ofonoVoiceCall.voiceCallPath());
        emit action("GUI::OpenOverlay", props);
    } else if(state == "disconnected"){
        emit action("GUI::CloseOverlay", QVariant());
    }
}

void TelephonyManager::answerCall(QString call_path){
    m_ofonoVoiceCall.setVoiceCallPath(call_path);
    m_ofonoVoiceCall.answer();
}

void TelephonyManager::declineCall(QString call_path){
    m_ofonoVoiceCall.setVoiceCallPath(call_path);
    m_ofonoVoiceCall.hangup();
}

void TelephonyManager::obexPullPhonebook (BluezQt::ObexSessionPtr session){
    qCDebug(BLUEZ) << "obex session added";

    qCDebug(BLUEZ)  << "Pulling phonebook";
    QString path = session.data()->objectPath().path();

    qDBusRegisterMetaType<QPair<QString,QString>>();
    qDBusRegisterMetaType<QList<QPair<QString,QString>>>();

    org::bluez::obex::PhonebookAccess1 *pbapAccess;
    pbapAccess = new org::bluez::obex::PhonebookAccess1("org.bluez.obex", path, QDBusConnection::sessionBus(), this);

    //Select phone's internal phonebook
    QDBusPendingCall pcall = pbapAccess->Select("int", "pb");
    pcall.waitForFinished();

    if(pcall.isError()){
        qCWarning(BLUEZ)  << "Error phonebook select:"<< pcall.error().message();
        return;
    }

    //Delete and recreate contact dir to avoid conflicts
    QDir dir(m_contactsFolder);
    if(dir.exists())
        dir.removeRecursively();
    dir.mkpath(m_contactsFolder);

    //Pull all entries from the phone's internal phonebook
    pcall = pbapAccess->PullAll(m_contactsFolder + "/contacts.vcf", QVariantMap());
    pcall.waitForFinished();
    if(pcall.isError()){
        qCWarning(BLUEZ)  << "Error phonebook pull:"<< pcall.error().message();
        return;
    }
    qCDebug(BLUEZ)  << "Got phonebook";
    emit phonebookChanged();
}

void TelephonyManager::getPhonebooks(QString destination){
    if(m_obexManager.isOperational()){
        QVariantMap args;
        args.insert("Target", "PBAP");
        m_obexManager.createSession(destination, args);
    } else {
        qCWarning(BLUEZ)  << "obexManager is not operational";
        return;
    }
}
void TelephonyManager::setBluezDevice(BluezQt::Device *device) {
    if(m_activeDevice){
        m_pairedDevices.insert(m_activeDevice->ubi(), m_activeDevice->name());
    }
    if(device){
        m_pairedDevices.remove(device->ubi());
        qCDebug(BLUEZ)  << "Device set : " << device->name();
        initOfono(device->ubi());
    } else {
        qCDebug(BLUEZ)  << "Device set : NULL";
    }
    m_activeDevice = device;
    emit activeDeviceChanged();
    emit pairedDevicesChanged();
}

void TelephonyManager::deviceAdded(BluezQt::DevicePtr device){
    qCDebug(BLUEZ)  << "Device added : " << device->name();

    connect(device.data(), &BluezQt::Device::connectedChanged, this, &TelephonyManager::deviceConnectionChanged);
    connect(device.data(), &BluezQt::Device::pairedChanged , this, &TelephonyManager::devicePairedChanged);
    if(device->isPaired()){
        m_pairedDevices.insert(device->ubi(),device->name());
    }
    emit pairedDevicesChanged();
}
void TelephonyManager::deviceRemoved(BluezQt::DevicePtr device){
    qCDebug(BLUEZ)  << "Device removed : " << device->name();
    if(m_activeDevice == device){
        setBluezDevice(nullptr);
    }

    disconnect(device.data(), &BluezQt::Device::connectedChanged, this, &TelephonyManager::deviceConnectionChanged);
    disconnect(device.data(), &BluezQt::Device::pairedChanged, this, &TelephonyManager::devicePairedChanged);
    m_pairedDevices.remove(device->ubi());
    emit pairedDevicesChanged();
}

void TelephonyManager::deviceConnectionChanged(bool connected){
    BluezQt::Device* device = dynamic_cast<BluezQt::Device*>(QObject::sender());
    if(connected){
        qCDebug(BLUEZ) << "Device connected : " << device->name() << device->ubi();

        for(BluezQt::DevicePtr p_device : m_bluez_manager.devices()){
            if(device != p_device.data()){
                if(p_device->isConnected()){
                    qCDebug(BLUEZ) << "Disconnecting : " << p_device->name();
                    p_device->disconnectFromDevice();
                }
            }
        }
        setBluezDevice(device);

        getPhonebooks(m_activeDevice->address());
    } else {
        qCDebug(BLUEZ) << "Device disconnected : " << device->name();
        if(m_activeDevice == device){
            setBluezDevice(nullptr);
        }
    }
}

void TelephonyManager::devicePairedChanged(bool paired) {
    BluezQt::Device* device = dynamic_cast<BluezQt::Device*>(QObject::sender());
    if(paired && device->isConnected()){
        deviceConnectionChanged(true);
    }
}

void TelephonyManager::deviceDiscoveringChanged(bool discovering){
    qCDebug(BLUEZ)  << "Discovering changed : " << discovering;
}

void TelephonyManager::connectToNextDevice(){
    BluezQt::Device *device = nullptr;
    bool activeDeviceFound = false;
    for(BluezQt::DevicePtr p_device : m_bluez_manager.devices()){
        if(p_device->isTrusted()){
            if(!m_previouslyTriedDevice) {
                device = p_device.data();
                break;
            } else if(p_device == m_previouslyTriedDevice){
                activeDeviceFound = true;
            } else if(activeDeviceFound){
                device = p_device.data();
                break;
            }
        }
    }
    m_previouslyTriedDevice = device;
    if(device){
        qCDebug(BLUEZ)  << "Connecting to : " << device->name();
        BluezQt::PendingCall * connectCall = device->connectToDevice();
        connectCall->setUserData(device->ubi());
        connect(connectCall, &BluezQt::PendingCall::finished, this, &TelephonyManager::connectToDeviceCallback);
    }
}

void TelephonyManager::connectToDeviceCallback(BluezQt::PendingCall *call){
    disconnect(call, &BluezQt::PendingCall::finished, this, &TelephonyManager::connectToDeviceCallback);
    BluezQt::DevicePtr device = m_bluez_manager.deviceForUbi(call->userData().toString());
    if(call->error() == BluezQt::PendingCall::NoError){
        qCDebug(BLUEZ)  << "Finished connecting to : " << device->name();
    } else {
        qCDebug(BLUEZ)  << "Error connecting to : " << device->name() << " : " << call->errorText();
        if(device == m_previouslyTriedDevice){
            connectToNextDevice();
        }
    }
}


void TelephonyManager::enablePairing(){
    if(m_bluez_adapter){
        m_bluez_adapter->setDiscoverable(true);
        m_bluez_adapter->setPairable(true);
    }
}
void TelephonyManager::disablePairing(){
    if(m_bluez_adapter){
        m_bluez_adapter->setDiscoverable(false);
        m_bluez_adapter->setPairable(false);
    }
}

void TelephonyManager::settingsChanged(const QString &key, const QVariant &value){
    if(key == "adapterName") {
        if(value.canConvert(QMetaType::QString)){
            m_bluez_adapter->setName(value.toString());
        }
    }
}

void TelephonyManager::connectToDevice(QString ubi){
    BluezQt::DevicePtr device = m_bluez_manager.deviceForUbi(ubi);
    if(device){
        if(m_activeDevice){
            m_activeDevice->disconnectFromDevice();
        }
        device->connectToDevice();
    }
}
void TelephonyManager::mediaPlaybackStarted() {
    if(!m_androidAutoConnected){
        emit playbackStarted();
    }
}
void TelephonyManager::eventMessage(QString id, QVariant message) {
    if(id == "AndroidAuto::connected"){
        m_androidAutoConnected = message.toBool();
    }
}

