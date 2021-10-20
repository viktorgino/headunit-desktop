#include "welleio.h"

WelleIoPlugin::WelleIoPlugin(QObject *parent) : QObject (parent), m_translator(this)
{
    QVariantMap commandLineOptions;
    commandLineOptions["dumpFileName"] = "test.dump";

    m_radioController = new CRadioController (commandLineOptions, this);
    m_guiHelper = new CGUIHelper (m_radioController, this);
}

void WelleIoPlugin::init(){
    m_guiHelper->setTranslator(&m_translator);

    connect(&m_settings, &QQmlPropertyMap::valueChanged, this, &WelleIoPlugin::settingsChanged);

    for(QString key : m_settings.keys()) {
        settingsChanged(key, m_settings[key]);
    }
    onSettingsPageDestroyed();
    m_radioController->selectFFTWindowPlacement(1);
    m_radioController->setFreqSyncMethod(1);
    playLastStation();
}

void WelleIoPlugin::settingsChanged(const QString &key, const QVariant &value){
    if(key == "auto_gain"){
        m_radioController->setAGC(value.toBool());
    } else if(key == "gain"){
        m_radioController->setGain(value.toInt());
    } else if(key == "auto_receiver"){
        m_adapterChanged = true;
    } else if(key == "receiver"){
        m_adapterChanged = true;
    } else if(key == "airspy_bias_tee"){
        m_guiHelper->setBiasTeeAirspy(value.toBool());
    } else if(key == "rtlsdr_bias_tee"){
        m_guiHelper->setBiasTeeRtlSdr(value.toBool());
    } else if(key == "soapysdr_antenna"){
        m_guiHelper->setAntennaSoapySdr(value.toString());
    } else if(key == "soapysdr_clock"){
        m_guiHelper->setClockSourceSoapySdr(value.toString());
    } else if(key == "soapysdr_driver_args"){
        m_guiHelper->setDriverArgsSoapySdr(value.toString());
    } else if(key == "rtltcp_host"){
        m_adapterChanged = true;
    } else if(key == "rtltcp_port"){
        m_adapterChanged = true;
    } else if(key == "raw_file"){
        m_adapterChanged = true;
    } else if(key == "raw_type"){
        m_adapterChanged = true;
    } else if(key == "coarse_corrector"){
        m_radioController->disableCoarseCorrector(!value.toBool());
    } else if(key == "coarse_corrector_algo"){
        m_radioController->setFreqSyncMethod(value.toInt());
    } else if(key == "fft_window_algo"){
        m_radioController->selectFFTWindowPlacement(value.toInt());
    }
}

void WelleIoPlugin::onSettingsPageDestroyed() {
    qDebug() << "WelleIoPlugin onSettingsPageDestroyed";
    if(m_adapterChanged){
        if(m_settings["auto_receiver"].toBool()){
            m_guiHelper->openAutoDevice();
        } else {
            switch (m_settings["receiver"].toInt()) {
            case 1:
                m_guiHelper->openAirspy();
                break;
            case 2:
                m_guiHelper->openRtlSdr();
                break;
            case 3:
                m_guiHelper->openSoapySdr();
                break;
            case 4:
                m_guiHelper->openRtlTcp(m_settings["rtltcp_host"].toString(), m_settings["rtltcp_port"].toInt(), true);
                break;
            case 5:
                m_guiHelper->openRawFile(m_settings["raw_file"].toString(), m_settings["raw_type"].toString());
                break;
            default:
                break;
            }

        }
        m_adapterChanged = false;
    }

}

void WelleIoPlugin::play(QString channel, QString title, quint32 service) {
    QSettings settings;
    settings.beginGroup("WelleIoPlugin");
    settings.setValue("lastChannel", channel);
    settings.setValue("lastChannelTitle", title);
    settings.setValue("lastService", service);

    m_radioController->play(channel,title,service);
}


void WelleIoPlugin::playLastStation() {
    QSettings settings;
    settings.beginGroup("WelleIoPlugin");

    if(settings.contains("lastChannel") && settings.contains("lastService")){
        m_radioController->play(settings.value("lastChannel").toString(), settings.value("lastChannelTitle").toString(), settings.value("lastService").toULongLong());
    }
}

WelleIoPlugin::~WelleIoPlugin() {
    qDebug() << "Deleting WelleIoPlugin";

    if(m_guiHelper){
        delete m_guiHelper;
    }
    if(m_radioController){
        delete m_radioController;
    }
}
CGUIHelper *WelleIoPlugin::guiHelper(){
    return m_guiHelper;
}

CRadioController *WelleIoPlugin::radioController(){
    return m_radioController;
}

QObject *WelleIoPlugin::getContextProperty(){
    return this;
}

QQuickImageProvider *WelleIoPlugin::getImageProvider() {
    return m_guiHelper->motImageProvider;
}
