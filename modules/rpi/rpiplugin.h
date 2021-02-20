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
    bool initialized;
    void onLoad();
    void applyBrightness(double v);
    int mapBrightness(double v);
public:
    explicit RPiPlugin(QObject *parent = nullptr);
    void init() override;
signals:
    void message(QString id, QString message);

public slots:
    void settingChanged(QString id, QVariant val);
};

#endif // RPIPLUGIN_H
