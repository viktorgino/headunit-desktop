#ifndef HVACCONTROLLER_H
#define HVACCONTROLLER_H

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

#include <hvac-common.h>

class HVACController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantMap settings READ settings NOTIFY settingsChanged)
    Q_PROPERTY(QVariantMap parameters READ getHVACParameters NOTIFY parametersUpdated)
    Q_PROPERTY(QVariantList carManufacturers READ getHVACParameters NOTIFY carsUpdated)

public:
    explicit HVACController(QObject *parent = nullptr);
    ~HVACController();

    void setHVACParameters(QVariant parameters);
public slots:
    void setZoneParameter(QString zone, QString parameter, QVariant value);
    void setParameter(QString parameter, bool value);
    void openOverlay(bool rightHand);
    void closeOverlay();

signals:
    void settingsChanged();
    void parametersUpdated();
    void carsUpdated();
    void hvacParmeterUpdate(ClimateControlCommandFrame commandFrame);
    void action(QString id, QVariant message);

private:
    void updateHVACParameters();
    QVariantMap m_settings;
    ClimateControlCommandFrame m_commandFrame;    
    QVariantMap m_hvacParameters;
    QVariantList m_manufacturers;

    QVariantMap settings(){
        return m_settings;
    }
    QVariantList carManufacturers(){
        return m_manufacturers;
    }
    QVariantMap getHVACParameters(){
        return m_hvacParameters;
    }
public slots:
};

#endif // HVACCONTROLLER_H
