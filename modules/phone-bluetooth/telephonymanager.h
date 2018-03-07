#ifndef TelephonyManager_H

#include <QObject>
#include "qofonomanager.h"
#include "qofonophonebook.h"
#include "qofonovoicecallmanager.h"
#include "qofonovoicecall.h"
#include "qofonomodem.h"
#include <BluezQt/Manager>
#include <BluezQt/Agent>
#include <BluezQt/Device>
#include <BluezQt/MediaPlayer>
#include <BluezQt/ObexManager>
#include <BluezQt/ObexSession>
#include <BluezQt/PendingCall>
#include <BluezQt/InitManagerJob>
#include <BluezQt/InitObexManagerJob>
#include <BluezQt/ObexAgent>
#include <BluezQt/ObexFileTransfer>
#include <BluezQt/ObexFileTransferEntry>
#include "phonebookaccess1_interface.h"
#include <QStandardPaths>
#include <QLoggingCategory>

#include "bluezagent.h"

class TelephonyManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString contactsFolder READ getContactsFolder NOTIFY contactsFolderChanged)
    Q_PROPERTY(int deviceIndex READ getDeviceIndex NOTIFY deviceIndexChanged)
public:
    explicit TelephonyManager(QObject *parent = nullptr);
    ~TelephonyManager();

    QString getContactsFolder() const {
        return m_contactsFolder;
    }

    int getDeviceIndex(){
        return m_deviceIndex;
    }
Q_SIGNALS:
    void messageReceived(const QString &sender, const QString &message);
    void incomingCall(QString caller, QString caller_number, QString call_path);
    void phonebookChanged();
    void contactsFolderChanged();
    void deviceIndexChanged();

public slots:
    void answerCall(QString call_path);
    void declineCall(QString call_path);

private slots:
    void initHFP(bool available);
    void initHFP2(bool available);
    void callAdded(const QString &call);
    void voiceCallStateChanged(const QString &state);

    void obexManagerStartResult (BluezQt::InitObexManagerJob *job);
    void bluezManagerStartResult (BluezQt::InitManagerJob *job);
    void obexSessionAdded (BluezQt::ObexSessionPtr session);
    void deviceAdded(BluezQt::DevicePtr device);
    void deviceRemoved(BluezQt::DevicePtr device);
    void deviceChanged(BluezQt::DevicePtr device);

private:
    QOfonoModem *modem;
    QOfonoManager *manager;
    QOfonoPhonebook *phonebook;
    QOfonoVoiceCallManager voiceCallManager;
    QOfonoVoiceCall *voiceCall;
    QString modemPath = "";
    QString m_contactsFolder = "contacts/";
    int m_deviceIndex = -1;

    BluezQt::Manager *bluez_manager;
    BluezQt::Device *m_activeDevice = NULL;
    BluezQt::ObexManager* obexManager;
    BluezQt::ObexAgent* obexAgent;
    BluezQt::PendingCall* obexSessionRegisterPC;
    BluezQt::ObexSessionPtr pbapSession;
    BluezQt::Agent *bluez_agent;

    void pullPhonebooks();
    void getPhonebooks(QString destination);
    void setBluezDevice(BluezQt::Device* device);
};

#endif // TelephonyManager_H
