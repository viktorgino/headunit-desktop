#ifndef TelephonyManager_H

#include <QObject>
#include <plugininterface.h>
#include <qofonomanager.h>
#include <qofonophonebook.h>
#include <qofonovoicecallmanager.h>
#include <qofonovoicecall.h>
#include <qofonomodem.h>
#include <qofonohandsfree.h>
#include <BluezQt/Manager>
#include <BluezQt/Agent>
#include <BluezQt/Adapter>
#include <BluezQt/Device>
#include <BluezQt/MediaPlayer>
#include <BluezQt/ObexManager>
#include <BluezQt/ObexSession>
#include <BluezQt/PendingCall>
#include <BluezQt/InitManagerJob>
#include <BluezQt/InitObexManagerJob>
#include <BluezQt/Profile>
#include <BluezQt/ObexFileTransfer>
#include <BluezQt/ObexFileTransferEntry>
#include "phonebookaccess1_interface.h"
#include <QStandardPaths>
#include <QLoggingCategory>

#include "bluezagent.h"

class TelephonyManager : public QObject, PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.viktorgino.headunit.phonebluetooth" FILE "config.json")
    Q_PROPERTY(QStringList adapters READ getAdapters NOTIFY adaptersUpdated)
    Q_PROPERTY(QString contactsFolder READ getContactsFolder NOTIFY contactsFolderChanged)
    Q_PROPERTY(QString activeDevice READ getActiveDevice NOTIFY activeDeviceChanged)
    Q_PROPERTY(QVariantMap pairedDevices READ getPairedDevices NOTIFY pairedDevicesChanged)
    Q_PROPERTY(QString activeDeviceName READ getActiveDeviceName NOTIFY activeDeviceChanged)

    Q_INTERFACES(PluginInterface)
public:
    explicit TelephonyManager(QObject *parent = nullptr);
    ~TelephonyManager();

    QObject *getContextProperty() override;
    void init() override;

    QString getContactsFolder() const {
        return m_contactsFolder;
    }

    QString getActiveDevice() {
      if(m_activeDevice) {
        return m_activeDevice->ubi();
      } else {
        return "";
      }
    }
    QString getActiveDeviceName() {
      if(m_activeDevice) {
        return m_activeDevice->name();
      } else {
        return "";
      }
    }
    QVariantMap getPairedDevices() {
      return m_pairedDevices;
    }
    QStringList getAdapters() {
        updateAdapters();
        return m_adapters;
    }

signals:
    void adaptersUpdated();
    void messageReceived(const QString &sender, const QString &message);
    void incomingCall(QString caller, QString caller_number, QString call_path);
    void phonebookChanged();
    void contactsFolderChanged();
    void activeDeviceChanged();
    void pairedDevicesChanged();

    void action(QString id, QVariant message);

public slots:
    void answerCall(QString call_path);
    void declineCall(QString call_path);
    void toggleVoice();

    void enablePairing();
    void disablePairing();
    void connectToDevice(QString ubi);

private slots:
    void initOfono(QString ubi);
    void initBluez (BluezQt::InitManagerJob *job);
    void initObex (BluezQt::InitObexManagerJob *job);

    void ofonoAvailableChanged(bool available);

    void callAdded(const QString &call);
    void voiceCallStateChanged(const QString &state);


    void obexPullPhonebook (BluezQt::ObexSessionPtr session);
    void deviceAdded(BluezQt::DevicePtr device);
    void deviceRemoved(BluezQt::DevicePtr device);
    void deviceDiscoveringChanged(bool discovering);
    void deviceConnectionChanged(bool connected);
    void devicePairedChanged(bool paired);
    void connectToDeviceCallback(BluezQt::PendingCall *call);

    void settingsChanged(const QString &key, const QVariant &value);
private:
    QOfonoManager m_ofonoManager;
    QOfonoModem m_ofonoModem;

    QOfonoVoiceCallManager m_ofonoVoiceCallManager;
    QOfonoHandsfree m_ofonoHandsFree;
    QOfonoVoiceCall m_ofonoVoiceCall;
    QString modemPath = "";
    QString m_contactsFolder = "contacts/";

    BluezQt::Manager m_bluez_manager;
    BluezQt::Device* m_activeDevice = nullptr;
    BluezQt::Device* m_previouslyTriedDevice = nullptr;
    BluezQt::Adapter *m_bluez_adapter = nullptr;
    BluezQt::ObexManager m_obexManager;
    BluezQt::Agent *bluez_agent;

    void getPhonebooks(QString destination);
    void setBluezDevice(BluezQt::Device* device);
    void initAdapter(BluezQt::AdapterPtr adapter);

    QVariantMap m_pairedDevices;
    QStringList m_adapters;
    void updateAdapters();
    void connectToNextDevice();
};

#endif // TelephonyManager_H
