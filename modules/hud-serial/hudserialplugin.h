#ifndef HUDSERIALPLUGIN_H
#define HUDSERIALPLUGIN_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QStringList>
#include <QDebug>
#include <QByteArray>
#include <plugininterface.h>
#include <hvac-common.h>
#include "CRL/HUDSerial/HUDSerial.h"
#include "CRL/common.h"


class HUDSerialPlugin : public QObject, PluginInterface, PlatformCallbacks
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.viktorgino.headunit.hudserial" FILE "config.json")
    Q_INTERFACES(PluginInterface)

    Q_PROPERTY(QVariantMap ports READ getPorts NOTIFY portsUpdated)
    Q_PROPERTY(bool connected MEMBER m_connected NOTIFY connectedUpdated)
public:
    explicit HUDSerialPlugin(QObject *parent = nullptr);

    void init() override;
    QObject *getContextProperty() override;

    void ClimateControlCallback(const ClimateControlCommandFrame &controlFrame) override;
    void CustomCommandCallback(const CustomCommandFrame &commandFrame) override;
    void BodyControlCommandCallback(const BodyControlCommandFrame &controlFrame) override;
    void DriveTrainControlCommandCallback(const DriveTrainControlCommandFrame &controlFrame) override;
    void ButtonInputCommandCallback(Keys key) override;
    void SendMessageCallback(uint8_t sendByte) override;
    void PrintString(char * message, int length) override;

public slots:
    void eventMessage(QString id, QVariant message) override;
    void serialRestart();
signals:
    void portsUpdated();
    void message(QString id, QVariant message);
    void connectedUpdated();
    void action(QString id, QVariant message);
private slots:
    void handleSerialReadyRead();
    void handleSerialError(QSerialPort::SerialPortError error);
    void settingsChanged(const QString &key, const QVariant &value);
private:
    QSerialPort m_serial;
    QVariantMap m_ports;
    HUDSerial::HUDSerial m_serialProtocol;
    bool m_connected;

    void updatePorts();
    void serialConnect();
    void serialDisconnect();

    QVariantMap getPorts() {
        updatePorts();
        return m_ports;
    }
};

#endif // HUDSERIALPLUGIN_H
