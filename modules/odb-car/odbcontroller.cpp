#include "odbcontroller.h"

OdbController::OdbController(QObject *parent)
{
    settings.beginGroup("odbsettings");
    if(settings.contains("type")){
        if(settings.value("type") == "elm327"){
            settings.beginGroup("elm327");
            if(settings.value("type") == "bluetooth"){
                //load bt elm327 controller
            } else if(settings.value("type") == "serial"){
                //load serial elm327 controller
            }
            settings.endGroup();
        }
    }
}
