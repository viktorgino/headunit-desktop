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

    QJsonObject menuItemAC
        {
            {"source","qrc:/qml/ClimateControl/CCLayout.qml"},
            {"image","icons/svg/thermometer.svg"},
            {"text","A/C"},
            {"color","#d32f2f"}
        };

    QJsonObject menuItemSettings
        {
            {"source","Item"},
            {"image","icons/gear-a.png"},
            {"text","Settings"},
            {"color","#fbc02d"}
        };

    QJsonObject menuItemRadio
        {
            {"source","qrc:/qml/Radio/RadioLayout.qml"},
            {"image","icons/svg/radio-waves.svg"},
            {"text","Radio"},
            {"color","#512da8"}
        };

    QJsonObject menuItemAA
        {
            {"source","qrc:/aaVideo.qml"},
            {"image","icons/svg/social-android.svg"},
            {"text","Android Auto"},
            {"color","#388e3c"}
        };

    QJsonObject menuItemMusic
        {
            {"source","Item"},
            {"image","icons/svg/social-note.svg"},
            {"text","ASdsd"},
            {"color","#0288d1"}
        };

    QVariantList menuItems;
    menuItems << menuItemAC.toVariantMap() << menuItemSettings.toVariantMap() << menuItemRadio.toVariantMap() << menuItemAA.toVariantMap() << menuItemMusic.toVariantMap();
    QGst::Quick::VideoSurface *surface = new QGst::Quick::VideoSurface;
    Headunit *headunit = new Headunit(surface->videoSink());
    engine.rootContext()->setContextProperty("videoSurface1", surface);
    engine.rootContext()->setContextProperty("headunit", headunit);
    engine.rootContext()->setContextProperty("menuItems", menuItems);
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    app.exec();
    headunit->stop();
    return 0;
}
