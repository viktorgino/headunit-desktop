#ifndef TelephonyManager_H
#define TelephonyManager_H

#include <QObject>
#include <QtBluetooth>
#include <QBluetoothLocalDevice>
#include <QBluetoothServer>
#include <QBluetoothAddress>
#include "QVcf/vcfreader.h"
#include "qofonomanager.h"
#include "qofonophonebook.h"
#include "qofonovoicecallmanager.h"
#include "qofonovoicecall.h"

class TelephonyManager : public QObject
{
    Q_OBJECT
public:
    explicit TelephonyManager(QObject *parent = nullptr);
    ~TelephonyManager();

signals:
    void messageReceived(const QString &sender, const QString &message);
    void incomingCall(QString caller, QString caller_number, QString call_path);

public slots:
    void answerCall(QString call_path);
    void declineCall(QString call_path);

private slots:
    void deviceConnected(const QBluetoothAddress &address);
    void deviceDisconnected(const QBluetoothAddress &address);
    void pairingDisplayPinCode(const QBluetoothAddress &address, QString pin);
    void vCardImportReady(const QString vcardData);
    void initHFP(bool available);
    void callAdded(const QString &call);
    void voiceCallStateChanged(const QString &state);

private:
    QBluetoothServiceInfo serviceInfo;
    QList<QBluetoothSocket *> clientSockets;
    QBluetoothLocalDevice *localAdapter;
    void loadAddressBook();

    QOfonoManager manager;
    QOfonoPhonebook phonebook;
    QOfonoVoiceCallManager voiceCallManager;
    QOfonoVoiceCall *voiceCall;
    QString modemPath = "";
};

#endif // TelephonyManager_H
