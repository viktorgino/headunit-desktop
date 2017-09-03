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
    Q_PROPERTY(QVariantList connectedDevices READ getConnectedDevices NOTIFY deviceListChanged)
    Q_PROPERTY(int btConnectionState READ getBtConnectionState WRITE setBtConnectionState NOTIFY bluetoothStateChanged)
public:
    explicit TelephonyManager(QObject *parent = nullptr);
    ~TelephonyManager();
    QVariantList getConnectedDevices();
    void setBtConnectionState(const int state);
    int getBtConnectionState();
    Q_INVOKABLE void connectToBtDevice(QString address);
    Q_INVOKABLE void forgetBtDevice(QString address);
    Q_INVOKABLE void disconnectBtDevice(QString address);

signals:
    void messageReceived(const QString &sender, const QString &message);
    void incomingCall(QString caller, QString caller_number, QString call_path);
    void deviceListChanged();
    void bluetoothStateChanged();

public slots:
    void answerCall(QString call_path);
    void declineCall(QString call_path);
    void vCardImportReady(const QString vcardData);
    void vCardImportFailed();
    void vCardImportChanged();

private slots:
    void deviceConnected(const QBluetoothAddress &address);
    void deviceDisconnected(const QBluetoothAddress &address);
    void pairingDisplayPinCode(const QBluetoothAddress &address, QString pin);
    void initHFP(bool available);
    void initHFP2(bool available);
    void callAdded(const QString &call);
    void voiceCallStateChanged(const QString &state);
    void btStateChanged(QBluetoothLocalDevice::HostMode state);
    void pairingFinished(const QBluetoothAddress &address);

private:
    QBluetoothServiceInfo serviceInfo;
    QList<QBluetoothSocket *> clientSockets;
    QBluetoothLocalDevice *localAdapter;
    void loadAddressBook();
    void setupService();

    QOfonoModem *modem;
    QOfonoManager *manager;
    QOfonoPhonebook *phonebook;
    QOfonoVoiceCallManager voiceCallManager;
    QOfonoVoiceCall *voiceCall;
    QString modemPath = "";
};

#endif // TelephonyManager_H
