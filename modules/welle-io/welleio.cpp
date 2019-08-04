#include "welleio.h"

WelleIoPlugin::WelleIoPlugin(QObject *parent) : QObject (parent), welleIoWrapper(new WelleIoWrapper())
{

    qDebug() << "WelleIoPlugin";
    CDebugOutput::init();

    // Register waterfall diagram
    qmlRegisterType<WaterfallItem>("io.welle", 1, 0, "Waterfall");

    QStringList themePaths;
    themePaths << "qrc:/WelleIoPluginRes/icon";
    QIcon::setThemeSearchPaths(themePaths);
    QIcon::setThemeName("welle_io_icons");

    QVariantMap commandLineOptions;
    commandLineOptions["dumpFileName"] = "test.dump";

    m_radioController = new CRadioController (commandLineOptions);

    m_guiHelper = new CGUIHelper (m_radioController);


    welleIoWrapper->setRadioController(m_radioController);
    welleIoWrapper->setGuiHelper(m_guiHelper);

}


WelleIoPlugin::~WelleIoPlugin() {
    qDebug() << "Deleting WelleIoPlugin";
    if(welleIoWrapper){
        delete welleIoWrapper;
    }
    if(m_guiHelper){
        delete m_guiHelper;
    }
    if(m_radioController){
        delete m_radioController;
    }
}
CGUIHelper *WelleIoWrapper::guiHelper(){
    return m_guiHelper;
}

CRadioController *WelleIoWrapper::radioController(){
    return m_radioController;
}

void WelleIoWrapper::setGuiHelper(CGUIHelper * GUIHelper){
    m_guiHelper = GUIHelper;
    emit guiHelperChanged();
}

void WelleIoWrapper::setRadioController(CRadioController * radioController){
    m_radioController = radioController;
    emit radioControllerChanged();
}

WelleIoWrapper::~WelleIoWrapper() {
    qDebug() << "Deleting WelleIoWrapper";
}
QObject *WelleIoPlugin::getContextProperty(){
    return welleIoWrapper;
}

QQuickImageProvider *WelleIoPlugin::getImageProvider() {
    return welleIoWrapper->guiHelper()->motImage;
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
