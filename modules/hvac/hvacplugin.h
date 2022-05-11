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
#include <QFile>

#include <plugininterface.h>

#include <hvac-common.h>
#include <hud-common.h>

class HVACPlugin : public QObject, PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.viktorgino.headunit.hvac" FILE "config.json")
    Q_INTERFACES(PluginInterface)
    Q_PROPERTY(QVariantMap hvacSettings READ settings NOTIFY settingsChanged)
    Q_PROPERTY(QVariantMap parameters READ getHVACParameters NOTIFY parametersUpdated)
    Q_PROPERTY(QVariantList bottomBarItems READ getBottomBarItems NOTIFY bottomBarItemsChanged)
    Q_PROPERTY(QString source READ getSource NOTIFY sourceChanged)
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
    void bottomBarItemsChanged();
    void sourceChanged();

public slots:
    void actionMessage(QString id, QVariant message) override;
    void setZoneParameter(QString zone, QString parameter, QVariant value);
    void setParameter(QString parameter, bool value);
    void openOverlay(bool rightHand);
    void closeOverlay();

private:
    QVariantList m_bottomBarItems;
    QVariantMap m_hvacSettings;
    ClimateControlCommandFrame m_commandFrame;
    QVariantMap m_hvacParameters;
    QString m_source;

    void updateHVACParameters();
    void resetHVACSettings();
    void loadBottomBarSettings();
    void loadSettings();
    void saveSettings();
    void setMenuItem();
    QVariantMap settings(){
        return m_hvacSettings;
    }
    QVariantMap getHVACParameters(){
        return m_hvacParameters;
    }
    QVariantList getBottomBarItems(){
        return m_bottomBarItems;
    }
    QString getSource(){
        return m_source;
    }
};

#endif // HVACPLUGIN_H
