#include "telephonymanager.h"
#include  "qofonovoicecallmanager.h"
#include  "qofonovoicecall.h"


TelephonyManager::TelephonyManager(QObject *parent) : QObject(parent)
{
    BluezQt::Manager *manager = new BluezQt::Manager();
    BluezQt::InitManagerJob *job = manager->init();
    job->start();
    connect(job, &BluezQt::InitManagerJob::result, this, &TelephonyManager::bluezManagerStartResult);
}
TelephonyManager::~TelephonyManager(){
    delete modem;
    delete manager;
    delete phonebook;
    delete voiceCall;
}

void TelephonyManager::obexManagerStartResult (BluezQt::InitObexManagerJob *job){
    if(job->error() != 0){
        qWarning() << "[BluezQt] obexManagerStartResult : "<< job->error() << job->errorText();
        return;
    }
}
void TelephonyManager::bluezManagerStartResult (BluezQt::InitManagerJob *job){
    if(job->error() == BluezQt::Job::Error::NoError){
        obexManager = new BluezQt::ObexManager();
        BluezQt::InitObexManagerJob *obexjob = obexManager->init();
        obexjob->start();
        connect(obexjob, &BluezQt::InitObexManagerJob::result, this, &TelephonyManager::obexManagerStartResult);
        connect(obexManager, &BluezQt::ObexManager::sessionAdded, this, &TelephonyManager::obexSessionAdded);
    } else {
        qWarning() << "[BluezQt] obexManagerStartResult : " << job->errorText();
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
            qDebug() << "[ofono] : No modem found";
        }
    }
}

void TelephonyManager::initHFP2(bool available){
    if(available){
        voiceCallManager.setModemPath(modem->modemPath());

        if(voiceCallManager.isValid())
            qDebug() << "[ofono] voiceCallManager is valid";
        else
            qWarning() << "[ofono] voiceCallManager is not valid";
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
    qDebug()<< "voiceCallStateChanged : " << state;
    if(state == "incoming"){
        qDebug()<< "Incoming call from name:" << voiceCall->name()
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
    pbapSession = session;
    pullPhonebooks();
}

void TelephonyManager::pullPhonebooks(){
    contactsFolder = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/contacts";

    QString path = pbapSession.data()->objectPath().path();

    qDBusRegisterMetaType<QPair<QString,QString>>();
    qDBusRegisterMetaType<QList<QPair<QString,QString>>>();

    org::bluez::obex::PhonebookAccess1 *pbapAccess;
    pbapAccess = new org::bluez::obex::PhonebookAccess1("org.bluez.obex", path, QDBusConnection::sessionBus(), this);

    //Select phone's internal phonebook
    QDBusPendingCall pcall = pbapAccess->Select("int", "pb");
    pcall.waitForFinished();

    if(pcall.isError()){
        qWarning() << "[Bluez] Phonebook Select:"<< pcall.error().message();
        return;
    }

    //Delete and recreate contact dir to avoid conflicts
    QDir dir(contactsFolder);
    if(dir.exists())
        dir.removeRecursively();
    dir.mkpath(contactsFolder);

    //Pull all entries from the phone's internal phonebook
    pcall = pbapAccess->PullAll(contactsFolder + "/contacts.vcf", QVariantMap());
    pcall.waitForFinished();
    if(pcall.isError()){
        qWarning() << "[Bluez] Phonebook Pull:"<< pcall.error().message();
        return;
    }
    emit phonebookChanged(contactsFolder);
}
void TelephonyManager::getPhonebooks(QString destination){
    if(obexManager->isOperational()){
        BluezQt::PendingCall * obexServiceStart = obexManager->startService();
        obexServiceStart->waitForFinished();
        if(obexServiceStart->error() != BluezQt::PendingCall::NoError){
            qWarning() << "[BluezQt] obexServiceStart  : " << obexServiceStart->errorText();
            return;
        }
        if(obexManager->sessions().size() == 0){
            QVariantMap args;
            args.insert("Target", "PBAP");
            obexSessionRegisterPC = obexManager->createSession(destination, args);
            obexSessionRegisterPC->waitForFinished();
        } else {
            QList<BluezQt::ObexSessionPtr> sessions = obexManager->sessions();
            qDebug() << "[BluezQt] Session already exists";
            for(int i=0; i < sessions.size(); i++){
                qWarning() << "    " << sessions.at(i).data()->objectPath().path();
            }
            pbapSession = obexManager->sessions().at(0);
            pullPhonebooks();
        }
    } else {
        qWarning() << "[BluezQt] obexManager is not operational";
        return;
    }
}
