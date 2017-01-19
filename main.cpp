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
#include "headunit.h"

int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    QGst::init(&argc, &argv);

    QVariantList menuItems;
    menuItems << QJsonObject {{"source","qrc:/qml/ClimateControl/CCLayout.qml"},{"image","icons/svg/thermometer.svg"},{"text","A/C"},{"color","#d32f2f"}}.toVariantMap()
              << QJsonObject {{"source","Item"},{"image","icons/gear-a.png"},{"text","Settings"},{"color","#fbc02d"}}.toVariantMap()
              << QJsonObject {{"source","qrc:/qml/Radio/RadioLayout.qml"},{"image","icons/svg/radio-waves.svg"},{"text","Radio"},{"color","#512da8"}}.toVariantMap()
              << QJsonObject {{"source","qrc:/aaVideo.qml"},{"image","icons/svg/social-android.svg"},{"text","Android Auto"},{"color","#388e3c"}}.toVariantMap()
              << QJsonObject {{"source","Item"},{"image","icons/svg/music-note.svg"},{"text","ASdsd"},{"color","#0288d1"}}.toVariantMap();
    QGst::Quick::VideoSurface surface;
    Headunit headunit(surface.videoSink());
    engine.rootContext()->setContextProperty("videoSurface1", &surface);
    engine.rootContext()->setContextProperty("headunit", &headunit);
    engine.rootContext()->setContextProperty("menuItems", menuItems);
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    int ret = app.exec();
    headunit.stop();
    return ret;
}
