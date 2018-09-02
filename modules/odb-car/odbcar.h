#ifndef ODBCAR_H
#define ODBCAR_H

#include <QObject>
#include <plugininterface.h>

class OdbCar : public QObject, PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.viktorgino.headunit.odbcar" FILE "config.json")
    Q_INTERFACES(PluginInterface)
public:
    explicit OdbCar(QObject *parent = nullptr);
    QObject *getContextProperty() override;
    QQuickImageProvider *getImageProvider() override;
    QStringList eventListeners() override;
    QStringList events() override;
    QStringList actions() override;
private:

signals:

public slots:
    void eventMessage(QString id, QString message) override;
    void actionMessage(QString id, QString message) override;
};

#endif // ODBCAR_H
