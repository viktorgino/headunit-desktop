#ifndef ODBCAR_H
#define ODBCAR_H

#include <QObject>
#include <plugininterface.h>

#include <unistd.h>
#include "version.h"
#include "dab-constants.h"
#include "radio_controller.h"
#include "gui_helper.h"
#include "debug_output.h"
#include "waterfallitem.h"

class WelleIoWrapper : public QObject {

    Q_OBJECT
    Q_PROPERTY(CGUIHelper* GUIHelper READ guiHelper NOTIFY guiHelperChanged)
    Q_PROPERTY(CRadioController* RadioController READ radioController NOTIFY radioControllerChanged)
public:
    ~WelleIoWrapper();
    CGUIHelper *guiHelper();
    CRadioController *radioController();
    void setGuiHelper(CGUIHelper * m_guiHelper);
    void setRadioController(CRadioController * radioController);
signals:
    void radioControllerChanged();
    void guiHelperChanged();
    void motChanged();
private:
    CRadioController* m_radioController = nullptr;
    CGUIHelper *m_guiHelper = nullptr;
};

class WelleIoPlugin : public QObject, PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.viktorgino.headunit.welleio" FILE "config.json")
    Q_INTERFACES(PluginInterface)
public:
    explicit WelleIoPlugin(QObject *parent = nullptr);
    ~WelleIoPlugin() override;
    QObject *getContextProperty() override;
    QQuickImageProvider *getImageProvider() override;
    void init() override;
private:
    void loadWelleIo(QQmlApplicationEngine *engine);
    bool welleioError;
    WelleIoWrapper *welleIoWrapper;
    CRadioController* m_radioController = nullptr;
    CGUIHelper *m_guiHelper = nullptr;
};

#endif // ODBCAR_H
