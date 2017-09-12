#include "telephonymanager.h"
#include  "qofonovoicecallmanager.h"
#include  "qofonovoicecall.h"

static const QLatin1String serviceUuid("e8e10f95-1a70-4b27-9ccf-02010264e9c8");

TelephonyManager::TelephonyManager(QObject *parent) : QObject(parent)
{
    QList<QBluetoothHostInfo> localAdapters = QBluetoothLocalDevice::allDevices();
    if(localAdapters.size() > 0){
        localAdapter = new QBluetoothLocalDevice(localAdapters.at(0).address());

        manager = new QOfonoManager;
        connect(manager, &QOfonoManager::availableChanged, this, &TelephonyManager::initHFP);
    }
    //initBluez();
}
TelephonyManager::~TelephonyManager(){
    qDebug() << "Destroying TelephonyManager";
    localAdapter->setHostMode(QBluetoothLocalDevice::HostPoweredOff);
    delete localAdapter;

    delete modem;
    delete manager;
    delete phonebook;
    delete voiceCall;
}
void TelephonyManager::setupService(){

    if (localAdapter->isValid()) {
        qDebug() << "Bluetooth is available on this device";
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
        protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::HandsfreeAudioGateway));
        protocolDescriptorList.append(QVariant::fromValue(protocol));
        protocol.clear();
        protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::SIMAccess));
        protocolDescriptorList.append(QVariant::fromValue(protocol));
        protocol.clear();
        protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::BatteryService));
        protocolDescriptorList.append(QVariant::fromValue(protocol));
        protocol.clear();
        protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::DeviceInformation));
        protocolDescriptorList.append(QVariant::fromValue(protocol));
        protocol.clear();
        protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::MessageAccessProfile));
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
        connect(localAdapter, &QBluetoothLocalDevice::pairingFinished,
                this, &TelephonyManager::pairingFinished);

    }
}

void TelephonyManager::deviceConnected(const QBluetoothAddress &address){
    qDebug() << "deviceConnected: " << address.toString();
    emit deviceListChanged();
}
void TelephonyManager::pairingFinished(const QBluetoothAddress &address){
    qDebug() << "pairingFinished: " << address.toString();
    emit deviceListChanged();
}
void TelephonyManager::deviceDisconnected(const QBluetoothAddress &address){
    qDebug() << "deviceDisconnected: " << address.toString();
    emit deviceListChanged();
}
void TelephonyManager::pairingDisplayPinCode(const QBluetoothAddress &address, QString pin){
    qDebug() << "Pairing confirmation "  << address.toString() << " pin: " << pin;
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
        setupService();
        qDebug() << "[OFONO] interfaces:";
        for(int i = 0; i < modem->interfaces().size(); i++){
            qDebug() << "    " << modem->interfaces().at(i);
        }
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
