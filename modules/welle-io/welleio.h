#ifndef ODBCAR_H
#define ODBCAR_H

#include <QObject>
#include <plugininterface.h>

#include <unistd.h>
#include "CInputFactory.h"
#include "CRAWFile.h"
#include "CRTL_TCP_Client.h"
#include "DabConstants.h"
#include "CRadioController.h"
#include "CGUI.h"

class WelleIoWrapper : public QObject {

    Q_OBJECT
    Q_PROPERTY(CGUI *GUI READ gui NOTIFY guiChanged)
    Q_PROPERTY(CRadioController* RadioController READ radioController NOTIFY radioControllerChanged)
public:
    CGUI *gui();
    CRadioController *radioController();
    void setGui(CGUI * GUI);
    void setRadioController(CRadioController * radioController);
signals:
    void radioControllerChanged();
    void guiChanged();
private:
    CRadioController* m_radioController = nullptr;
    CGUI *m_GUI = nullptr;
};

class WelleIoPlugin : public QObject, PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.viktorgino.headunit.welleio" FILE "config.json")
    Q_INTERFACES(PluginInterface)
public:
    explicit WelleIoPlugin(QObject *parent = nullptr);
    QObject *getContextProperty() override;
    QQuickImageProvider *getImageProvider() override;
    QStringList eventListeners() override;
    QStringList events() override;
    QStringList actions() override;
private:
    void loadWelleIo(QQmlApplicationEngine *engine);
    bool welleioError;
    WelleIoWrapper *welleIoWrapper;
public slots:
    void eventMessage(QString id, QString message) override;
    void actionMessage(QString id, QString message) override;
};

#endif // ODBCAR_H
