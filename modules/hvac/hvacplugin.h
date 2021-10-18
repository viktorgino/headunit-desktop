#ifndef HVACPLUGIN_H
#define HVACPLUGIN_H

#include <QObject>
#include <QSettings>
#include <QDebug>
#include <QJSEngine>
#include <QTimer>
#include <QList>
#include <QThread>
#include <QRunnable>
#include <QQmlPropertyMap>
#include <QThreadPool>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QQmlPropertyMap>

#include <plugininterface.h>

#include <hvac-common.h>

class HVACPlugin : public QObject, PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.viktorgino.headunit.hvac" FILE "config.json")
    Q_INTERFACES(PluginInterface)
    Q_PROPERTY(QVariantMap hvacSettings READ settings NOTIFY settingsChanged)
    Q_PROPERTY(QVariantMap parameters READ getHVACParameters NOTIFY parametersUpdated)
public:
    explicit HVACPlugin(QObject *parent = nullptr);
    void init() override;

    QObject *getContextProperty() override;
    ~HVACPlugin() override;

signals:
    void message(QString id, QVariant message);
    void action(QString id, QVariant message);
    void parametersUpdated();
    void hvacParmeterUpdate(ClimateControlCommandFrame commandFrame);
    void settingsChanged();

public slots:
    void actionMessage(QString id, QVariant message) override;
    void setZoneParameter(QString zone, QString parameter, QVariant value);
    void setParameter(QString parameter, bool value);
    void openOverlay(bool rightHand);
    void closeOverlay();

private:
    QVariantMap m_hvacSettings;
    ClimateControlCommandFrame m_commandFrame;
    QVariantMap m_hvacParameters;

    void updateHVACParameters();
    void resetHVACSettings();
    QVariantMap settings(){
        return m_hvacSettings;
    }
    QVariantMap getHVACParameters(){
        return m_hvacParameters;
    }
};

#endif // HVACPLUGIN_H
