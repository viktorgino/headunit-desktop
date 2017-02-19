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
#include <unistd.h>
#include "headunit.h"
#include "usbconnectionlistener.h"
#include "medialibrary.h"
#ifdef HAVE_WELLEIO
#include "dab-constants.h"
#include "gui.h"

#define	DEFAULT_INI	".welle.io.ini"

RadioInterface *loadWelleIO(){
    // Default values
    QSettings	*dabSettings;		// ini file

    QString	initFileName;
    initFileName = QDir::homePath ();
    initFileName. append ("/");
    initFileName. append (QString (DEFAULT_INI));
    initFileName = QDir::toNativeSeparators (initFileName);

    if (!initFileName.endsWith (".ini"))
        initFileName.append (".ini");

    dabSettings =  new QSettings (initFileName, QSettings::IniFormat);
    uint8_t dabMode = dabSettings -> value ("dabMode", 1). toInt ();
    QString dabDevice = dabSettings -> value ("device", "dabstick"). toString ();
    QString dabBand = dabSettings -> value ("band", "BAND III"). toString ();

    return new RadioInterface (dabSettings,
                               dabDevice,
                               dabMode,
                               dabBand);
}

#endif
int main(int argc, char *argv[])
{

    setbuf(stdout, NULL);

    QApplication app(argc, argv);
    QQmlApplicationEngine engine;

    QGst::init(&argc, &argv);

    int defaultMenuItem = 3;
    QVariantList menuItems;
    menuItems << QJsonObject {{"source","qrc:/qml/ClimateControl/CCLayout.qml"},{"image","icons/svg/thermometer.svg"},{"text","A/C"},{"color","#f44336"}}.toVariantMap()
              << QJsonObject {{"source","qrc:/qml/Radio/RadioLayout.qml"},{"image","icons/svg/radio-waves.svg"},{"text","Radio"},{"color","#E91E63"}}.toVariantMap()
             #ifdef HAVE_WELLEIO
              << QJsonObject {{"source","qrc:/welleIO.qml"},{"image","icons/svg/radio-waves.svg"},{"text","DAB"},{"color","#9C27B0"}}.toVariantMap()
             #endif
              << QJsonObject {{"source","qrc:/aaVideo.qml"},{"image","icons/svg/social-android.svg"},{"text","Android Auto"},{"color","#673AB7"}}.toVariantMap()
              << QJsonObject {{"source","qrc:/qml/MediaPlayer/MediaPlayerLayout.qml"},{"image","icons/svg/music-note.svg"},{"text","Media player"},{"color","#3F51B5"}}.toVariantMap()
              << QJsonObject {{"source",""},{"image","icons/gear-a.png"},{"text","Settings"},{"color","#2196F3"}}.toVariantMap();
    QGst::Quick::VideoSurface surface;
    Headunit *headunit =  new Headunit(surface.videoSink());
    MediaLibrary *mediaLibrary = new MediaLibrary();
#ifdef HAVE_WELLEIO
    RadioInterface *MyRadioInterface = loadWelleIO();
    engine.rootContext()->setContextProperty("cppGUI", MyRadioInterface);
    engine.addImageProvider(QLatin1String("motslideshow"), MyRadioInterface->MOTImage);
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
    MyRadioInterface->~RadioInterface ();
#endif
    delete(headunit);
    connectionListener->stop();
    return ret;
}
