#ifndef RPIPLUGIN_H
#define RPIPLUGIN_H

#include <plugininterface.h>
#include <QDebug>

class RPiPlugin : public QObject, PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.viktorgino.headunit.rpiplugin" FILE "config.json")
    Q_INTERFACES(PluginInterface)
private:
    void applyBrightness(int v);
    int mapBrightness(int v);
public:
    explicit RPiPlugin(QObject *parent = nullptr);
    void init() override;
    void eventMessage(QString id, QVariant message) override;

public slots:
    void settingsChanged(const QString &key, const QVariant &value);
};

#endif // RPIPLUGIN_H
