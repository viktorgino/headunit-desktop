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
int main(int argc, char *argv[])
{

    setbuf(stdout, NULL);

    QApplication app(argc, argv);
    QQmlApplicationEngine engine;

    QGst::init(&argc, &argv);


    int defaultMenuItem = 2;
    QVariantList menuItems;
    menuItems << QJsonObject {{"source","qrc:/qml/ClimateControl/CCLayout.qml"},{"image","icons/svg/thermometer.svg"},{"text","A/C"},{"color","#f44336"}}.toVariantMap()
              << QJsonObject {{"source","qrc:/qml/Radio/RadioLayout.qml"},{"image","icons/svg/radio-waves.svg"},{"text","Radio"},{"color","#E91E63"}}.toVariantMap()
              << QJsonObject {{"source","qrc:/aaVideo.qml"},{"image","icons/svg/social-android.svg"},{"text","Android Auto"},{"color","#673AB7"}}.toVariantMap()
              << QJsonObject {{"source","qrc:/qml/MediaPlayer/MediaPlayerLayout.qml"},{"image","icons/svg/music-note.svg"},{"text","Media player"},{"color","#3F51B5"}}.toVariantMap()
              << QJsonObject {{"source","qrc:/qml/SettingsPage/SettingsPage.qml"},{"image","icons/gear-a.png"},{"text","Settings"},{"color","#2196F3"}}.toVariantMap();
    QGst::Quick::VideoSurface surface;
    Headunit *headunit =  new Headunit(surface.videoSink());
    MediaLibrary *mediaLibrary = new MediaLibrary();

    //Load welle.io
#ifdef HAVE_WELLEIO
    bool welleioError = false;
    // Default values
    CDABParams DABParams(1);
    QString dabDevice = "auto";
    QString ipAddress = "127.0.0.1";
    uint16_t ipPort = 1234;
    QString rawFile = "";
    QString rawFileFormat = "u8";

    // Init device
    CVirtualInput* Device;
    try {
        Device = CInputFactory::GetDevice(dabDevice);
    } catch (int e) {
        qDebug()<< "DAB device not found! ";
        welleioError = true;
    }
    CRadioController* RadioController;
    CGUI *GUI;
    if(!welleioError){
        try {

            // Create a new radio interface instance
            RadioController = new CRadioController(Device, DABParams);
            GUI = new CGUI(RadioController, &DABParams);
            // Load welle.io's control object to QML context
            engine.rootContext()->setContextProperty("cppGUI", GUI);
            // Add MOT slideshow provider
            engine.addImageProvider(QLatin1String("motslideshow"), GUI->MOTImage);
            // Load the DAB helper class to QML context
            DABHelper *dabHelper = new DABHelper();
            engine.rootContext()->setContextProperty("dabHelper", dabHelper);
        } catch (int e) {
            qDebug()<< "Can't initialise welle.io. Exception: " << e ;
            welleioError = true;
        }
    }
    if(!welleioError){
        menuItems.insert(2,QJsonObject {{"source","qrc:/DABLayout.qml"},{"image","icons/svg/radio-waves.svg"},{"text","DAB"},{"color","#9C27B0"}}.toVariantMap());
    }
#endif

    engine.rootContext()->setContextProperty("videoSurface1", &surface);
    engine.rootContext()->setContextProperty("headunit", headunit);
    engine.rootContext()->setContextProperty("menuItems", menuItems);
    engine.rootContext()->setContextProperty("mediaLibrary", mediaLibrary);
    engine.rootContext()->setContextProperty("defaultMenuItem", defaultMenuItem);
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
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
    delete(headunit);
    connectionListener->stop();
    return ret;
}
