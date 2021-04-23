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
    return QDBusObjectPath(QStringLiteral("/HUDBluezAgent"));
}

void BluezAgent::requestPinCode(BluezQt::DevicePtr device, const BluezQt::Request<QString> &request)
{
    qDebug() << "requestPinCode";
    m_device = device;
    m_pinRequested = true;

    request.accept(QString());

}

void BluezAgent::displayPinCode(BluezQt::DevicePtr device, const QString &pinCode)
{
    qDebug() << "displayPinCode: " << pinCode;
    m_device = device;
    m_displayedPinCode = pinCode;
}

void BluezAgent::requestPasskey(BluezQt::DevicePtr device, const BluezQt::Request<quint32> &request)
{
    qDebug() << "requestPasskey";
    m_device = device;
    m_passkeyRequested = true;

    request.accept(0);
}

void BluezAgent::displayPasskey(BluezQt::DevicePtr device, const QString &passkey, const QString &entered)
{
    qDebug() << "displayPasskey passkey : " << passkey << " entered: " << entered;
    m_device = device;
    m_displayedPasskey = passkey;
    m_enteredPasskey = entered;
}

void BluezAgent::requestConfirmation(BluezQt::DevicePtr device, const QString &passkey, const BluezQt::Request<> &request)
{
    qDebug() << "requestConfirmation: " << device << " : " << passkey;
    m_device = device;
    m_requestedPasskey = passkey;

    request.accept();
}

void BluezAgent::requestAuthorization(BluezQt::DevicePtr device, const BluezQt::Request<> &request)
{
    qDebug() << "requestAuthorization";
    m_device = device;
    m_authorizationRequested = true;

    request.accept();
}

void BluezAgent::authorizeService(BluezQt::DevicePtr device, const QString &uuid, const BluezQt::Request<> &request)
{
  qDebug() << "authorizeService " << uuid;
    m_device = device;
    m_authorizedUuid = uuid;

    request.accept();
    device->setTrusted(true);
//    device->connectToDevice();
}

void BluezAgent::cancel()
{
    qDebug() << "cancel";
    m_cancelCalled = true;
}

void BluezAgent::release()
{
    qDebug() << "release";
    m_releaseCalled = true;
}


BluezQt::Agent::Capability BluezAgent::capability() const{
  return DisplayOnly;
}
