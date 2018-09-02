#include "odbcar.h"

OdbCar::OdbCar(QObject *parent) : QObject (parent)
{

}

QObject *OdbCar::getContextProperty(){
    return nullptr;
}

QStringList OdbCar::eventListeners(){
    return QStringList();
}

QQuickImageProvider *OdbCar::getImageProvider() {
    return nullptr;
}

QStringList OdbCar::events(){
    return QStringList();
}

QStringList OdbCar::actions() {
    return QStringList();
}

void OdbCar::eventMessage(QString id, QString message){

}

void OdbCar::actionMessage(QString id, QString message){

}
