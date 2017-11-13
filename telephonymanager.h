#ifndef TelephonyManager_H

#include <QObject>
#include "qofonomanager.h"
#include "qofonophonebook.h"
#include "qofonovoicecallmanager.h"
#include "qofonovoicecall.h"
#include "qofonomodem.h"
#include <BluezQt/Manager>
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

class TelephonyManager : public QObject
{
    Q_OBJECT
public:
    explicit TelephonyManager(QObject *parent = nullptr);
    ~TelephonyManager();
    Q_INVOKABLE void getPhonebooks(QString destination);

Q_SIGNALS:
    void messageReceived(const QString &sender, const QString &message);
    void incomingCall(QString caller, QString caller_number, QString call_path);
    void deviceListChanged();
    void phonebookChanged(QString path);

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
private:
    QOfonoModem *modem;
    QOfonoManager *manager;
    QOfonoPhonebook *phonebook;
    QOfonoVoiceCallManager voiceCallManager;
    QOfonoVoiceCall *voiceCall;
    QString modemPath = "";
    QString contactsFolder = "contacts/";

    BluezQt::ObexManager* obexManager;
    BluezQt::ObexAgent* obexAgent;
    BluezQt::PendingCall* obexSessionRegisterPC;
    BluezQt::ObexSessionPtr pbapSession;

    void pullPhonebooks();
};

#endif // TelephonyManager_H
