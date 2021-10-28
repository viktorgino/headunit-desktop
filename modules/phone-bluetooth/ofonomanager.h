#ifndef OFONOMANAGER_H
#define OFONOMANAGER_H

#include <QObject>
#include <QDebug>
#include <QDBusConnection>

#include <QDBusObjectPath>
#include <QDBusArgument>

#include "ofono_dbus_types.h"
#include "manager_interface.h"
#include "handsfree_interface.h"
#include "modem_interface.h"
#include "manager_interface.h"
#include "voicecall_interface.h"
#include "voicecallmanager_interface.h"
#include "networkregistration_interface.h"

class OfonoManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString callState READ getCallState NOTIFY callStateChanged)
    Q_PROPERTY(QString caller READ getCaller NOTIFY callerChanged)
    Q_PROPERTY(QString callTime READ getCallTime NOTIFY callTimeChanged)
    Q_PROPERTY(int batteryCharge READ getBatteryCharge NOTIFY batteryChargeChanged)
    Q_PROPERTY(int signalStrength READ getSignalStrength NOTIFY signalStrengthChanged)
    Q_PROPERTY(QString networkName READ getNetworkName NOTIFY networkNameChanged)
    Q_PROPERTY(bool online READ getOnline NOTIFY onlineChanged)

public:
    explicit OfonoManager(QObject *parent = nullptr);

    QStringList getModems();
    int setDefaultModem(QString path);
    int setModem(QString path);
    void init();

public slots:
    void answerCall();
    void hangupCall();
    void dialNumber(QString number);
    void activateVoiceControl();

signals:
    void showOverlay();
    void hideOverlay();
    void callStateChanged();
    void callerChanged();
    void callTimeChanged();
    void batteryChargeChanged();
    void signalStrengthChanged();
    void networkNameChanged();
    void onlineChanged();
    void callFinished();

private:
    QDBusConnection m_dbusConnection;
    org::ofono::Manager m_manager;
    QSharedPointer<org::ofono::Modem> m_modem;
    QSharedPointer<org::ofono::Handsfree> m_handsfree;
    QSharedPointer<org::ofono::VoiceCall> m_voicecall;
    QSharedPointer<org::ofono::VoiceCallManager> m_voicecallmanager;
    QSharedPointer<org::ofono::NetworkRegistration> m_networkregistration;

    QString m_deviceDefaultPath;
    QString m_activeDevicePath;

    QString m_callState;
    QString m_caller;
    QString m_callTime;
    int m_batteryCharge;
    int m_signalStrength;
    QString m_networkName;
    bool m_online;

    QString getCallState() {return m_callState;}
    QString getCaller() {return m_caller;}
    QString getCallTime() {return m_callTime;}
    int getBatteryCharge() {return m_batteryCharge;}
    int getSignalStrength() {return m_signalStrength;}
    QString getNetworkName() {return m_networkName; }
    bool getOnline() {return m_online;}

    void loadProperties();
    void disconnectDevice();
    void startCallTimer(QString time);
    void stopCallTimer();
    QTimer m_callTimeTimer;
    QDateTime m_callStartTime;

private slots :
    void CallAdded(const QDBusObjectPath &path, const QVariantMap &properties);
    void CallRemoved(const QDBusObjectPath &path);
    void CallPropertyChanged(const QString &property, const QDBusVariant &value);
    void ModemPropertyChanged(const QString &property, const QDBusVariant &value);
    void HandsfreePropertyChanged(const QString &property, const QDBusVariant &value);
    void NetworkRegistrationPropertyChanged(const QString &property, const QDBusVariant &value);
    void updateCallTime();
    void ModemAdded(const QDBusObjectPath &path, const QVariantMap &properties);
    void ModemRemoved(const QDBusObjectPath &path);
};

#endif // OFONOMANAGER_H
