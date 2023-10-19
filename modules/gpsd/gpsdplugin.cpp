#include "gpsdplugin.h"
#define _USE_MATH_DEFINES
#include <math.h>

const int m_fence = 0; //only one fence
double fences[1][4][2]; // 1 Fence, 4 Points = 2 Measurements (Lat/Lon)

GPSDPlugin::GPSDPlugin(QObject *parent) : QObject (parent)
{
    //m_pluginSettings.events = QStringList() << "MediaInput::position";
}

void GPSDPlugin::init() {
    startWorker();
}

void GPSDPlugin::handleMode(const int& result) {
    if(this->m_mode != result) {
        this->m_mode = result;
        emit modeUpdated();
        emit message("Mode", this->m_mode);
    }
}

void GPSDPlugin::handleLocation(const double& lat, const double& lon, const bool& inFence) {
    if((this->m_latitude != lat || this->m_longitude != lon) && this->m_mode >= MODE_2D) {
        this->m_latitude = lat;
        this->m_longitude = lon;
        emit latitudeUpdated();
        emit longitudeUpdated();

        QVariantMap location;
        location.insert("latitude", this->m_latitude);
        location.insert("longitude", this->m_longitude);
        location.insert("track", this->m_track);
        location.insert("speed", this->m_speed);
        location.insert("altitude", this->m_altitude);
        location.insert("epx", this->m_epx);
        location.insert("epy", this->m_epy);
        location.insert("eph", this->m_eph);
        emit message("Location", location);

        if(this->m_inFence != inFence) {
            this->m_inFence = inFence;
            emit inFenceUpdated();
            emit message("InFence", inFence);
        }
    }
}

void GPSDPlugin::handleHERR(const double& epx, const double &epy, const double &eph) {
    if(this->m_epx != epx) {
        this->m_epx = epx;
        emit epxUpdated();
    }
    if(this->m_epy != epy) {
        this->m_epy = epy;
        emit epyUpdated();
    }
    if(this->m_eph != eph) {
        this->m_eph = eph;
        emit ephUpdated();
    }
}

void GPSDPlugin::handleTrack(const double& track) {
    if(this->m_track != track) {
        this->m_track = track;
        emit trackUpdated();
    }
}

void GPSDPlugin::handleSpeed(const double& speed) {
    if(this->m_speed != speed) {
        this->m_speed = speed;
        emit speedUpdated();
    }
}

void GPSDPlugin::handleAltitude(const double& altitude) {
    if(this->m_altitude != altitude) {
        this->m_altitude = altitude;
        emit altitudeUpdated();
    }
}

QObject *GPSDPlugin::getContextProperty(){
    return this;
}

void GPSDPlugin::eventMessage(QString id, QVariant message) {
}

GPSDPlugin::~GPSDPlugin() {
    stopWorker();
}

void GPSDPlugin::stopWorker() {
    workerThread.quit();
    workerThread.requestInterruption();
    workerThread.wait();
}

void GPSDPlugin::startWorker() {
    m_host  = m_settings.value("host").toString();
    m_port  = m_settings.value("port").toUInt();

    if (m_fence >= 0) {
        //Get Fence Points
        int fenceSize = sizeof(fences[m_fence])/sizeof(fences[m_fence][0]);
        char buffer[11];
        for(int i = 0; i < fenceSize; i++) {
            snprintf(buffer, 11, "fence%dpt%d", m_fence + 1, i + 1);
            QString pt = m_settings.value(buffer).toString();
            QStringList pieces = pt.split(",");
            if(pieces.length() == 2) {
                fences[m_fence][i][0] = pieces.value(0).toDouble();
                fences[m_fence][i][1] = pieces.value(1).toDouble();
            } else {
                fences[m_fence][i][0] = -91; //invalid latitude
                fences[m_fence][i][1] = -181; //invalid longitude
            }
        }
    }

    //Run the data colleciton in another thread
    GPSDWorker* worker = new GPSDWorker;
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &GPSDPlugin::operate, worker, &GPSDWorker::connect);
    connect(worker, &GPSDWorker::mode, this, &GPSDPlugin::handleMode);
    connect(worker, &GPSDWorker::location, this, &GPSDPlugin::handleLocation);
    connect(worker, &GPSDWorker::track, this, &GPSDPlugin::handleTrack);
    connect(worker, &GPSDWorker::speed, this, &GPSDPlugin::handleSpeed);
    connect(worker, &GPSDWorker::altitude, this, &GPSDPlugin::handleAltitude);
    connect(worker, &GPSDWorker::herr, this, &GPSDPlugin::handleHERR);
    workerThread.start();
    operate(m_host, m_port, m_fence);
    qDebug() << "GPSD: Started worker thread: " << workerThread.isRunning();
}

