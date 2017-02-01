#include <QGuiApplication>
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
#include "headunit.h"
#include "usbconnectionlistener.h"

int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    QGst::init(&argc, &argv);
    int defaultMenuItem = 0;

    QVariantList menuItems;
    menuItems << QJsonObject {{"source","qrc:/qml/ClimateControl/CCLayout.qml"},{"image","icons/svg/thermometer.svg"},{"text","A/C"},{"color","#d32f2f"}}.toVariantMap()
              << QJsonObject {{"source","qrc:/qml/Radio/RadioLayout.qml"},{"image","icons/svg/radio-waves.svg"},{"text","Radio"},{"color","#fbc02d"}}.toVariantMap()
              << QJsonObject {{"source","qrc:/aaVideo.qml"},{"image","icons/svg/social-android.svg"},{"text","Android Auto"},{"color","#512da8"}}.toVariantMap()
              << QJsonObject {{"source",""},{"image","icons/svg/music-note.svg"},{"text","Media player"},{"color","#388e3c"}}.toVariantMap()
              << QJsonObject {{"source",""},{"image","icons/gear-a.png"},{"text","Settings"},{"color","#0288d1"}}.toVariantMap();
    QGst::Quick::VideoSurface surface;
    Headunit *headunit =  new Headunit(surface.videoSink());
    engine.rootContext()->setContextProperty("videoSurface1", &surface);
    engine.rootContext()->setContextProperty("headunit", headunit);
    engine.rootContext()->setContextProperty("menuItems", menuItems);
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    emit headunit->setMenuItem(defaultMenuItem);
    headunit->startHU();
    UsbConnectionListener *connectionListener = new UsbConnectionListener();
    headunit->setUsbConnectionListener(connectionListener);
    QThreadPool::globalInstance()->start(connectionListener);

    int ret = app.exec();
    delete(headunit);
    connectionListener->stop();
    return ret;
}
