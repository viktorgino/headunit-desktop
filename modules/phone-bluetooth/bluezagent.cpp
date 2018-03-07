#include "bluezagent.h"

BluezAgent::BluezAgent(QObject *parent)
    : Agent(parent)
    , m_pinRequested(false)
    , m_passkeyRequested(false)
    , m_authorizationRequested(false)
    , m_cancelCalled(false)
    , m_releaseCalled(false)
{
}

QDBusObjectPath BluezAgent::objectPath() const
{
    return QDBusObjectPath(QStringLiteral("/BluezAgent"));
}

void BluezAgent::requestPinCode(BluezQt::DevicePtr device, const BluezQt::Request<QString> &request)
{
    m_device = device;
    m_pinRequested = true;

    request.accept(QString());
    qDebug() << "requestPinCode";

}

void BluezAgent::displayPinCode(BluezQt::DevicePtr device, const QString &pinCode)
{
    m_device = device;
    m_displayedPinCode = pinCode;
    qDebug() << "displayPinCode: " << pinCode;
}

void BluezAgent::requestPasskey(BluezQt::DevicePtr device, const BluezQt::Request<quint32> &request)
{
    m_device = device;
    m_passkeyRequested = true;

    request.accept(0);
    qDebug() << "requestPasskey";
}

void BluezAgent::displayPasskey(BluezQt::DevicePtr device, const QString &passkey, const QString &entered)
{
    m_device = device;
    m_displayedPasskey = passkey;
    m_enteredPasskey = entered;
    qDebug() << "displayPasskey passkey : " << passkey << " entered: " << entered;
}

void BluezAgent::requestConfirmation(BluezQt::DevicePtr device, const QString &passkey, const BluezQt::Request<> &request)
{
    m_device = device;
    m_requestedPasskey = passkey;

    request.accept();
    qDebug() << "requestConfirmation: " << device << " : " << passkey;
}

void BluezAgent::requestAuthorization(BluezQt::DevicePtr device, const BluezQt::Request<> &request)
{
    m_device = device;
    m_authorizationRequested = true;

    request.accept();
    qDebug() << "requestAuthorization";
}

void BluezAgent::authorizeService(BluezQt::DevicePtr device, const QString &uuid, const BluezQt::Request<> &request)
{
    m_device = device;
    m_authorizedUuid = uuid;

    request.accept();
    qDebug() << "authorizeService";
}

void BluezAgent::cancel()
{
    m_cancelCalled = true;
    qDebug() << "cancel";
}

void BluezAgent::release()
{
    m_releaseCalled = true;
    qDebug() << "release";
}
