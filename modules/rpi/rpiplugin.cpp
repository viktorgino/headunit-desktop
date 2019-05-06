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
    } else {
	    qDebug() << "rpiplugin not initializing.";
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

// maps slider value (0-100) to (0-200)
int RPiPlugin::mapBrightness(double v) {
	return v*200/100;
}

void RPiPlugin::onLoad() {
	if (settings.contains(PROP_NAME)) {
		applyBrightness(settings[PROP_NAME].toJsonValue().toString().toDouble());
	}
}

void RPiPlugin::applyBrightness(double v) {
    if (!initialized) return;
    
    QFile file(RPI_BACKLIGHT_PATH);
    if (file.open(QIODevice::WriteOnly | QIODevice::ExistingOnly | QIODevice::Text)) {
        QTextStream stream(&file);
	stream << mapBrightness(v);
	file.close();
    }	
}

void RPiPlugin::settingChanged(QString id, QVariant val){
    if (id == PROP_NAME) applyBrightness(val.toDouble());
}

void RPiPlugin::eventMessage(QString id, QString message){
	qDebug() << "eventMessage: " << id << " " << message; 
}

void RPiPlugin::actionMessage(QString id, QString message){
	qDebug() << "actionMessage: " << id << " " << message; 
}
