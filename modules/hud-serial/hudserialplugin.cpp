#include "hudserialplugin.h"

HUDSerialPlugin::HUDSerialPlugin(QObject *parent) : QObject (parent), m_serial(), m_serialProtocol(), m_serialRetryTimer(this)
{
    m_serialProtocol.setCallbacks(this);
    m_pluginSettings.eventListeners = QStringList() << "HVACPlugin::Update" << "HVACPlugin::CustomCommandUpdate";
    m_pluginSettings.events = QStringList() << "KeyInput" << "MediaInput";
    m_pluginSettings.actions = QStringList() << "AudioControl";
    m_speeds = {9600,19200,38400,57600,115200,230400,460800,500000,921600,1000000,1152000,2000000,4608000};
}

void HUDSerialPlugin::init() {
    updatePorts();
    updateManufacturers();
    updateCars();
    loadCarSettings(m_settings["car"].toString());

    serialConnect();
    connect(&m_serial, &QSerialPort::readyRead, this, &HUDSerialPlugin::handleSerialReadyRead);
    connect(&m_serial, &QSerialPort::errorOccurred, this, &HUDSerialPlugin::handleSerialError);
    connect(&m_settings, &QQmlPropertyMap::valueChanged, this, &HUDSerialPlugin::settingsChanged);
    connect(&m_serialRetryTimer, &QTimer::timeout, this, &HUDSerialPlugin::serialConnect);

    for(int i = 0; i < 16; i++) {
        m_customBits.append(0);
    }
    for(int i = 0; i < 6; i++) {
        m_customBytes.append(0);
    }

    emit customCommandUpdated();
}

QObject *HUDSerialPlugin::getContextProperty(){
    return this;
}

void HUDSerialPlugin::eventMessage(QString id, QVariant message) {
    if(id == "HVACPlugin::Update"){
        if(message.canConvert<ClimateControlCommandFrame>()){
            m_serialProtocol.sendClimateControlCommand(qvariant_cast<ClimateControlCommandFrame>(message));
        }
    } else if (id == "HVACPlugin::CustomCommandUpdate") {
        if(message.canConvert(QMetaType::QVariantMap)){
            QVariantMap map = message.toMap();
            if(map.contains("bits") && map.contains("bytes")){

                CustomCommandFrame commandFrame;
                QVariantList customBits = map["bits"].toList();
                QVariantList customBytes = map["bytes"].toList();
                for(int i = 0; i < customBits.size(); i++) {
                    commandFrame.Bits[i] = customBits[i].toBool();
                }
                for(int i = 0; i < customBytes.size(); i++) {
                    commandFrame.Bytes[i] = customBytes[i].toUInt();
                }
                m_serialProtocol.sendCustomCommand(commandFrame);
            }
        }
    }
}

void HUDSerialPlugin::actionMessage(QString id, QVariant message) {

}
void HUDSerialPlugin::settingsChanged(const QString &key, const QVariant &){
    if(key == "serial_port" || key == "serial_speed"){
        serialDisconnect();
        serialConnect();
    } else if(key == "car_make"){
        updateCars();
        if(m_cars.size() > 0){
            m_settings["car"] = m_cars.keys()[0];
            loadCarSettings(m_settings["car"].toString());
            emit m_settings.valueChanged("car", m_settings["car"]);
        }
    } else if(key == "car") {
        loadCarSettings(m_settings["car"].toString());
    }
}

void HUDSerialPlugin::updatePorts() {
    m_ports.clear();
    for(const QSerialPortInfo &port : QSerialPortInfo::availablePorts()){
        QString displayName = QString("%1 (%2 - %3)").arg(port.portName()).arg(port.manufacturer()).arg(port.productIdentifier());
        m_ports.insert(port.portName(),displayName);
    }
    emit portsUpdated();
}

void HUDSerialPlugin::serialConnect(){
//    m_serialRetryTimer.stop();
    m_serial.setPortName(m_settings.value("serial_port").toString());
    int serialId = m_settings.value("serial_speed").toInt();
    m_serial.setBaudRate(m_speeds[serialId].toInt());

    if (!m_serial.open(QIODevice::ReadWrite)) {
        qDebug() << QObject::tr("Failed to open port %1, error: %2")
                        .arg(m_settings.value("port").toString(), m_serial.errorString())
                 << "\n";
    } else {
        qDebug() << "Connected to Serial : " << m_serial.portName() << m_serial.baudRate();
        m_connected = true;
        m_serialProtocol.sendUpdateRequest();
        emit connectedUpdated();
    }
}

