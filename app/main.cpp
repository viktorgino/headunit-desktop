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
    qDebug("%lld ms : Loading theme loader", time.elapsed());
    engine->load(QUrl(QStringLiteral("qrc:/loader.qml")));

    qDebug("%lld ms : Starting main loop", time.elapsed());
    int ret = app.exec();

    return ret;
}
