#include "welleio.h"

WelleIoPlugin::WelleIoPlugin(QObject *parent) : QObject (parent), welleIoWrapper(new WelleIoWrapper())
{
    welleioError = false;

    // Default values
    QVariantMap commandLineOptions;
    CDABParams DABParams(1);
    commandLineOptions["dabDevice"] = "auto";
    commandLineOptions["ipAddress"] = "127.0.0.1";
    commandLineOptions["ipPort"] = 1234;
    commandLineOptions["rawFile"] =  "";
    commandLineOptions["rawFileFormat"] = "u8";

    if(!welleioError){
        try {
            // Create a new radio interface instance
            CRadioController* RadioController = new CRadioController(commandLineOptions, DABParams);
            QTimer::singleShot(0, RadioController, SLOT(onEventLoopStarted()));
            CGUI *GUI = new CGUI(RadioController, &DABParams);

            welleIoWrapper->setGui(GUI);
            welleIoWrapper->setRadioController(RadioController);

        } catch (int e) {
            qDebug()<< "Can't initialise welle.io. Exception: " << e ;
            welleioError = true;
        }
    }
}

CGUI *WelleIoWrapper::gui(){
    return m_GUI;
}

CRadioController *WelleIoWrapper::radioController(){
    return m_radioController;
}

void WelleIoWrapper::setGui(CGUI * GUI){
    m_GUI = GUI;
    emit guiChanged();
}

void WelleIoWrapper::setRadioController(CRadioController * radioController){
    m_radioController = radioController;
    emit radioControllerChanged();
}

QObject *WelleIoPlugin::getContextProperty(){
    return welleIoWrapper;
}

QQuickImageProvider *WelleIoPlugin::getImageProvider() {
    return welleIoWrapper->gui()->MOTImage;
}

QStringList WelleIoPlugin::eventListeners(){
    return QStringList();
}

QStringList WelleIoPlugin::events(){
    return QStringList();
}

QStringList WelleIoPlugin::actions() {
    return QStringList();
}

void WelleIoPlugin::eventMessage(QString id, QString message){

}

void WelleIoPlugin::actionMessage(QString id, QString message){

}
