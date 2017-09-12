#ifndef TelephonyManager_H
#define TelephonyManager_H

#include <QObject>
#include <QtBluetooth>
#include <QBluetoothLocalDevice>
#include <QBluetoothServer>
#include <QBluetoothAddress>
#include <QIODevice>
#include "QVcf/vcfreader.h"
#include "qofonomanager.h"
#include "qofonophonebook.h"
#include "qofonovoicecallmanager.h"
#include "qofonovoicecall.h"
#include "qofonomodem.h"

class TelephonyManager : public QObject
{
    Q_OBJECT
public:
    explicit TelephonyManager(QObject *parent = nullptr);
    ~TelephonyManager();

Q_SIGNALS:
    void messageReceived(const QString &sender, const QString &message);
    void incomingCall(QString caller, QString caller_number, QString call_path);
    void deviceListChanged();

public slots:
    void answerCall(QString call_path);
    void declineCall(QString call_path);

private slots:
    void deviceConnected(const QBluetoothAddress &address);
    void deviceDisconnected(const QBluetoothAddress &address);
    void pairingDisplayPinCode(const QBluetoothAddress &address, QString pin);
    void initHFP(bool available);
    void initHFP2(bool available);
    void callAdded(const QString &call);
    void voiceCallStateChanged(const QString &state);
    void pairingFinished(const QBluetoothAddress &address);
private:
    QBluetoothServiceInfo serviceInfo;
    QList<QBluetoothSocket *> clientSockets;
    QBluetoothLocalDevice *localAdapter = NULL;

    void setupService();
    QOfonoModem *modem;
    QOfonoManager *manager;
    QOfonoPhonebook *phonebook;
    QOfonoVoiceCallManager voiceCallManager;
    QOfonoVoiceCall *voiceCall;
    QString modemPath = "";
};

#endif // TelephonyManager_H
