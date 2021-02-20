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
        connect(&m_settings,&QQmlPropertyMap::valueChanged, this, &RPiPlugin::settingChanged);
	    initialized = true;
    } else {
	    qDebug() << "rpiplugin not initializing.";
    } 
}
void RPiPlugin::init(){

}
// maps slider value (0-100) to (0-200)
int RPiPlugin::mapBrightness(double v) {
	return v*200/100;
}

void RPiPlugin::onLoad() {
    if (m_settings.contains(PROP_NAME)) {
        applyBrightness(m_settings.value(PROP_NAME).toJsonValue().toString().toDouble());
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

