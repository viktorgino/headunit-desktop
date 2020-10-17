#include "hvaccontroller.h"

HVACController::HVACController(QDir settingsDir, std::map<QString, QVariant> settings, QObject *parent) :
    QObject(parent), m_parameters(new QQmlPropertyMap)
{
    if(settings.size() <= 0){
        return;
    }

    loadValues();

    //Populate default settings array and merge it with loaded settings
    std::map<QString,QVariant> defaultSettings;
    defaultSettings["file"] = QString("hvac.js");
    defaultSettings["type"] = QString("J1850-PWM");
    defaultSettings["loop"] = true;
    defaultSettings["loop_interval"] = 1000;
    defaultSettings["fan1"] = QString("true");
    defaultSettings["fan2"] = QString("true");
    defaultSettings["temp1"] = QString("true");
    defaultSettings["temp2"] = QString("true");
    defaultSettings["left_seat_heat"] = QString("true");
    defaultSettings["right_seat_heat"] = QString("true");
    defaultSettings["air_dir_top"] = QString("true");
    defaultSettings["air_dir_centre"] = QString("true");
    defaultSettings["air_dir_bottom"] = QString("true");
    defaultSettings["air_dir_rear_top"] = QString("true");
    defaultSettings["air_dir_rear_centre"] = QString("true");
    defaultSettings["air_dir_rear_bottom"] = QString("true");
    defaultSettings["air_dir_windshield"] = QString("true");
    defaultSettings["front_def"] = QString("true");
    defaultSettings["rear_def"] = QString("true");
    defaultSettings["ac"] = QString("true");
    defaultSettings["recirculate"] = QString("true");
    defaultSettings["autoac"] = QString("true");
    defaultSettings["power"] = QString("true");
    defaultSettings["temp1_min"] = 15;
    defaultSettings["temp1_max"] = 27;
    defaultSettings["temp2_min"] = 15;
    defaultSettings["temp2_max"] = 27;
    defaultSettings["temp1_step_size"] = 1;
    defaultSettings["temp2_step_size"] = 1;
    defaultSettings["fan1_min"] = 0;
    defaultSettings["fan1_max"] = 6;
    defaultSettings["fan2_min"] = 0;
    defaultSettings["fan2_max"] = 6;
    defaultSettings["left_seat_heat_steps"] = 3;
    defaultSettings["right_seat_heat_steps"] = 3;
    defaultSettings["func-setTemperature1"] = QString("setTemperature1");
    defaultSettings["func-setTemperature2"] = QString("setTemperature2");
    defaultSettings["func-setFanLevel1"] = QString("setFanLevel1");
    defaultSettings["func-setFanLeve2"] = QString("setFanLevel2");
    defaultSettings["func-setTempUnit"] = QString("setTempUnit");
    defaultSettings["func-setRecirculate"] = QString("setRecirculate");
    defaultSettings["func-setHvacPower"] = QString("setHvacPower");
    defaultSettings["func-setRearDefrost"] = QString("setRearDefrost");
    defaultSettings["func-setAcPower"] = QString("setAcPower");
    defaultSettings["func-setAirflowDirection"] = QString("setAirflowDirection");
    defaultSettings["func-timerAction"] = QString("timerAction");

    settings.insert(defaultSettings.begin(), defaultSettings.end());

    m_settings = QMap<QString,QVariant>(settings);

    QString hvacFileName = settings["file"].toString();

    QFile hvacFile(settingsDir.filePath(hvacFileName));
    if(hvacFile.open(QIODevice::ReadOnly)){
        hvacEngine = new QJSEngine(this);
        hvacEngine->installExtensions(QJSEngine::ConsoleExtension);

        QJSValue ControllerJSproperty = hvacEngine->newQObject(this);
        hvacEngine->globalObject().setProperty("Controller", ControllerJSproperty);

        QJSValue meta = hvacEngine->newQMetaObject(&HVACController::staticMetaObject);
        hvacEngine->globalObject().setProperty("Meta", meta);


        QTextStream hvacFileStream(&hvacFile);
        QString hvacFileContents = hvacFileStream.readAll();
        hvacFile.close();
        hvacEngine->evaluate(hvacFileContents, hvacFileName);

        if((hvacEngine->globalObject().hasProperty(settings["func-timerAction"].toString())
                && m_settings.value("loop").toBool())){
            timer = new QTimer(this);
            int loopInterval = m_settings["loop_interval"].toInt();
            connect(timer, &QTimer::timeout, this, &HVACController::executeTimedAction);
            timer->start(loopInterval);
        }
    }
}

