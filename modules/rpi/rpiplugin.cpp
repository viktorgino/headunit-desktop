#include "rpiplugin.h"
#include <QFileInfo>
#include <QFile>
#include <QJsonValue>

#define PROP_NAME "rpiplugin_brightness"
#define RPI_BACKLIGHT_PATH "/sys/class/backlight/rpi_backlight/brightness" 

RPiPlugin::RPiPlugin(QObject *parent) : QObject (parent)
{
    initialized = false;
    QFileInfo check_file(RPI_BACKLIGHT_PATH);
    if (check_file.exists() && check_file.isFile()) {
	    connect(&settings,&QQmlPropertyMap::valueChanged, this, &RPiPlugin::settingChanged);
	    initialized = true;
    } 
}

QObject *RPiPlugin::getContextProperty(){
    return new QObject();
}

QQuickImageProvider *RPiPlugin::getImageProvider() {
    return nullptr;
}

QStringList RPiPlugin::eventListeners(){
    return QStringList();
}

QStringList RPiPlugin::events(){
    return QStringList();
}

QStringList RPiPlugin::actions() {
    return QStringList();
}

void RPiPlugin::settingChanged(QString id, QVariant val){
    if (!initialized) return;

    if (id == PROP_NAME) {
    	QFile file(RPI_BACKLIGHT_PATH);
        if (file.open(QIODevice::WriteOnly | QIODevice::ExistingOnly | QIODevice::Text)) {
		QTextStream stream(&file);
	   	qDebug() << "applying setting: " << id << " " << val.toDouble();
		stream << val.toDouble();
		file.close();
	}	
    }
}

void RPiPlugin::eventMessage(QString id, QString message){
	qDebug() << "eventMessage: " << id << " " << message; 
}

void RPiPlugin::actionMessage(QString id, QString message){
	qDebug() << "actionMessage: " << id << " " << message; 
}
