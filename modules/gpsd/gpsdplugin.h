#ifndef GPSDPLUGIN_H
#define GPSDPLUGIN_H

#include <QObject>
#include <QStringList>
#include <QDebug>
#include <QByteArray>
#include <QThread>
#include <QCoreApplication>
#include <plugininterface.h>

#include "libgpsmm.h"

using namespace std;

class GPSDPlugin : public QObject, PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.aselafernando.headunit.gpsd" FILE "config.json")
    Q_INTERFACES(PluginInterface)

    //Q_PROPERTY(bool connected MEMBER m_connected NOTIFY connectedUpdated)
    Q_PROPERTY(int mode MEMBER m_mode NOTIFY modeUpdated)
    Q_PROPERTY(double longitude MEMBER m_longitude NOTIFY longitudeUpdated)
    Q_PROPERTY(double latitude MEMBER m_latitude NOTIFY latitudeUpdated)


public:
    explicit GPSDPlugin(QObject *parent = nullptr);
    ~GPSDPlugin();

    void init() override;
    QObject *getContextProperty() override;
    void PrintString(const char * message);

public slots:
    void eventMessage(QString id, QVariant message) override;
    void handleMode(const int& result);
    void handleLocation(const double& lat, const double& lon);

signals:
    void message(QString id, QVariant message);
    //void connectedUpdated();
    void action(QString id, QVariant message);
    void operate();
    void modeUpdated();
    void longitudeUpdated();
    void latitudeUpdated();

private slots:
    void settingsChanged(const QString &key, const QVariant &value);

private:
    bool m_connected;
    QString m_host = "";
    quint32 m_port = 0;
    QThread workerThread;
    int m_mode = 0;
    double m_latitude = 0;
    double m_longitude = 0;
};

class GPSDWorker : public QObject {
    Q_OBJECT

private:
    void getData();
    void procData(struct gps_data_t * gps);
    bool stopClient = false;
    ~GPSDWorker();

public slots:
    void connect();
    void disconnect();

signals:
    void mode(const int& result);
    void location(const double& latitude, const double& longitude);
};


#endif // GPSDPLUGIN_H
