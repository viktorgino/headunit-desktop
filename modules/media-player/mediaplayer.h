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
    void init() override;
private:
    MediaLibrary mediaLibrary;

signals:
    void message(QString id, QVariant message);
};

#endif // MEDIAPLAYER_H

