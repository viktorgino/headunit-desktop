#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQuick/QQuickView>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlEngine>
#include <QGst/Init>
#include <QGst/Quick/VideoSurface>
#include <QGst/Element>
#include <QGlib/RefPointer>
#include "headunit.h"

int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    QGst::init(&argc, &argv);


    QGst::Quick::VideoSurface *surface = new QGst::Quick::VideoSurface;
    Headunit *headunit = new Headunit(surface->videoSink());
    engine.rootContext()->setContextProperty("videoSurface1", surface);
    engine.rootContext()->setContextProperty("headunit", headunit);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    app.exec();
    headunit->stop();
    return 0;
}
