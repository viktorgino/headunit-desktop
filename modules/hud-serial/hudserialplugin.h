#ifndef HUDSERIALPLUGIN_H
#define HUDSERIALPLUGIN_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QStringList>
#include <QDebug>
#include <QByteArray>
#include <QTimer>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QCoreApplication>
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
    Q_PROPERTY(QVariantMap carManufacturers READ carManufacturers NOTIFY carsUpdated)
    Q_PROPERTY(QVariantMap cars READ cars NOTIFY carsUpdated)
    Q_PROPERTY(QVariantList carSettings READ getCarSettings NOTIFY carSettingsUpdated)
    Q_PROPERTY(QVariantList customBits READ getCustomBits NOTIFY customCommandUpdated)
    Q_PROPERTY(QVariantList customBytes READ getCustomBytes NOTIFY customCommandUpdated)
    Q_PROPERTY(QVariantList speeds READ getSpeeds CONSTANT)
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
    void actionMessage(QString id, QVariant message) override;
    void serialRestart();
    void setCustomBit(uint bitNumber, bool value);
    void setCustomByte(uint byteNumber, uint value);
signals:
    void portsUpdated();
    void message(QString id, QVariant message);
    void connectedUpdated();
    void customCommandUpdated();
    void carsUpdated();
    void carSettingsUpdated();
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
    QTimer m_serialRetryTimer;
    QVariantMap m_manufacturers;
    QVariantMap m_cars;
    QVariantList m_carSettings;
    QVariantList m_customBits;
    QVariantList m_customBytes;
    QVariantList m_speeds;

    void updatePorts();
    void serialConnect();
    void serialDisconnect();

    void updateManufacturers();
    void updateCars();
    void loadCarSettings(QString fileName);

    QVariantMap getPorts() {
        updatePorts();
        return m_ports;
    }

    QVariantMap carManufacturers(){
        return m_manufacturers;
    }
    QVariantMap cars(){
        return m_cars;
    }
    QVariantList getCarSettings(){
        return m_carSettings;
    }
    QVariantList getCustomBits(){
        return m_customBits;
    }
    QVariantList getCustomBytes(){
        return m_customBytes;
    }
    QVariantList getSpeeds(){
        return m_speeds;
    }
};

#endif // HUDSERIALPLUGIN_H
