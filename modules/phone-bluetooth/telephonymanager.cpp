#include "telephonymanager.h"
#include  "qofonovoicecallmanager.h"
#include  "qofonovoicecall.h"

Q_LOGGING_CATEGORY(HEADUNIT, "telephony")
TelephonyManager::TelephonyManager(QObject *parent) : QObject(parent)
{
    bluez_manager = new BluezQt::Manager();
    BluezQt::InitManagerJob *job = bluez_manager->init();
    job->start();
    connect(job, &BluezQt::InitManagerJob::result, this, &TelephonyManager::bluezManagerStartResult);
    m_contactsFolder = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/contacts";
    emit contactsFolderChanged();
}
TelephonyManager::~TelephonyManager(){
    delete modem;
    delete manager;
    delete phonebook;
    delete voiceCall;

    delete bluez_manager;
    delete m_activeDevice;
    delete obexManager;
    delete obexAgent;
    delete obexSessionRegisterPC;
}

void TelephonyManager::obexManagerStartResult (BluezQt::InitObexManagerJob *job){
    if(job->error() == BluezQt::Job::Error::NoError){
        qCWarning(HEADUNIT)  << "[BluezQt] obex manager started";
        for(int i = 0; bluez_manager->devices().size() > i; i++){
            if(bluez_manager->devices().at(i).data()->isConnected()){
                setBluezDevice(bluez_manager->devices().at(i).data());
                getPhonebooks(m_activeDevice->address());
                break;
            }
        }
    } else {
        qCWarning(HEADUNIT)  << "[BluezQt] obexManagerStartResult : "<< job->error() << job->errorText();
    }
}
void TelephonyManager::bluezManagerStartResult (BluezQt::InitManagerJob *job){
    if(job->error() == BluezQt::Job::Error::NoError){
        qCWarning(HEADUNIT)  << "[BluezQt] bluez manager started";

        bluez_agent = new BluezAgent();
        BluezQt::PendingCall *agent_pcall = bluez_manager->registerAgent(bluez_agent);
        agent_pcall->waitForFinished();
        if(agent_pcall->error() != BluezQt::PendingCall::NoError){
            qCWarning(HEADUNIT)  << "[BluezQt] registerAgent  : " << agent_pcall->errorText();
        } else {
            qCWarning(HEADUNIT)  << "[BluezQt] Agent registered";
        }


        agent_pcall = bluez_manager->requestDefaultAgent(bluez_agent);
        agent_pcall->waitForFinished();
        if(agent_pcall->error() != BluezQt::PendingCall::NoError){
            qCWarning(HEADUNIT)  << "[BluezQt] requestDefaultAgent  : " << agent_pcall->errorText();
        } else {
            qCWarning(HEADUNIT)  << "[BluezQt] Agent set as default";
        }

        BluezQt::PendingCall * obexServiceStart = BluezQt::ObexManager::startService();
        obexServiceStart->waitForFinished();
        if(obexServiceStart->error() != BluezQt::PendingCall::NoError){
            qCWarning(HEADUNIT)  << "[BluezQt] obexServiceStart  : " << obexServiceStart->errorText();
        } else {
            qCWarning(HEADUNIT)  << "[BluezQt] obex service started";
        }

        obexManager = new BluezQt::ObexManager();
        BluezQt::InitObexManagerJob *obexjob = obexManager->init();
        obexjob->start();
        connect(obexjob, &BluezQt::InitObexManagerJob::result, this, &TelephonyManager::obexManagerStartResult);
        connect(obexManager, &BluezQt::ObexManager::sessionAdded, this, &TelephonyManager::obexSessionAdded);

        connect(bluez_manager, &BluezQt::Manager::deviceAdded, this, &TelephonyManager::deviceAdded);
        connect(bluez_manager, &BluezQt::Manager::deviceRemoved, this, &TelephonyManager::deviceRemoved);
        connect(bluez_manager, &BluezQt::Manager::deviceChanged, this, &TelephonyManager::deviceChanged);
    } else {
        qCWarning(HEADUNIT)  << "[BluezQt] bluezManagerStartResult : " << job->errorText();
        return;
    }
}

void TelephonyManager::initHFP(bool available){
    if(available){
        QStringList modems = manager->modems();
        if(modems.size() > 0){
            modem = new QOfonoModem;
            modem->setModemPath(modems[0]);
            modem->setOnline(true);
            modem->setPowered(true);
            connect(modem, &QOfonoModem::poweredChanged, this, &TelephonyManager::initHFP2);
        } else {
            qCDebug(HEADUNIT)  << "[ofono] : No modem found";
        }
    }
}

void TelephonyManager::initHFP2(bool available){
    if(available){
        voiceCallManager.setModemPath(modem->modemPath());

        if(voiceCallManager.isValid())
            qCDebug(HEADUNIT)  << "[ofono] voiceCallManager is valid";
        else
            qCWarning(HEADUNIT)  << "[ofono] voiceCallManager is not valid";
        connect(&voiceCallManager, &QOfonoVoiceCallManager::callAdded, this, &TelephonyManager::callAdded);
    }
}

