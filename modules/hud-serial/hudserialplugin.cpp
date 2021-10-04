#include "hudserialplugin.h"

HUDSerialPlugin::HUDSerialPlugin(QObject *parent) : QObject (parent), m_serial(this), m_serialProtocol()
{
    m_serialProtocol.setCallbacks(this);

    m_pluginSettings.eventListeners = QStringList() << "HVACPlugin::Update";
    m_pluginSettings.events = QStringList() << "KeyInput" << "MediaInput";
}

void HUDSerialPlugin::init() {
    updatePorts();

    serialConnect();
    connect(&m_serial, &QSerialPort::readyRead, this, &HUDSerialPlugin::handleSerialReadyRead);
    connect(&m_serial, &QSerialPort::errorOccurred, this, &HUDSerialPlugin::handleSerialError);
    connect(&m_settings, &QQmlPropertyMap::valueChanged, this, &HUDSerialPlugin::settingsChanged);
}

QObject *HUDSerialPlugin::getContextProperty(){
    return this;
}

void HUDSerialPlugin::eventMessage(QString id, QVariant message) {
    if(id == "HVACPlugin::Update"){
        if(message.canConvert<ClimateControlCommandFrame>()){
            m_serialProtocol.sendClimateControlCommand(qvariant_cast<ClimateControlCommandFrame>(message));
        }
    }
}
void HUDSerialPlugin::settingsChanged(const QString &key, const QVariant &){
    if(key == "serial_port" || key == "serial_speed"){
        serialDisconnect();
        serialConnect();
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
    m_serial.setPortName(m_settings.value("serial_port").toString());
    m_serial.setBaudRate(m_settings.value("serial_speed").toString().toInt());

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
    if(error == QSerialPort::ReadError){
        if(m_serial.isOpen()){
            m_serial.close();
        }
        m_serial.clearError();
        m_connected = false;
        emit connectedUpdated();
        qDebug() << "Error : " << error;
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
}

void HUDSerialPlugin::BodyControlCommandCallback(const BodyControlCommandFrame &controlFrame) {

}
void HUDSerialPlugin::DriveTrainControlCommandCallback(const DriveTrainControlCommandFrame &controlFrame) {

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
        emit action("VolumeControl::VolumeUp", 0);
        break;
    case Key_VolumeDown:
        emit action("VolumeControl::VolumeDown", 0);
        break;
    default:
        break;
    }

}
void HUDSerialPlugin::SendMessageCallback(uint8_t receivedByte){
    if(m_serial.isOpen()){
        m_serial.write((char*)&receivedByte, 1);
        m_serial.flush();
    }
}

void HUDSerialPlugin::PrintString(char *message, int length) {
    qDebug() << "HCU DEBUG : " << message;
}


