#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQuick/QQuickView>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlEngine>
#include <QGst/Init>
#include <QGst/Quick/VideoSurface>
#include <QGst/Element>
#include <QGlib/RefPointer>
#include <QJsonObject>
#include <QVariantList>
#include <QQmlComponent>
#include <QThreadPool>
#include <QDir>
#include <QSettings>
#include <QtDebug>
#include <QLoggingCategory>
#include <unistd.h>
#include "headunit.h"
#include "usbconnectionlistener.h"
#include "medialibrary.h"

#ifdef HAVE_WELLEIO
#include <unistd.h>
#include "CInputFactory.h"
#include "CRAWFile.h"
#include "CRTL_TCP_Client.h"
#include "DabConstants.h"
#include "CRadioController.h"
#include "CGUI.h"
#include "dab-gui/dabHelper.h"
#endif

#include "includes/plugininterface.h"

Q_DECLARE_LOGGING_CATEGORY(HEADUNIT)

static QVariantList menuItems;
static QVariantList configItems;
static QQmlApplicationEngine *engine;

bool loadPlugins(QMap<QString, PluginInterface *> *plugins, QMap<QString, QJsonObject> *pluginConfigs)
{
    QDir pluginsDir(qApp->applicationDirPath());
#if defined(Q_OS_WIN)
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
#endif
    pluginsDir.cd("plugins");
    //Load plugins
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));

        QJsonObject metaData;
        if(pluginLoader.metaData().value("MetaData").type() != QJsonValue::Object){
            qDebug() << "Invalid plugin : " << fileName << " config missing";
            //break;
        }

        QObject *plugin = pluginLoader.instance();
        if (!plugin) {
            qDebug() << "Error loading plugin : " << fileName;
            //break;
        } else {
            qDebug() << "Plugin loaded : " << fileName;
        }

        metaData = pluginLoader.metaData().value("MetaData").toObject();
        pluginConfigs->insert(metaData.value("name").toString(), metaData);
        plugins->insert(metaData.value("name").toString(), qobject_cast<PluginInterface *>(plugin));

        QJsonValue menu = metaData.value("menu");
        if(menu.type() == QJsonValue::Object){
            menuItems << menu.toObject().toVariantMap();
        }

        QJsonValue config = metaData.value("config");
        if(config.type() == QJsonValue::Object){
            configItems << config.toObject().toVariantMap();
        }
    }
    //Load QML plugins
    pluginsDir.cd("qml");
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QJsonValue uri = pluginLoader.metaData().value("MetaData").toObject().value("uri");
        QList<QQmlError> errors;
        engine->importPlugin(pluginsDir.absoluteFilePath(fileName),uri.toString(),&errors);
    }
    return false;
}

int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);

    QCoreApplication::setOrganizationName("viktorgino");
    QCoreApplication::setOrganizationDomain("https://github.com/viktorgino/headunit-desktop");
    QCoreApplication::setApplicationName("viktorgino's HeadUnit Desktop");

    QApplication app(argc, argv);
    QLoggingCategory::setFilterRules("");
    engine = new QQmlApplicationEngine;

    QGst::init(&argc, &argv);


    int defaultMenuItem = 4;
    QMap<QString, PluginInterface *> plugins;
    QMap<QString, QJsonObject> pluginConfigs;
    loadPlugins(&plugins, &pluginConfigs);

    menuItems << QJsonObject {{"source","qrc:/qml/ClimateControl/CCLayout.qml"},{"image","icons/svg/thermometer.svg"},{"text","A/C"},{"color","#f44336"}}.toVariantMap()
              << QJsonObject {{"source","qrc:/qml/Radio/RadioLayout.qml"},{"image","icons/svg/radio-waves.svg"},{"text","Radio"},{"color","#E91E63"}}.toVariantMap()
              << QJsonObject {{"source","qrc:/aaVideo.qml"},{"image","icons/svg/social-android.svg"},{"text","Android Auto"},{"color","#673AB7"}}.toVariantMap()
              << QJsonObject {{"source","qrc:/qml/MediaPlayer/MediaPlayerLayout.qml"},{"image","icons/svg/music-note.svg"},{"text","Media player"},{"color","#3F51B5"}}.toVariantMap()
              << QJsonObject {{"source","qrc:/qml/SettingsPage/SettingsPage.qml"},{"image","icons/svg/gear-a.svg"},{"text","Settings"},{"color","#4CAF50"}}.toVariantMap();

    configItems << QJsonObject {{"name","theme"},{"iconImage","qrc:/qml/icons/android-color-palette.png"},{"text","Theme"},{"section","General"},{"source",""}}.toVariantMap()
                << QJsonObject {{"name","behaviour"},{"iconImage","qrc:/qml/icons/android-settings.png"},{"text","Behaviour"},{"section","General"},{"source",""}}.toVariantMap()
                << QJsonObject {{"name","media-locations"},{"iconImage","qrc:/qml/icons/android-folder.png"},{"text","Media locations"},{"section","Media apps"},{"source","qrc:/qml/SettingsPage/SettingsPageMediaLocations.qml"}}.toVariantMap()
                << QJsonObject {{"name","android-uto"},{"iconImage","qrc:/qml/icons/svg/social-android.svg"},{"text","Android Auto"},{"section","Media apps"},{"source","qrc:/qml/SettingsPage/SettingsPageAA.qml"}}.toVariantMap()
                << QJsonObject {{"name","bluetooth"},{"iconImage","qrc:/qml/icons/bluetooth.png"},{"text","Bluetooth"},{"section","Media apps"},{"source","qrc:/qml/SettingsPage/SettingsPageBluetooth.qml"}}.toVariantMap()
                << QJsonObject {{"name","quit"},{"iconImage","qrc:/qml/icons/log-out.png"},{"text","Quit headunit-desktop"},{"section","Other"},{"source",""}}.toVariantMap();
    QGst::Quick::VideoSurface surface;
    Headunit *headunit =  new Headunit(surface.videoSink());
    MediaLibrary *mediaLibrary = new MediaLibrary();

    //Load welle.io