void TelephonyManager::callAdded(const QString &call)
{
    voiceCall = new QOfonoVoiceCall();
    voiceCall->setVoiceCallPath(call);
    connect(voiceCall, &QOfonoVoiceCall::stateChanged, this, &TelephonyManager::voiceCallStateChanged);
}
void TelephonyManager::voiceCallStateChanged(const QString &state){
    qCDebug(HEADUNIT) << "voiceCallStateChanged : " << state;
    if(state == "incoming"){
        qCDebug(HEADUNIT) << "Incoming call from name:" << voiceCall->name()
                          << " | information:"<< voiceCall->information()
                          << " | voiceCallPath:"<< voiceCall->voiceCallPath()
                          << " | lineIdentification:"<< voiceCall->lineIdentification()
                          << " | incomingLine:"<< voiceCall->incomingLine()
                          << " | state:"<< voiceCall->state()
                          << " | startTime:"<< voiceCall->startTime();
        emit incomingCall(voiceCall->name(), voiceCall->lineIdentification(), voiceCall->voiceCallPath());
    } else if(state == "disconnected"){

    }
}

void TelephonyManager::answerCall(QString call_path){
    voiceCall->setVoiceCallPath(call_path);
    voiceCall->answer();
}
void TelephonyManager::declineCall(QString call_path){
    voiceCall->setVoiceCallPath(call_path);
    voiceCall->hangup();
}
void TelephonyManager::obexSessionAdded (BluezQt::ObexSessionPtr session){
    qCDebug(HEADUNIT) << "[BluezQt] obex session added";
    pbapSession = session;
    pullPhonebooks();
}

void TelephonyManager::pullPhonebooks(){
    qCWarning(HEADUNIT)  << "[BluezQt] Pulling phonebook";
    QString path = pbapSession.data()->objectPath().path();

    qDBusRegisterMetaType<QPair<QString,QString>>();
    qDBusRegisterMetaType<QList<QPair<QString,QString>>>();

    org::bluez::obex::PhonebookAccess1 *pbapAccess;
    pbapAccess = new org::bluez::obex::PhonebookAccess1("org.bluez.obex", path, QDBusConnection::sessionBus(), this);

    //Select phone's internal phonebook
    QDBusPendingCall pcall = pbapAccess->Select("int", "pb");
    pcall.waitForFinished();

    if(pcall.isError()){
        qCWarning(HEADUNIT)  << "[Bluez] Phonebook Select:"<< pcall.error().message();
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
        qCWarning(HEADUNIT)  << "[Bluez] Phonebook Pull:"<< pcall.error().message();
        return;
    }
    qCWarning(HEADUNIT)  << "[BluezQt] Got phonebook";
    emit phonebookChanged();
}

void TelephonyManager::getPhonebooks(QString destination){
    if(obexManager->isOperational()){
        QVariantMap args;
        args.insert("Target", "PBAP");
        obexSessionRegisterPC = obexManager->createSession(destination, args);
        obexSessionRegisterPC->waitForFinished();
    } else {
        qCWarning(HEADUNIT)  << "[BluezQt] obexManager is not operational";
        return;
    }
}
void TelephonyManager::setBluezDevice(BluezQt::Device* device) {
    m_deviceIndex = -1;
    if(device){
        for(int i=0; bluez_manager->devices().size() > i; i++){
            if(bluez_manager->devices().at(i).data()->address() == device->address()){
                m_deviceIndex = i;
                break;
            }
        }
    } else {
        for(int i=0; bluez_manager->devices().size() > i; i++){
            if(bluez_manager->devices().at(i).data()->isConnected()){
                m_deviceIndex = i;
                break;
            }
        }
    }
    emit deviceIndexChanged();
    m_activeDevice = device;
}
void TelephonyManager::deviceAdded(BluezQt::DevicePtr device){
    qCWarning(HEADUNIT)  << "[Bluez] Device added";
    if(device.data()->isConnected() && (!m_activeDevice || device.data()->address() != m_activeDevice->address())){
        setBluezDevice(device.data());
        getPhonebooks(m_activeDevice->address());
    }
}

void TelephonyManager::deviceRemoved(BluezQt::DevicePtr device){
    if(m_activeDevice && device.data()->address() == m_activeDevice->address()){
        setBluezDevice(NULL);
    }
}

void TelephonyManager::deviceChanged(BluezQt::DevicePtr device){
    //If new device connected
    if(device.data()->isConnected() && (!m_activeDevice || device.data()->address() != m_activeDevice->address())){
        setBluezDevice(device.data());
        getPhonebooks(m_activeDevice->address());
    }
    //If current device disconnected
    if(!device.data()->isConnected() && m_activeDevice && device.data()->address() == m_activeDevice->address()){
        setBluezDevice(NULL);
    }
}
