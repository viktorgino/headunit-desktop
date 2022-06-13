#ifndef TelephonyManager_H

#include <QObject>
#include <plugininterface.h>
#include <mediainterface.h>

//#include <qofonomanager.h>
//#include <qofonophonebook.h>
//#include <qofonovoicecallmanager.h>
//#include <qofonovoicecall.h>
//#include <qofonomodem.h>
//#include <qofonohandsfree.h>
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
#include <QFileSystemWatcher>


#include "ofonomanager.h"
#include "bluezagent.h"
#include "phonebookmodel.h"

class TelephonyManager : public QObject, PluginInterface, public MediaInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.viktorgino.headunit.phonebluetooth" FILE "config.json")
    Q_PROPERTY(QStringList adapters READ getAdapters NOTIFY adaptersUpdated)
    Q_PROPERTY(QString activeDevice READ getActiveDevice NOTIFY activeDeviceChanged)
    Q_PROPERTY(QVariantMap pairedDevices READ getPairedDevices NOTIFY pairedDevicesChanged)
    Q_PROPERTY(QString activeDeviceName READ getActiveDeviceName NOTIFY activeDeviceChanged)
    Q_PROPERTY(OfonoManager *Handsfree READ getOfonoManager CONSTANT)
    Q_PROPERTY(PhonebookModel *PhonebookModel READ getPhonebookModel CONSTANT)
    Q_PROPERTY(PhonebookModel *CallHistoryModel READ getCallHistoryModel CONSTANT)

    Q_INTERFACES(PluginInterface)
public:
    explicit TelephonyManager(QObject *parent = nullptr);
    ~TelephonyManager();

    QObject *getContextProperty() override;
    void init() override;

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
    QStringList getAdapters() {
        updateAdapters();
        return m_adapters;
    }
    QString getContactsFolder() const {return m_contactsFolder;}
    QVariantMap getPairedDevices() {return m_pairedDevices;}
    OfonoManager *getOfonoManager() {return &m_ofonoManagerClass;}
    PhonebookModel *getPhonebookModel() {return &m_phonebookModel;}
    PhonebookModel *getCallHistoryModel() {return &m_callHistoryModel;}


signals:
    void adaptersUpdated();
    void messageReceived(const QString &sender, const QString &message);
    void incomingCall(QString caller, QString caller_number, QString call_path);
    void activeDeviceChanged();
    void pairedDevicesChanged();

    void action(QString id, QVariant message);

    void start() override;
    void stop() override;
    void prevTrack() override;
    void nextTrack() override;
    void setMediaVolume(uint8_t volume) override;

    void playbackStarted() override;

public slots:
    void enablePairing();
    void disablePairing();
    void connectToDevice(QString ubi);

    void mediaPlaybackStarted();

    void eventMessage(QString id, QVariant message) override;
    void actionMessage(QString id, QVariant message) override;

private slots:
    void initOfono(QString ubi);
    void initBluez (BluezQt::InitManagerJob *job);
    void initObex (BluezQt::InitObexManagerJob *job);

    void showOverlay();
    void hideOverlay();
    void pullCallHistory();
    void deviceAdded(BluezQt::DevicePtr device);
    void deviceRemoved(BluezQt::DevicePtr device);
    void deviceDiscoveringChanged(bool discovering);
    void deviceConnectionChanged(bool connected);
    void devicePairedChanged(bool paired);
    void connectToDeviceCallback(BluezQt::PendingCall *call);

    void settingsChanged(const QString &key, const QVariant &value);

    void contactsFolderChanged(const QString &path);
    void contactsChanged(const QString &path);
private:
    QString m_contactsFolder = "contacts/";

    BluezQt::Manager m_bluez_manager;
    BluezQt::Device* m_activeDevice = nullptr;
    BluezQt::Device* m_previouslyTriedDevice = nullptr;
    BluezQt::Adapter *m_bluez_adapter = nullptr;
    BluezQt::ObexManager m_obexManager;
    BluezQt::Agent *bluez_agent;

    QFileSystemWatcher m_phonebookWatcher;
    bool m_androidAutoConnected = false;

    void getPhonebooks(QString destination, bool callHistoryOnly = false);
    void setBluezDevice(BluezQt::Device* device);
    void initAdapter(BluezQt::AdapterPtr adapter);
    void pullPhonebook (QString path, QString type, QString output);

    QVariantMap m_pairedDevices;
    QStringList m_adapters;
    void updateAdapters();
    void connectToNextDevice();

    OfonoManager m_ofonoManagerClass;
    PhonebookModel m_phonebookModel;
    PhonebookModel m_callHistoryModel;
};

#endif // TelephonyManager_H