void HUDSerialPlugin::serialDisconnect(){
    if(m_serial.isOpen()){
        m_serial.close();
    }
    m_connected = false;
    emit connectedUpdated();
}

void HUDSerialPlugin::serialRestart(){
    emit message("GUI::Notification", "{\"image\":\"qrc:/qml/icons/alert.png\", \"title\":\"HUD Serial\", \"description\":\"Serial Port restarted\"}");
    serialDisconnect();
    serialConnect();
}
void HUDSerialPlugin::handleSerialError(QSerialPort::SerialPortError error){
    switch (error) {
    case QSerialPort::WriteError:
    case QSerialPort::ReadError:
    case QSerialPort::NotOpenError:
    case QSerialPort::DeviceNotFoundError:
    case QSerialPort::PermissionError:
    case QSerialPort::TimeoutError:
        if(m_serial.isOpen()){
            m_serial.close();
        }
        m_serial.clearError();
        m_connected = false;
        emit connectedUpdated();
        qDebug() << "Error : " << error;
//        m_serialRetryTimer.start(1000);

        break;
    default:
        break;
    }
}

void HUDSerialPlugin::handleSerialReadyRead(){
    if(m_serial.isOpen()){
        uint8_t buffer;
        while(m_serial.read((char*)&buffer, 1)){
            uint8_t rec = static_cast<uint8_t>(buffer);
            m_serialProtocol.receiveByte(rec);
            if(!m_serial.isOpen()){
                break;
            }
        }
    }
}
QString buttonToString(Keys key) {
    switch (key) {
    case Key_1:
        return "1";
    case Key_2:
        return "2";
    case Key_3:
        return "3";
    case Key_4:
        return "4";
    case Key_5:
        return "5";
    case Key_6:
        return "6";
    case Key_7:
        return "7";
    case Key_8:
        return "8";
    case Key_9:
        return "9";
    case Key_0:
        return "0";
    case Key_Enter:
        return "Enter";
    case Key_Exit:
        return "Exit";
    case Key_Back:
        return "Back";
    case Key_Menu:
        return "Menu";
    case Key_Power:
        return "Power";
    case Key_Left:
        return "Left";
    case Key_Right:
        return "Right";
    case Key_Up:
        return "Up";
    case Key_Down:
        return "Down";
    case Key_VolumeUp:
        return "VolumeUp";
    case Key_VolumeDown:
        return "VolumeDown";
    case Key_TuneUp:
        return "TuneUp";
    case Key_TuneDown:
        return "TuneDown";
    case Key_Sound:
        return "Sound";
    case Key_AMFM:
        return "AMFM";
    case Key_CD:
        return "CD";
    case Key_Eject:
        return "Eject";
    case Key_Auto:
        return "Auto";
    case Key_Scan:
        return "Scan";
    default:
        return "";
    }
}

void HUDSerialPlugin::ClimateControlCallback(const ClimateControlCommandFrame &commandFrame){
    emit action("HVACPlugin::Update", QVariant::fromValue(commandFrame));
}
void HUDSerialPlugin::CustomCommandCallback(const CustomCommandFrame &commandFrame){
    m_customBits.clear();
    m_customBytes.clear();
    for(int i = 0; i < 16; i++) {
        m_customBits.append(commandFrame.Bits[i]);
    }
    for(int i = 0; i < 6; i++) {
        m_customBytes.append(commandFrame.Bytes[i]);
    }

    emit customCommandUpdated();
}

void HUDSerialPlugin::BodyControlCommandCallback(const BodyControlCommandFrame &controlFrame) {
    // emit action("SYSTEM::SetNightMode",controlFrame.NightLight);
    // TODO : Fix screen light
}
void HUDSerialPlugin::DriveTrainControlCommandCallback(const DriveTrainControlCommandFrame &) {

}

