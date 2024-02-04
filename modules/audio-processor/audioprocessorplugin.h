#ifndef AUDIOPROCESSOR_H
#define AUDIOPROCESSOR_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include <plugininterface.h>

#include "backends/audioprocessorinterface.h"
#include "backends/TDA7418/TDA7418.h"

class AudioProcessorPlugin : public QObject, PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.viktorgino.headunit.audio-processor" FILE "config.json")
    Q_PROPERTY(int settingsState MEMBER m_soundSettingsState NOTIFY settingsStateChanged)
    Q_PROPERTY(QQmlPropertyMap * Settings READ getSettings CONSTANT)
    Q_INTERFACES(PluginInterface)
public:
    explicit AudioProcessorPlugin(QObject *parent = nullptr);

    void init() override;

    Q_INVOKABLE void openOverlay();
    Q_INVOKABLE void setAudioParameter(QString parameter, int value);
    Q_INVOKABLE void onSettingsPageDestroyed();
    QObject *getContextProperty() override;

public slots:
    void actionMessage(QString id, QVariant message) override;
signals:
    void message(QString id, QVariant message);
    void action(QString id, QVariant message);
    void settingsStateChanged();

private slots:
    void closeOverlay();
private :
    int m_i2cDevNumber = 0;
    AudioProcessorInterface *m_audioBackend = nullptr;
    void settingsChanged(const QString &key, const QVariant &value);
    enum SoundSettingsState{
        DefaultState,
        Balance,
        Sub,
        Bass,
        Middle,
        Treble
    };
    int m_soundSettingsState = DefaultState;
    QTimer m_overlayTimeout;
};

#endif // AUDIOPROCESSOR_H
