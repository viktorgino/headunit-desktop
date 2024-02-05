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
    Q_PROPERTY(bool inFence MEMBER m_inFence NOTIFY inFenceUpdated)
    Q_PROPERTY(double track MEMBER m_track NOTIFY trackUpdated)
    Q_PROPERTY(double speed MEMBER m_speed NOTIFY speedUpdated)
    Q_PROPERTY(double altitude MEMBER m_altitude NOTIFY altitudeUpdated)
    Q_PROPERTY(double epx MEMBER m_epx NOTIFY epxUpdated)
    Q_PROPERTY(double epy MEMBER m_epy NOTIFY epyUpdated)
    Q_PROPERTY(double eph MEMBER m_eph NOTIFY ephUpdated)

public:
    explicit GPSDPlugin(QObject *parent = nullptr);
    ~GPSDPlugin();

    void init() override;
    QObject *getContextProperty() override;
    void PrintString(const char * message);

public slots:
    void eventMessage(QString id, QVariant message) override;
    void handleMode(const int& result);
    void handleLocation(const double& lat, const double& lon, const bool& inFence);
    void handleTrack(const double& track);
    void handleSpeed(const double& speed);
    void handleAltitude(const double& altitude);
    void handleHERR(const double& epx, const double& epy, const double& eph);

signals:
    void message(QString id, QVariant message);
    void action(QString id, QVariant message);
    void operate(QString host, quint32 port, int fence);
    void modeUpdated();
    void longitudeUpdated();
    void latitudeUpdated();
    void inFenceUpdated();
    void trackUpdated();
    void speedUpdated();
    void altitudeUpdated();
    void epxUpdated();
    void epyUpdated();
    void ephUpdated();

private slots:
    void settingsChanged(const QString &key, const QVariant &value);

private:
    QString m_host = "";
    quint32 m_port = 0;
    QThread workerThread;
    int m_mode = 0;
    double m_latitude = -91;
    double m_longitude = -181;
    bool m_inFence = false;
    double m_track = 0;
    double m_speed = 0;
    double m_altitude = 0;
    double m_epx = 0;
    double m_epy = 0;
    double m_eph = 0;
    void stopWorker();
    void startWorker();

};

class GPSDWorker : public QObject {
    Q_OBJECT

private:
    void getData(std::string host, uint32_t port);
    void procData(struct gps_data_t * gps);
    bool stopClient = false;
    bool pointInPolygon(double lat, double lon);
    ~GPSDWorker();

public slots:
    void connect(QString host, quint32 port);
    void disconnect();

signals:
    void mode(const int& result);
    void location(const double& latitude, const double& longitude, const bool& inFence);
    void track(const double& track);
    void speed(const double& speed);
    void altitude(const double& altitude);
    void herr(const double& epx, const double& epy, const double& eph);
};


#endif // GPSDPLUGIN_H
