#ifndef GPSDPLUGIN_H
#define GPSDPLUGIN_H

#include <QObject>
#include <QStringList>
#include <QDebug>
#include <QByteArray>
#include <QCoreApplication>
#include <plugininterface.h>

class GPSDPlugin : public QObject, PluginInterface, PlatformCallbacks
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.aselafernando.headunit.gpsd" FILE "config.json")
    Q_INTERFACES(PluginInterface)

    Q_PROPERTY(bool connected MEMBER m_connected NOTIFY connectedUpdated)

public:
    explicit GPSDPlugin(QObject *parent = nullptr);

    void init() override;
    QObject *getContextProperty() override;
    void PrintString(char * message, int length) override;

public slots:
    void eventMessage(QString id, QVariant message) override;

signals:
    void message(QString id, QVariant message);
    void connectedUpdated();
    void action(QString id, QVariant message);

private slots:
    void settingsChanged(const QString &key, const QVariant &value);

private:
    bool m_connected;
};

#endif // GPSDPLUGIN_H
