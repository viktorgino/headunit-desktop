#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQuick/QQuickView>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlEngine>
#include <QJsonObject>
#include <QVariantList>
#include <QQmlComponent>
#include <QCommandLineParser>
#include <QThreadPool>
#include <QDir>
#include <QSettings>
#include <QtDebug>
#include <QLoggingCategory>
#include <unistd.h>
#include <QElapsedTimer>
#include <QTimer>
#include <systemd/sd-daemon.h>

#include "hudloader.h"

Q_DECLARE_LOGGING_CATEGORY(HEADUNIT)

int main(int argc, char *argv[])
{
    QElapsedTimer time;
    time.start();
    setbuf(stdout, nullptr);

    QCoreApplication::setOrganizationName("viktorgino");
    QCoreApplication::setOrganizationDomain("https://github.com/viktorgino/headunit-desktop");
    QCoreApplication::setApplicationName("viktorgino's HeadUnit Desktop");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);
    QLoggingCategory::setFilterRules("");
    QLoggingCategory::setFilterRules("qt.qml.connections.warning=false");

    QQmlApplicationEngine *engine = new QQmlApplicationEngine();

    QCommandLineParser parser;
    parser.setApplicationDescription("viktorgino's HeadUnit Desktop");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption pluginsOption(QStringList() << "p" << "plugins",
	QCoreApplication::translate("main", "Plugins to enable (defaults to all)"),
	QCoreApplication::translate("main", "plugins")
    );
    parser.addOption(pluginsOption);

    QCommandLineOption lazyLoadingOption(QStringList() << "l" << "lazy-loading",
                                     QCoreApplication::translate("main", "Load plugins and theme in separate threads (experimental)")
                                     );
    parser.addOption(lazyLoadingOption);

    parser.process(app);

    QString p = parser.value(pluginsOption);
    bool whitelist = parser.isSet(pluginsOption);
    QStringList plugins;
    if (whitelist) {
        plugins = p.split(" ",QString::SkipEmptyParts);
    }

    bool lazyLoading = parser.isSet(lazyLoadingOption);

    HUDLoader loader(engine, lazyLoading, plugins, &app);
    engine->rootContext()->setContextProperty("loader", &loader);
    qDebug("%lld ms : Loading theme loader", time.elapsed());
    engine->load(QUrl(QStringLiteral("qrc:/loader.qml")));

    // If service Type=notify the service is only considered ready once we send this
    sd_notify(0, "READY=1");

    uint64_t watchdogIntervalUs = 0;
    QTimer watchdogTimer;
    QObject::connect(&watchdogTimer, &QTimer::timeout, [](){
        sd_notify(0, "WATCHDOG=1");
    });

    // Service WatchdogSec must be set for this to return > 0
    if (sd_watchdog_enabled(0, &watchdogIntervalUs) > 0) {
        qDebug("Systemd watchdog is enabled with %lu us\n", watchdogIntervalUs);
        // Recommended reporting interval is half the watchdog interval
        watchdogTimer.start(watchdogIntervalUs / 2000);
    }

    qDebug("%lld ms : Starting main loop", time.elapsed());
    int ret = app.exec();

    return ret;
}
