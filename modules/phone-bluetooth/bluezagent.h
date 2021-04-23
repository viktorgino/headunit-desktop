#ifndef BLUEZAGENT_H
#define BLUEZAGENT_H

#include <QObject>
#include <BluezQt/Device>
#include <BluezQt/Agent>
#include <BluezQt/PendingCall>
#include <BluezQt/Manager>
#include <QDBusObjectPath>
#include <QDebug>

class BluezAgent : public BluezQt::Agent
{
    Q_OBJECT

public:
    explicit BluezAgent(QObject *parent = Q_NULLPTR);


    void authorizeService(BluezQt::DevicePtr device, const QString &uuid, const BluezQt::Request<> &request) Q_DECL_OVERRIDE;
    void cancel() Q_DECL_OVERRIDE;
    BluezQt::Agent::Capability capability() const Q_DECL_OVERRIDE;
    void displayPasskey(BluezQt::DevicePtr device, const QString &passkey, const QString &entered) Q_DECL_OVERRIDE;
    void displayPinCode(BluezQt::DevicePtr device, const QString &pinCode) Q_DECL_OVERRIDE;
    QDBusObjectPath objectPath() const Q_DECL_OVERRIDE;
    void release() Q_DECL_OVERRIDE;
    void requestAuthorization(BluezQt::DevicePtr device, const BluezQt::Request<> &request) Q_DECL_OVERRIDE;
    void requestConfirmation(BluezQt::DevicePtr device, const QString &passkey, const BluezQt::Request<> &request) Q_DECL_OVERRIDE;
    void requestPasskey(BluezQt::DevicePtr device, const BluezQt::Request<quint32> &request) Q_DECL_OVERRIDE;
    void requestPinCode(BluezQt::DevicePtr device, const BluezQt::Request<QString> &request) Q_DECL_OVERRIDE;

    BluezQt::DevicePtr m_device;

    // requestPinCode
    bool m_pinRequested;

    // displayPinCode
    QString m_displayedPinCode;

    // requestPasskey
    bool m_passkeyRequested;

    // displayPasskey
    QString m_displayedPasskey;
    QString m_enteredPasskey;

    // requestConfirmation
    QString m_requestedPasskey;

    // requestAuthorization
    bool m_authorizationRequested;

    // authorizeService
    QString m_authorizedUuid;

    // cancel
    bool m_cancelCalled;

    // release
    bool m_releaseCalled;
};

#endif // BLUEZAGENT_H
