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
    QQuickImageProvider *getImageProvider() override;
    QStringList eventListeners() override;
    QStringList events() override;
    QStringList actions() override;
private:
     TelephonyManager *telephonyManager;

signals:

public slots:
     void eventMessage(QString id, QString message) override;
     void actionMessage(QString id, QString message) override;
};

#endif // PHONEBLUETOOTH_H