void HUDSerialPlugin::ButtonInputCommandCallback(Keys key){
    qDebug() << "Key pressed " << buttonToString(key);

    switch (key) {
    case Key_Next:
        emit message("MediaInput", "Next");
        break;
    case Key_Previous:
        emit message("MediaInput", "Previous");
        break;
    case Key_VolumeUp:
        emit action("AudioProcessorPlugin::VolumeUp", 0);
        break;
    case Key_VolumeDown:
        emit action("AudioProcessorPlugin::VolumeDown", 0);
        break;
    case Key_Sound:
        emit action("AudioProcessorPlugin::Sound", 0);
        break;
    case Key_TuneDown:
        emit action("AudioProcessorPlugin::TuneDown", 0);
        break;
    case Key_TuneUp:
        emit action("AudioProcessorPlugin::TuneUp", 0);
        break;
    default:
        break;
    }

}
void HUDSerialPlugin::SendMessageCallback(uint8_t receivedByte){
    if(m_serial.isOpen()){
        m_serial.write((char*)&receivedByte, 1);
//        m_serial.flush(); 
    }
}

void HUDSerialPlugin::PrintString(char *message, int length) {
    if(length > 0) {
        qDebug("HCU DEBUG : %s", message);
    }
}

void HUDSerialPlugin::setCustomBit(uint bitNumber, bool value){
    if(m_customBits.size() > bitNumber){
        QVariantList customBits(m_customBits);
        customBits[bitNumber] = value;

        CustomCommandFrame commandFrame;

        for(int i = 0; i < customBits.size(); i++) {
            commandFrame.Bits[i] = customBits[i].toBool();
        }
        for(int i = 0; i < m_customBytes.size(); i++) {
            commandFrame.Bytes[i] = m_customBytes[i].toUInt();
        }
        m_serialProtocol.sendCustomCommand(commandFrame);
    }
}

void HUDSerialPlugin::setCustomByte(uint byteNumber, uint value){
    if(m_customBytes.size() > byteNumber){
        QVariantList customBytes(m_customBytes);
        customBytes[byteNumber] = value;

        CustomCommandFrame commandFrame;

        for(int i = 0; i < m_customBits.size(); i++) {
            commandFrame.Bits[i] = m_customBits[i].toBool();
        }
        for(int i = 0; i < customBytes.size(); i++) {
            commandFrame.Bytes[i] = customBytes[i].toUInt();
        }
        m_serialProtocol.sendCustomCommand(commandFrame);
    }
}

void HUDSerialPlugin::loadCarSettings(QString fileName){
    if(fileName.isEmpty()){
        return;
    }
//    resetHVACSettings();

    QFile file;
    file.setFileName(QCoreApplication::applicationDirPath() + "/" + fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QString configFile = file.readAll();
    file.close();
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(configFile.toUtf8(), &error);
    if(doc.isNull()){
        qDebug() << "JSON Parse error : " << error.errorString();
    }
    QJsonObject json = doc.object();
    QVariantMap carSettings;
    for (auto it = json.constBegin(); it != json.constEnd(); it++) {
        carSettings.insert(it.key(), it.value());
    }
    m_carSettings = carSettings.value("CarSettings").toList();
    emit action("HVACPlugin::HvacSettingsUpdate", carSettings.value("AcSettings").toMap());
    emit carSettingsUpdated();
    qDebug() << "Loaded car settings : " << fileName;
}
void HUDSerialPlugin::updateManufacturers(){
    QDir dir(QCoreApplication::applicationDirPath());
    dir.cd("modules/hud-serial/cars");
    for(const QString &info : dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)){
        m_manufacturers.insert(info,info);
    }
    emit carsUpdated();

}
void HUDSerialPlugin::updateCars(){
    m_cars.clear();
    QDir dir(QCoreApplication::applicationDirPath());
    dir.cd("modules/hud-serial/cars");
    if(dir.cd(m_settings["car_make"].toString())){
        dir.setNameFilters(QStringList("*.json"));
        for(const QFileInfo &info : dir.entryInfoList()){
            QFile file;
            file.setFileName(info.filePath());
            file.open(QIODevice::ReadOnly | QIODevice::Text);

            QString configFile = file.readAll();
            file.close();
            QJsonObject json = QJsonDocument::fromJson(configFile.toUtf8()).object();
            QString name;
            if(json.contains("name")){
                name = json["name"].toString();
            } else {
                name = info.fileName();
            }

            QDir appDir(QCoreApplication::applicationDirPath());
            QString relativePath = appDir.relativeFilePath(info.filePath());
            m_cars.insert(relativePath,name);
        }

        emit carsUpdated();
    }
}
