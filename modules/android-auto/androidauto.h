#ifndef ANDROIDAUTO_H
#define ANDROIDAUTO_H

#include <QObject>
#include <QThreadPool>
#include <plugininterface.h>
#include "headunit.h"

#include <QGst/Init>
#include <QGst/Quick/VideoSurface>
#include <QGst/Element>
#include <QGlib/RefPointer>

class AndroidAuto : public QObject, PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.viktorgino.headunit.androidauto" FILE "config.json")
    Q_INTERFACES(PluginInterface)
public:
    explicit AndroidAuto(QObject *parent = nullptr);
    ~AndroidAuto() override;
    QObject *getContextProperty() override;
    QQuickImageProvider *getImageProvider() override;
    QStringList eventListeners() override;
    QStringList events() override;
    QStringList actions() override;
private:

signals:

public slots:
    void eventMessage(QString id, QString message) override;
    void actionMessage(QString id, QString message) override;
private:
    Headunit *headunit;
    QGst::Quick::VideoSurface *surface;
};

#endif // ANDROIDAUTO_H