void HVACController::executeTimedAction(){
    QString functionName = m_settings["func-timerAction"].toString();
    hvacEngine->globalObject().property(functionName).call();

}

void HVACController::sendMessage(QString message) {

}
QString HVACController::parameterKeytoString(HVACParameters parameter){
    switch (parameter) {
    case Windshield:
        return "Windshield";
    case FrontTop:
        return "FrontTop";
    case FrontMiddle:
        return "FrontMiddle";
    case FrontBottom:
        return "FrontBottom";
    case RearTop:
        return "RearTop";
    case RearMiddle:
        return "RearMiddle";
    case RearBottom:
        return "RearBottom";
    case FrontDefrost:
        return "FrontDefrost";
    case RearDefrost:
        return "RearDefrost";
    case AC:
        return "AC";
    case Recirculate:
        return "Recirculate";
    case AutoAc:
        return "AutoAc";
    case Power:
        return "Power";
    case Fan1:
        return "Fan1";
    case Fan2:
        return "Fan2";
    case Temperature1:
        return "Temperature1";
    case Temperature2:
        return "Temperature2";
    case SeatHeatLeft:
        return "SeatHeatLeft";
    case SeatHeatRight:
        return "SeatHeatRight";
    case InvalidParameter:
        return "";
    }
}

HVACController::HVACParameters HVACController::stringToParameterKey(QString key){
    if(key == "Windshield")
        return Windshield;
    if(key == "FrontTop")
        return FrontTop;
    if(key == "FrontMiddle")
        return FrontMiddle;
    if(key == "FrontBottom")
        return FrontBottom;
    if(key == "RearTop")
        return RearTop;
    if(key == "RearMiddle")
        return RearMiddle;
    if(key == "RearBottom")
        return RearBottom;
    if(key == "FrontDefrost")
        return FrontDefrost;
    if(key == "RearDefrost")
        return RearDefrost;
    if(key == "AC")
        return AC;
    if(key == "Recirculate")
        return Recirculate;
    if(key == "AutoAc")
        return AutoAc;
    if(key == "Power")
        return Power;
    if(key == "Fan1")
        return Fan1;
    if(key == "Fan2")
        return Fan2;
    if(key == "Temperature1")
        return Temperature1;
    if(key == "Temperature2")
        return Temperature2;
    if(key == "SeatHeatLeft")
        return SeatHeatLeft;
    if(key == "SeatHeatRight")
        return SeatHeatRight;
    qDebug() << "Invalid HVACParameter";
    return InvalidParameter;
}

void HVACController::toggleParameter(QString parameterString){
    HVACParameters parameter = stringToParameterKey(parameterString);
    QVariant value = m_parameters->value(parameterKeytoString(parameter));
    if(value.canConvert(QMetaType::Bool)){
        emit setParameter(parameter, !value.toBool());
    }
}
void HVACController::setParameterValue(QString parameterString, QVariant value){
    HVACParameters parameter = stringToParameterKey(parameterString);
    emit setParameter(parameter, value);
}
void HVACController::loadValues(){
    QList<HVACParameters> boolValues = {Windshield,FrontTop,FrontMiddle,FrontBottom,RearTop,RearMiddle,RearBottom,FrontDefrost,RearDefrost,AC,Recirculate,AutoAc,Power};
    QList<HVACParameters> intValues = {Fan1,Fan2,Temperature1,Temperature2,SeatHeatLeft,SeatHeatRight};

    foreach(HVACParameters parameter, boolValues){
        m_parameters->insert(parameterKeytoString(parameter),false);
    }

    foreach(HVACParameters parameter, intValues){
        m_parameters->insert(parameterKeytoString(parameter),0);
    }
}

HVACController::~HVACController(){
    qDebug() << "Deleting HVACController";
//    delete(hvacEngine);
    delete(timer);
}