void GPSDPlugin::settingsChanged(const QString &key, const QVariant &){
    stopWorker();
    startWorker();
    if(key == "host"){
    }
    else if (key == "port") {
    }
}

void GPSDPlugin::PrintString(const char *message) {
    qDebug() << "GPSD DEBUG : " << message;
}

bool GPSDWorker::pointInPolygon(double lat, double lon) {
    if (m_fence < 0) return false;

    int fenceSize = sizeof(fences[m_fence])/sizeof(fences[m_fence][0]);
    double vectors[fenceSize][2];

    for(int i = 0; i < fenceSize; i++){
        if(fences[m_fence][i][0] < -90 || fences[m_fence][i][1] < -180) {
            fenceSize = i;
            break;
        }
        vectors[i][0] = fences[m_fence][i][0] - lat;
        vectors[i][1] = fences[m_fence][i][1] - lon;
    }

    double angle = 0;
    double num, den;

    for(int i = 0; i < fenceSize; i++){
        num = (vectors[i%fenceSize][0])*(vectors[(i+1)%fenceSize][0])+ (vectors[i%fenceSize][1])*(vectors[(i+1)%fenceSize][1]);
        den = (sqrt(pow(vectors[i%fenceSize][0],2) + pow(vectors[i%fenceSize][1],2)))*(sqrt(pow(vectors[(i+1)%fenceSize][0],2) + pow(vectors[(i+1)%fenceSize][1],2)));
        angle = angle + (180*acos(num/den)/M_PI);
    }

    return (angle > 355 && angle < 365);
}

void GPSDWorker::procData(struct gps_data_t * gps) {
    if (gps->set & MODE_SET) {
        emit mode(gps->fix.mode);
        if(gps->fix.mode >= MODE_2D && gps->set & LATLON_SET)
            emit location(gps->fix.latitude, gps->fix.longitude, pointInPolygon(gps->fix.latitude, gps->fix.longitude));
    }

    if(gps->set & TRACK_SET)
        emit track(gps->fix.track);

    if(gps->set & ALTITUDE_SET)
        emit altitude(gps->fix.altitude);

     if(gps->set & SPEED_SET)
        emit speed(gps->fix.speed);

    if(gps->set & HERR_SET)
        emit herr(gps->fix.epx, gps->fix.epy, gps->fix.eph);

}

void GPSDWorker::getData(std::string host, uint32_t port) {
    gpsmm gps_rec(host.c_str(), DEFAULT_GPSD_PORT);

    if(port > 0 && port <= 65535) {
        char buffer[6];
        snprintf(buffer, 6, "%d", port);
        gpsmm gps_rec(host.c_str(), buffer);
    }

    if (gps_rec.stream(WATCH_ENABLE|WATCH_JSON) == NULL) {
        qDebug() << "GPSD: No GPSD running.\n";
        return;
    }

    while(stopClient == false && QThread::currentThread()->isInterruptionRequested() == false) {
        struct gps_data_t* newdata;

        if (!gps_rec.waiting(5000000))
            continue;

        if ((newdata = gps_rec.read()) == NULL) {
            qDebug() << "GPSD: Read error\n";
            QThread::sleep(1);
        } else {
            procData(newdata);
        }
    }
}

void GPSDWorker::connect(QString host, quint32 port) {
    stopClient = false;
    getData(host.toStdString(), port);
}

void GPSDWorker::disconnect() {
    stopClient = true;
}

GPSDWorker::~GPSDWorker() {
    stopClient = true;
}
