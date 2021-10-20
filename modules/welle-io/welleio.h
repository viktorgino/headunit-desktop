#ifndef ODBCAR_H
#define ODBCAR_H

#include <QObject>
#include <QTranslator>
#include <QSettings>
#include <plugininterface.h>

#include <unistd.h>
#include "version.h"
#include "dab-constants.h"
#include "radio_controller.h"
#include "gui_helper.h"
#include "debug_output.h"
#include "waterfallitem.h"

class WelleIoPlugin : public QObject, PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.viktorgino.headunit.welleio" FILE "config.json")
    Q_INTERFACES(PluginInterface)

    Q_PROPERTY(CGUIHelper* GUIHelper READ guiHelper NOTIFY guiHelperChanged)
    Q_PROPERTY(CRadioController* RadioController READ radioController NOTIFY radioControllerChanged)
public:
    explicit WelleIoPlugin(QObject *parent = nullptr);
    ~WelleIoPlugin() override;
    QObject *getContextProperty() override;
    QQuickImageProvider *getImageProvider() override;
    void init() override;
    CGUIHelper *guiHelper();
    CRadioController *radioController();
signals:
    void radioControllerChanged();
    void guiHelperChanged();
    void motChanged();
public slots:
    void onSettingsPageDestroyed();
    void play(QString channel, QString title, quint32 service);
    void playLastStation();
private slots:
    void settingsChanged(const QString &key, const QVariant &value);
private:
    void loadWelleIo(QQmlApplicationEngine *engine);
    bool welleioError;

    CRadioController* m_radioController = nullptr;
    CGUIHelper *m_guiHelper = nullptr;
    QTranslator m_translator;
    bool m_settingsChanged;
    bool m_adapterChanged;
};

#endif // ODBCAR_H
