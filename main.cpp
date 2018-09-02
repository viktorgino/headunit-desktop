#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQuick/QQuickView>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlEngine>
#include <QJsonObject>
#include <QVariantList>
#include <QQmlComponent>
#include <QThreadPool>
#include <QDir>
#include <QSettings>
#include <QtDebug>
#include <QLoggingCategory>
#include <unistd.h>

#include "pluginmanager.h"

Q_DECLARE_LOGGING_CATEGORY(HEADUNIT)

int main(int argc, char *argv[])
{
    setbuf(stdout, nullptr);

    QCoreApplication::setOrganizationName("viktorgino");
    QCoreApplication::setOrganizationDomain("https://github.com/viktorgino/headunit-desktop");
    QCoreApplication::setApplicationName("viktorgino's HeadUnit Desktop");

    QApplication app(argc, argv);
    QLoggingCategory::setFilterRules("");
    QQmlApplicationEngine *engine = new QQmlApplicationEngine();

    QVariantList menuItems;
    QVariantList configItems;

    int defaultMenuItem = 4;

    PluginManager pluginManager;
    pluginManager.loadPlugins(engine);

    engine->rootContext()->setContextProperty("defaultMenuItem", defaultMenuItem);

    engine->load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    int ret = app.exec();

    delete &pluginManager;
    return ret;
}
