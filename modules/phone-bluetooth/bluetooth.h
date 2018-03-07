#ifndef PHONEBLUETOOTH_H
#define PHONEBLUETOOTH_H

#include <QObject>
#include <plugininterface.h>
#include "telephonymanager.h"

class PhoneBluetooth : public QObject, PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.viktorgino.headunit.phonebluetooth" FILE "config.json")
    Q_INTERFACES(PluginInterface)
public:
    explicit PhoneBluetooth(QObject *parent = nullptr);
    QObject *getContextProperty() override;
private:
     TelephonyManager *telephonyManager;

signals:

public slots:
};

#endif // PHONEBLUETOOTH_H
