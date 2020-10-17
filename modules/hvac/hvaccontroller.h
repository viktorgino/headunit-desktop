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

class HVACController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantMap settings READ settings NOTIFY settingsChanged)
    Q_PROPERTY(QQmlPropertyMap *parameters MEMBER m_parameters CONSTANT)


public:
    explicit HVACController(QDir settingsDir, std::map<QString, QVariant> settings, QObject *parent = nullptr);
    ~HVACController();

    enum HVACParameters{
        InvalidParameter,
        Windshield,
        FrontTop,
        FrontMiddle,
        FrontBottom,
        RearTop,
        RearMiddle,
        RearBottom,
        FrontDefrost,
        RearDefrost,
        AC,
        Recirculate,
        AutoAc,
        Power,
        Fan1,
        Fan2,
        Temperature1,
        Temperature2,
        SeatHeatLeft,
        SeatHeatRight
    };

    Q_ENUM(HVACParameters)
public slots:
    void toggleParameter(QString parameterString);
    void setParameterValue(QString parameterString, QVariant value);
    void sendMessage(QString message);
signals:
    void settingsChanged();
    void parameterChanged();
    void setParameter(HVACParameters parameter, QVariant value);
private slots:

    void executeTimedAction();
private:
    QSettings *carPlatformSettings;
    QJSEngine *hvacEngine;
    QTimer *timer;
    QVariantMap m_settings;
    QQmlPropertyMap *m_parameters;
    QList<HVACParameters> boolValues = {Windshield,FrontTop,FrontMiddle,FrontBottom,RearTop,RearMiddle,RearBottom,FrontDefrost,RearDefrost,AC,Recirculate,AutoAc,Power};
    QList<HVACParameters> intValues = {Fan1,Fan2,Temperature1,Temperature2,SeatHeatLeft,SeatHeatRight};

    void loadValues();
    QString parameterKeytoString(HVACParameters);
    HVACParameters stringToParameterKey(QString key);
    QVariantMap settings(){
        return m_settings;
    }

public slots:
};

#endif // HVACCONTROLLER_H
