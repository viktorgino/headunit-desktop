#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQuick/QQuickView>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlEngine>
#include "dabHelper.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    DABHelper *dabHelper = new DABHelper();
    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("dabHelper", dabHelper);
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    return app.exec();
}