#ifdef HAVE_WELLEIO
    bool welleioError = false;
    // Default values
    QVariantMap commandLineOptions;
    CDABParams DABParams(1);
    commandLineOptions["dabDevice"] = "auto";
    commandLineOptions["ipAddress"] = "127.0.0.1";
    commandLineOptions["ipPort"] = 1234;
    commandLineOptions["rawFile"] =  "";
    commandLineOptions["rawFileFormat"] = "u8";

    CRadioController* RadioController = nullptr;
    CGUI *GUI = nullptr;
    if(!welleioError){
        try {

            // Create a new radio interface instance
            RadioController = new CRadioController(commandLineOptions, DABParams);
            QTimer::singleShot(0, RadioController, SLOT(onEventLoopStarted()));
            GUI = new CGUI(RadioController, &DABParams);
            // Load welle.io's control object to QML context
            engine->rootContext()->setContextProperty("cppGUI", GUI);
            engine->rootContext()->setContextProperty("cppRadioController", RadioController);
            // Add MOT slideshow provider
            engine->addImageProvider(QLatin1String("motslideshow"), GUI->MOTImage);
            // Load the DAB helper class to QML context
            DABHelper *dabHelper = new DABHelper();
            engine->rootContext()->setContextProperty("dabHelper", dabHelper);
        } catch (int e) {
            qDebug()<< "Can't initialise welle.io. Exception: " << e ;
            welleioError = true;
        }
    }
    if(!welleioError){
        menuItems.insert(2,QJsonObject {{"source","qrc:/DABLayout.qml"},{"image","icons/svg/radio-waves.svg"},{"text","DAB"},{"color","#9C27B0"}}.toVariantMap());
    }
#endif

    engine->rootContext()->setContextProperty("videoSurface1", &surface);
    engine->rootContext()->setContextProperty("headunit", headunit);
    engine->rootContext()->setContextProperty("menuItems", menuItems);
    engine->rootContext()->setContextProperty("configItems", configItems);
    engine->rootContext()->setContextProperty("mediaLibrary", mediaLibrary);
    engine->rootContext()->setContextProperty("defaultMenuItem", defaultMenuItem);

    foreach (QJsonObject config, pluginConfigs) {
        engine->rootContext()->setContextProperty(config.value("name").toString(),
                                                  plugins.value(config.value("name").toString())->getContextProperty());
    }

    engine->load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    headunit->startHU();
    UsbConnectionListener *connectionListener = new UsbConnectionListener();
    headunit->setUsbConnectionListener(connectionListener);
    QThreadPool::globalInstance()->start(connectionListener);

    int ret = app.exec();
#ifdef HAVE_WELLEIO
    if(!welleioError){
        delete GUI;
        delete RadioController;
    }
#endif
    foreach (PluginInterface * plugin, plugins) {
        delete(plugin);
    }
    delete(headunit);
    connectionListener->stop();
    return ret;
}
