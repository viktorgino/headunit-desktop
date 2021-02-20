#include "hvaccontroller.h"

HVACController::HVACController(QObject *parent) :
    QObject(parent)
{
    QString val;
    QFile file;
    file.setFileName("modules/hvac/cars/Sample/sample_car.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();
    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject test = d.object();

    QJsonObject defaultSettings;

    defaultSettings["fanSteps"] = 5;
    defaultSettings["fanStepSize"] = 1;
    defaultSettings["temperatureSteps"] = 15;
    defaultSettings["temperatureStepSize"] = 1;
    defaultSettings["temperatureOffset"] = 15;
    defaultSettings["seatHeatSteps"] = 5;
    defaultSettings["directionUp"] = true;
    defaultSettings["directionCenter"] = true;
    defaultSettings["directionDown"] = true;
    defaultSettings["directionAuto"] = true;
    defaultSettings["zones"] = 4;
    defaultSettings["recirculate"] = true;
    defaultSettings["auto"] = true;
    defaultSettings["ac"] = true;
    defaultSettings["frontDefrost"] = true;
    defaultSettings["rearDefrost"] = true;
    defaultSettings["seatHeating"] = true;

    for (auto it = test.constBegin(); it != test.constEnd(); it++) {
        defaultSettings.insert(it.key(), it.value());
    }

    m_settings = defaultSettings.toVariantMap();

    updateHVACParameters();
}

void HVACController::updateHVACParameters(){
    QVariantMap hvacControlArray;
    QVariantMap controlZoneFrontLeft;
    QVariantMap controlZoneFrontRight;
    QVariantMap controlZoneRearLeft;
    QVariantMap controlZoneRearRight;

    controlZoneFrontLeft.insert("DirUp",        m_commandFrame.Front.Left.Direction.Up);
    controlZoneFrontLeft.insert("DirCenter",    m_commandFrame.Front.Left.Direction.Center);
    controlZoneFrontLeft.insert("DirDown",      m_commandFrame.Front.Left.Direction.Down);
    controlZoneFrontLeft.insert("DirAuto",      m_commandFrame.Front.Left.Direction.Auto);
    controlZoneFrontLeft.insert("Fan",          m_commandFrame.Front.Left.Fan);
    controlZoneFrontLeft.insert("Temperature",  m_commandFrame.Front.Left.Temperature);
    controlZoneFrontLeft.insert("SeatHeating",  m_commandFrame.Front.Left.SeatHeating);

    controlZoneFrontRight.insert("DirUp",        m_commandFrame.Front.Right.Direction.Up);
    controlZoneFrontRight.insert("DirCenter",    m_commandFrame.Front.Right.Direction.Center);
    controlZoneFrontRight.insert("DirDown",      m_commandFrame.Front.Right.Direction.Down);
    controlZoneFrontRight.insert("DirAuto",     m_commandFrame.Front.Right.Direction.Auto);
    controlZoneFrontRight.insert("Fan",          m_commandFrame.Front.Right.Fan);
    controlZoneFrontRight.insert("Temperature",  m_commandFrame.Front.Right.Temperature);
    controlZoneFrontRight.insert("SeatHeating",  m_commandFrame.Front.Right.SeatHeating);

    controlZoneRearLeft.insert("DirUp",        m_commandFrame.Rear.Left.Direction.Up);
    controlZoneRearLeft.insert("DirCenter",    m_commandFrame.Rear.Left.Direction.Center);
    controlZoneRearLeft.insert("DirDown",      m_commandFrame.Rear.Left.Direction.Down);
    controlZoneRearLeft.insert("DirAuto",       m_commandFrame.Rear.Left.Direction.Auto);
    controlZoneRearLeft.insert("Fan",          m_commandFrame.Rear.Left.Fan);
    controlZoneRearLeft.insert("Temperature",  m_commandFrame.Rear.Left.Temperature);
    controlZoneRearLeft.insert("SeatHeating",  m_commandFrame.Rear.Left.SeatHeating);

    controlZoneRearRight.insert("DirUp",        m_commandFrame.Rear.Right.Direction.Up);
    controlZoneRearRight.insert("DirCenter",    m_commandFrame.Rear.Right.Direction.Center);
    controlZoneRearRight.insert("DirDown",      m_commandFrame.Rear.Right.Direction.Down);
    controlZoneRearRight.insert("DirAuto",      m_commandFrame.Rear.Right.Direction.Auto);
    controlZoneRearRight.insert("Fan",          m_commandFrame.Rear.Right.Fan);
    controlZoneRearRight.insert("Temperature",  m_commandFrame.Rear.Right.Temperature);
    controlZoneRearRight.insert("SeatHeating",  m_commandFrame.Rear.Right.SeatHeating);

    hvacControlArray.insert("FrontLeft",    controlZoneFrontLeft);
    hvacControlArray.insert("FrontRight",   controlZoneFrontRight);
    hvacControlArray.insert("RearLeft",     controlZoneRearLeft);
    hvacControlArray.insert("RearRight",    controlZoneRearRight);

    hvacControlArray.insert("TempSelectLeft",   m_commandFrame.TempSelectLeft);
    hvacControlArray.insert("TempSelectRight",  m_commandFrame.TempSelectRight);
    hvacControlArray.insert("FanSelectLeft",    m_commandFrame.FanSelectLeft);
    hvacControlArray.insert("FanSelectRight",   m_commandFrame.FanSelectRight);

    hvacControlArray.insert("ProgAuto",         m_commandFrame.ProgAuto);
    hvacControlArray.insert("ProgAutoFanFront", m_commandFrame.ProgAutoFanFront);
    hvacControlArray.insert("ProgAutoFanRear",  m_commandFrame.ProgAutoFanRear);
    hvacControlArray.insert("ProgWindscreen",   m_commandFrame.ProgWindscreen);

    hvacControlArray.insert("Recirculate",   m_commandFrame.Recirculate);
    hvacControlArray.insert("RearDefrost",   m_commandFrame.RearDefrost);
    hvacControlArray.insert("AC",   m_commandFrame.AC);
    m_hvacParameters = hvacControlArray;
    emit parametersUpdated();
}

void HVACController::setHVACParameters(QVariant parameters){
    if(parameters.canConvert<ClimateControlCommandFrame>()){
        m_commandFrame = qvariant_cast<ClimateControlCommandFrame>(parameters);
        updateHVACParameters();
    }
}

void HVACController::setParameter(QString parameter, bool value){
    if(parameter == "TempSelectLeft"){
        m_commandFrame.TempSelectLeft = value;
    } else if(parameter == "TempSelectRight"){
        m_commandFrame.TempSelectRight = value;
    } else if(parameter == "FanSelectLeft"){
        m_commandFrame.FanSelectLeft = value;
    } else if(parameter == "FanSelectRight"){
        m_commandFrame.FanSelectRight = value;
    } else if(parameter == "ProgAuto"){
        m_commandFrame.ProgAuto = value;
    } else if(parameter == "ProgAutoFanFront"){
        m_commandFrame.ProgAutoFanFront = value;
    } else if(parameter == "ProgAutoFanRear"){
        m_commandFrame.ProgAutoFanRear = value;
    } else if(parameter == "ProgWindscreen"){
        m_commandFrame.ProgWindscreen = value;
    } else if(parameter == "Recirculate"){
        m_commandFrame.Recirculate = value;
    } else if(parameter == "RearDefrost"){
        m_commandFrame.RearDefrost = value;
    } else if(parameter == "AC"){
        m_commandFrame.AC = value;
    }
    qDebug() << "hvacParmeterUpdate" << parameter;
    emit hvacParmeterUpdate(m_commandFrame);
}

void HVACController::setZoneParameter(QString zone, QString parameter, QVariant value) {
    ClimateControlZoneControl *zoneControl;

    if(zone == "FrontLeft"){
        zoneControl = &m_commandFrame.Front.Left;
    } else if(zone == "FrontRight"){
        zoneControl = &m_commandFrame.Front.Right;
    } else if(zone == "RearLeft"){
        zoneControl = &m_commandFrame.Rear.Left;
    } else if(zone == "RearRight"){
        zoneControl = &m_commandFrame.Rear.Right;
    } else {
        return;
    }

    if(parameter == "Fan"){
        zoneControl->Fan = value.toUInt();
    } else if(parameter == "Temperature"){
        zoneControl->Temperature = value.toUInt();;
    }  else if(parameter == "SeatHeating"){
        zoneControl->SeatHeating = value.toUInt();;
    } else if(parameter == "DirUp"){
        zoneControl->Direction.Up = value.toBool();
    } else if(parameter == "DirCenter"){
        zoneControl->Direction.Center = value.toBool();
    } else if(parameter == "DirDown"){
        zoneControl->Direction.Down = value.toBool();
    } else if(parameter == "DirAuto"){
        zoneControl->Direction.Auto = value.toBool();
    }
    qDebug() << "hvacParmeterUpdate" << zone << parameter << value;
    emit hvacParmeterUpdate(m_commandFrame);
}
void HVACController::openOverlay(bool rightHand){
    QVariantMap props;
    props.insert("rightHand", rightHand);
    emit action("GUI::OpenOverlay", props);
}
void HVACController::closeOverlay(){
    emit action("GUI::CloseOverlay", QVariant());
}

HVACController::~HVACController(){
    qDebug() << "Deleting HVACController";
}
