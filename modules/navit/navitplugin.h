#ifndef SAMPLEPLUGIN_H
#define SAMPLEPLUGIN_H

#include <plugininterface.h>

class SamplePlugin : public QObject, PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.viktorgino.headunit.sample" FILE "config.json")
    Q_INTERFACES(PluginInterface)
public:
    explicit SamplePlugin(QObject *parent = nullptr);
    QObject *getContextProperty() override;
    QQuickImageProvider *getImageProvider() override;
    QStringList eventListeners() override;
    QStringList events() override;
    QStringList actions() override;
private slots:
    void usbDeviceAdded(QString deviceDetails);
    void usbDeviceRemoved(QString deviceDetails);
signals:
    void message(QString id, QString message);
public slots:
    void eventMessage(QString id, QString message) override;
    void actionMessage(QString id, QString message) override;
};

#endif // SAMPLEPLUGIN_H
