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
#include "medialibrary.h"
#include <QQuickView>
#include <qmllive/livenodeengine.h>
#include <qmllive/remotereceiver.h>

class CustomQmlEngine : public QQmlEngine
{
    Q_OBJECT

public:
    CustomQmlEngine();
    Headunit *headunit;
    UsbConnectionListener *connectionListener;
};
class RuntimeLiveNodeEngine : public LiveNodeEngine
{
    Q_OBJECT

public:
    RuntimeLiveNodeEngine()
    {
        connect(this, &LiveNodeEngine::activeWindowChanged,
                this, &RuntimeLiveNodeEngine::onActiveWindowChanged);
    }

private slots:
    void onActiveWindowChanged(QQuickWindow *activeWindow)
    {
        if (activeWindow == 0)
            return;
        activeWindow->setFlags(activeWindow->flags() | Qt::WindowStaysOnTopHint);
        activeWindow->setColor(QColor(127,127,127));
    }
};
int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);
    QGst::init(&argc, &argv);

    QGuiApplication app(argc, argv);

    CustomQmlEngine qmlEngine;
    QQuickView fallbackView(&qmlEngine, 0);

    RuntimeLiveNodeEngine node;
    // Let qml live know your runtime
    node.setQmlEngine(&qmlEngine);
    // Allow it to display QML components with non-QQuickWindow root object
    node.setFallbackView(&fallbackView);
    // Tell it where file updates should be stored relative to
    node.setWorkspace("/home/gino/headunit-desktop",
                      LiveNodeEngine::AllowUpdates | LiveNodeEngine::UpdatesAsOverlay);
    qDebug()<<"The applicationDirPath path is: "<<app.applicationDirPath();
    // Listen to ipc call from remote QmlLiveBench
    RemoteReceiver receiver;
    receiver.registerNode(&node);
    receiver.listen(10234);

    int ret = app.exec();
    delete(qmlEngine.headunit);
    qmlEngine.connectionListener->stop();
    return ret;
}
CustomQmlEngine::CustomQmlEngine()
{
    int defaultMenuItem = 3;

    QVariantList menuItems;
    menuItems << QJsonObject {{"source","qrc:/qml/ClimateControl/CCLayout.qml"},{"image","icons/svg/thermometer.svg"},{"text","A/C"},{"color","#d32f2f"}}.toVariantMap()
              << QJsonObject {{"source","qrc:/qml/Radio/RadioLayout.qml"},{"image","icons/svg/radio-waves.svg"},{"text","Radio"},{"color","#fbc02d"}}.toVariantMap()
              << QJsonObject {{"source",""},{"image","icons/svg/social-android.svg"},{"text","Android Auto"},{"color","#512da8"}}.toVariantMap()
              << QJsonObject {{"source","qrc:/qml/MediaPlayer/MediaPlayerLayout.qml"},{"image","icons/svg/music-note.svg"},{"text","Media player"},{"color","#388e3c"}}.toVariantMap()
              << QJsonObject {{"source",""},{"image","icons/gear-a.png"},{"text","Settings"},{"color","#0288d1"}}.toVariantMap();
    QGst::Quick::VideoSurface surface;
    headunit =  new Headunit(surface.videoSink());
    MediaLibrary *mediaLibrary = new MediaLibrary();
    rootContext()->setContextProperty("videoSurface1", &surface);
    rootContext()->setContextProperty("headunit", headunit);
    rootContext()->setContextProperty("menuItems", menuItems);
    rootContext()->setContextProperty("mediaLibrary", mediaLibrary);
    rootContext()->setContextProperty("defaultMenuItem", defaultMenuItem);
    headunit->startHU();
    connectionListener = new UsbConnectionListener();
    headunit->setUsbConnectionListener(connectionListener);
    QThreadPool::globalInstance()->start(connectionListener);
}
#include "main.moc"
