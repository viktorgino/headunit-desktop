#ifndef HVACPLUGIN_H
#define HVACPLUGIN_H

#include <QObject>
#include <QSettings>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QJSEngine>
#include <QDir>
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

#include <plugininterface.h>

#include <hvac-common.h>

class HVACPlugin : public QObject, PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.viktorgino.headunit.hvac" FILE "config.json")
    Q_INTERFACES(PluginInterface)
    Q_PROPERTY(QVariantMap hvacSettings READ settings NOTIFY settingsChanged)
    Q_PROPERTY(QVariantMap parameters READ getHVACParameters NOTIFY parametersUpdated)
    Q_PROPERTY(QVariantMap carManufacturers READ carManufacturers NOTIFY carsUpdated)
    Q_PROPERTY(QVariantMap cars READ cars NOTIFY carsUpdated)
public:
    explicit HVACPlugin(QObject *parent = nullptr);
    void init() override;

    QObject *getContextProperty() override;
    ~HVACPlugin() override;


signals:
    void message(QString id, QVariant message);
    void action(QString id, QVariant message);

    void settingsChanged();
    void parametersUpdated();
    void carsUpdated();
    void hvacParmeterUpdate(ClimateControlCommandFrame commandFrame);
    void settingsProviderUpdated();

public slots:
    void actionMessage(QString id, QVariant message) override;
    void setZoneParameter(QString zone, QString parameter, QVariant value);
    void setParameter(QString parameter, bool value);
    void openOverlay(bool rightHand);
    void closeOverlay();
private slots:
    void settingsUpdated(QString key, QVariant value);
private:
    QVariantMap m_hvacSettings;
    ClimateControlCommandFrame m_commandFrame;
    QVariantMap m_hvacParameters;
    QVariantMap m_manufacturers;
    QVariantMap m_cars;

    void updateHVACParameters();
    void updateManufacturers();
    void loadHVACSettings(QString fileName);
    void resetHVACSettings();
    void updateCars();
    QVariantMap settings(){
        return m_hvacSettings;
    }
    QVariantMap carManufacturers(){
        return m_manufacturers;
    }
    QVariantMap cars(){
        return m_cars;
    }
    QVariantMap getHVACParameters(){
        return m_hvacParameters;
    }
};

#endif // HVACPLUGIN_H
