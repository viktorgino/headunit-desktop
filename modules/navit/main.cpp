#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QRunnable>
#include <QThread>
#include <QThreadPool>
#include <QDebug>
#include <thread>

extern "C" {
#include "start_real.h"
#include "linguistics.h"
#include "debug.h"
}

void runNavit () {
    char *argv[] = {"navit", nullptr};
    int argc = sizeof(argv) / sizeof(char*) - 1;
//    setenv("NAVIT_LIBDIR","/home/gino/projects/navit/build/navit-bin/lib/",1);
//    setenv("NAVIT_LOCALEDIR","/home/gino/projects/navit/build/navit-bin/locale",1);
//    //setenv("NAVIT_USER_DATADIR","/home/gino/projects/navit/build/install/usr/local/share/navit",1);
//    setenv("NAVIT_SHAREDIR","/home/gino/projects/navit/build/navit-bin/share",1);

    setenv("NAVIT_LIBDIR","/home/gino/projects/build-navit-Desktop_Qt_5_11_1_GCC_64bit_system-Default/navit",1);
    setenv("NAVIT_SHAREDIR","/home/gino/projects/build-navit-Desktop_Qt_5_11_1_GCC_64bit_system-Default/navit",1);
    setenv("NAVIT_LOCALEDIR","/home/gino/projects/build-navit-Desktop_Qt_5_11_1_GCC_64bit_system-Default/locale",1);
    setenv("NAVIT_LIBPREFIX","*/.libs/",1);

    main_real(argc, argv);
}
void create_qml_gui(){

}
class HelloWorldTask : public QRunnable
{
    void run() override
    {
        runNavit();
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);


    qDebug() << "starting navit gui";

//    HelloWorldTask *hello = new HelloWorldTask();
//    QThreadPool::globalInstance()->start(hello);

//    QThread::sleep(2);
    runNavit();

    qDebug() << "starting qml gui";
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    int ret = app.exec();
#ifndef HAVE_API_ANDROID
    linguistics_free();
    debug_finished();
#endif
    return ret;

//    runNavit();
//    return 1;
}
