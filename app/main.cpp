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
#include <QTime>
#include <QTimer>
#include <unistd.h>

#include "pluginmanager.h"
#include "thememanager.h"

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

    QQmlApplicationEngine *engine = new QQmlApplicationEngine();

    QCommandLineParser parser;
    parser.setApplicationDescription("helper");
    parser.addHelpOption();
    parser.addVersionOption();
    
    QCommandLineOption pluginsOption(QStringList() << "p" << "plugins",
	QCoreApplication::translate("main", "Plugins to enable (defaults to all)"),
	QCoreApplication::translate("main", "plugins")
    );
    parser.addOption(pluginsOption);

    parser.process(app);
    QString p = parser.value(pluginsOption);
    bool whitelist = parser.isSet(pluginsOption);
    QStringList plugins;
    if (whitelist) {
        plugins = p.split(" ",Qt::SkipEmptyParts);
    }

    qDebug("%lld ms : loading plugins", time.elapsed());
    PluginManager pluginManager(engine, plugins, &app);

    qDebug("%lld ms : loading theme", time.elapsed());
    ThemeManager themeManager(engine,"default-theme", &app);

    QObject::connect(&pluginManager, &PluginManager::themeEvent, &themeManager, &ThemeManager::onEvent);

    qDebug("Loading took : %lld ms", time.elapsed());

    int ret = app.exec();

    return ret;
}
