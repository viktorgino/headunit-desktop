#include "gpsdplugin.h"
#include <math.h>

GPSDPlugin::GPSDPlugin(QObject *parent) : QObject (parent)
{
    //m_pluginSettings.events = QStringList() << "MediaInput::position";
}

void GPSDPlugin::init() {
    m_host = m_settings.value("host").toString();
    m_port = m_settings.value("port").toUInt();
    //Run the data colleciton in another thread
    GPSDWorker* worker = new GPSDWorker;
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &GPSDPlugin::operate, worker, &GPSDWorker::connect);
    connect(worker, &GPSDWorker::mode, this, &GPSDPlugin::handleMode);
    connect(worker, &GPSDWorker::location, this, &GPSDPlugin::handleLocation);
    workerThread.start();
    operate();
    qDebug() << "GPSD: Started worker thread: " << workerThread.isRunning();
}

void GPSDPlugin::handleMode(const int& result) {
    this->m_mode = result;
    emit modeUpdated();
}

void GPSDPlugin::handleLocation(const double& lat, const double& lon) {
    this->m_latitude = lat;
    this->m_longitude = lon;
    emit latitudeUpdated();
    emit longitudeUpdated();
}

QObject *GPSDPlugin::getContextProperty(){
    return this;
}

void GPSDPlugin::eventMessage(QString id, QVariant message) {
}

GPSDPlugin::~GPSDPlugin() {
    workerThread.quit();
    workerThread.wait();
}

void GPSDPlugin::settingsChanged(const QString &key, const QVariant &){
    if(key == "host"){
    }
    else if (key == "port") {
    }
}

void GPSDPlugin::PrintString(const char *message) {
    qDebug() << "GPSD DEBUG : " << message;
}

void GPSDWorker::procData(struct gps_data_t * gps) {
        if (gps->set & MODE_SET) {
            emit mode(gps->fix.mode);
            if(gps->fix.mode == 3 && gps->set & LATLON_SET) {
                emit location(gps->fix.latitude, gps->fix.longitude);
            }
        }
}

void GPSDWorker::getData() {
    gpsmm gps_rec("localhost", DEFAULT_GPSD_PORT);

    if (gps_rec.stream(WATCH_ENABLE|WATCH_JSON) == NULL) {
        qDebug() << "No GPSD running.\n";
        return;
    }

    while(stopClient == false) {
        struct gps_data_t* newdata;

        if (!gps_rec.waiting(5000000))
            continue;

        if ((newdata = gps_rec.read()) == NULL) {
            qDebug() << "Read error.\n";
            return;
        } else {
            procData(newdata);
        }
    }
}

void GPSDWorker::connect() {
    stopClient = false;
    getData();
}

void GPSDWorker::disconnect() {
    stopClient = true;
}

GPSDWorker::~GPSDWorker() {
    stopClient = true;
}
