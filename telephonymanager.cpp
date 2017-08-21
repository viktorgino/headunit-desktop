#include "telephonymanager.h"
#include  "qofonovoicecallmanager.h"
#include  "qofonovoicecall.h"

static const QLatin1String serviceUuid("e8e10f95-1a70-4b27-9ccf-02010264e9c8");

TelephonyManager::TelephonyManager(QObject *parent) : QObject(parent)
{
    QList<QBluetoothHostInfo> localAdapters = QBluetoothLocalDevice::allDevices();
    localAdapter = new QBluetoothLocalDevice(localAdapters.at(0).address());

    if (localAdapter->isValid()) {
        qDebug() << "Bluetooth is available on this device";
        localAdapter->setHostMode(QBluetoothLocalDevice::HostConnectable);
        localAdapter->setHostMode(QBluetoothLocalDevice::HostDiscoverable);
        localAdapter->powerOn();
        localAdapter->setHostMode(QBluetoothLocalDevice::HostDiscoverable);

        qDebug() << "Trying adapter" << localAdapter->name();

        QBluetoothServiceInfo::Sequence classId;

        classId << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::SerialPort));
        serviceInfo.setAttribute(QBluetoothServiceInfo::BluetoothProfileDescriptorList,
                                 classId);

        classId.prepend(QVariant::fromValue(QBluetoothUuid(serviceUuid)));

        serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceClassIds, classId);

        serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceName, tr("viktorgino's Headunit Desktop"));
        serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceDescription,
                                 tr("A2DP and HFP server for headunit-dekstop"));
        serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceProvider, tr("headunit.viktorgino.me/"));

        serviceInfo.setServiceUuid(QBluetoothUuid(serviceUuid));

        QBluetoothServiceInfo::Sequence publicBrowse;
        publicBrowse << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::PublicBrowseGroup));
        serviceInfo.setAttribute(QBluetoothServiceInfo::BrowseGroupList,
                                 publicBrowse);

        QBluetoothServiceInfo::Sequence protocolDescriptorList;
        QBluetoothServiceInfo::Sequence protocol;
        protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::AudioSink));
        protocolDescriptorList.append(QVariant::fromValue(protocol));
        protocol.clear();
        protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::AudioSource));
        protocolDescriptorList.append(QVariant::fromValue(protocol));
        protocol.clear();
        protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::Handsfree));
        protocolDescriptorList.append(QVariant::fromValue(protocol));
        protocol.clear();
        protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::SIMAccess));
        protocolDescriptorList.append(QVariant::fromValue(protocol));
        protocol.clear();
        protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::AV_RemoteControl));
        protocolDescriptorList.append(QVariant::fromValue(protocol));
        protocol.clear();
        protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::PhonebookAccess));
        protocolDescriptorList.append(QVariant::fromValue(protocol));
        protocol.clear();
        protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::ServiceDiscoveryServer));
        protocolDescriptorList.append(QVariant::fromValue(protocol));

        serviceInfo.setAttribute(QBluetoothServiceInfo::ProtocolDescriptorList,
                                 protocolDescriptorList);

        serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceAvailability,10000);
        if(serviceInfo.registerService(localAdapter->address())){
            qDebug() << "Service is successfully registered";
        } else {
            qDebug() << "Service is NOT successfully registered";
        }
        connect(localAdapter, SIGNAL(deviceConnected(const QBluetoothAddress)), this, SLOT(deviceConnected(const QBluetoothAddress)));
        connect(localAdapter, SIGNAL(deviceDisconnected(const QBluetoothAddress)), this, SLOT(deviceDisconnected(const QBluetoothAddress)));
        connect(localAdapter, SIGNAL(pairingDisplayPinCode(const QBluetoothAddress, QString)), this,
                SLOT(pairingDisplayPinCode(const QBluetoothAddress, QString)));
        connect(localAdapter, SIGNAL(pairingDisplayConfirmation(const QBluetoothAddress, QString)), this,
                SLOT(pairingDisplayPinCode(const QBluetoothAddress, QString)));
        qDebug() << "Adapters:";
        for (int i = 0; i < localAdapter->connectedDevices().size(); ++i) {
            qDebug() << "    " << localAdapter->connectedDevices().at(i).toString();
        }

        connect(&manager, &QOfonoManager::availableChanged, this, &TelephonyManager::initHFP);
    }
}
TelephonyManager::~TelephonyManager(){
    qDebug() << "Destroying TelephonyManager";
    localAdapter->setHostMode(QBluetoothLocalDevice::HostPoweredOff);
}

void TelephonyManager::deviceConnected(const QBluetoothAddress &address){
    qDebug() << "deviceConnected";
}
void TelephonyManager::deviceDisconnected(const QBluetoothAddress &address){
    qDebug() << "deviceDisconnected";
}
void TelephonyManager::pairingDisplayPinCode(const QBluetoothAddress &address, QString pin){
    qDebug() << "Pairing confirmation pin: " << pin;
}
void TelephonyManager::initHFP(bool available){
    if(available){
        QStringList modems = manager.modems();
        if(modems.size() > 0){
            modemPath = modems[0];
            voiceCallManager.setModemPath(modemPath);
            connect(&voiceCallManager, &QOfonoVoiceCallManager::callAdded, this, &TelephonyManager::callAdded);
        } else {
            qDebug() << "[ofono] : No modem found";
        }
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

void TelephonyManager::loadAddressBook(){
    qDebug() << "[ofono] : Starting phonebook import";
    phonebook.setModemPath(modemPath);
    phonebook.beginImport();
    connect(&phonebook,&QOfonoPhonebook::importReady,this,&TelephonyManager::vCardImportReady);
}
void TelephonyManager::vCardImportReady(const QString vcardData){
    qDebug() << "[ofono] : Phonebook import ready";
    QByteArray byteArray = vcardData.toLocal8Bit();
    QBuffer buffer(&byteArray);
    vcf::VcfReader reader(&buffer);
    if (reader.open())
    {
        qDebug()<<reader.infos().keys();
        qDebug()<<reader.metadata().keys();
        qDebug()<<reader.samples();

        while (reader.next())
        {
            qDebug()<<reader.record().alt();
        }
    }
}
