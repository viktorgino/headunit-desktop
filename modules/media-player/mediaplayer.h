#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QObject>
#include <plugininterface.h>

#include "medialibrary.h"
class MediaPlayerPlugin : public QObject, PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.viktorgino.headunit.mediaplayer" FILE "config.json")
    Q_INTERFACES(PluginInterface)
public:
    explicit MediaPlayerPlugin(QObject *parent = nullptr);
    ~MediaPlayerPlugin() override;
    QObject *getContextProperty() override;
    QQuickImageProvider *getImageProvider() override;
    QStringList eventListeners() override;
    QStringList events() override;
    QStringList actions() override;
private:
    MediaLibrary mediaLibrary;

signals:

public slots:
    void eventMessage(QString id, QString message) override;
    void actionMessage(QString id, QString message) override;
};

#endif // MEDIAPLAYER_H

