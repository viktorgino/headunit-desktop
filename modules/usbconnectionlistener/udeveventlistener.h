#ifndef UDEVEVENTLISTENER_H
#define UDEVEVENTLISTENER_H


#include <QObject>
#include <QString>
#include <QRunnable>
#include <QJsonDocument>
#include <QThread>
#include <QDirIterator>
#include <QDebug>
#include <QTextStream>

#include <libudev.h>
#include <mntent.h>

class UdevEventListener : public QThread
{
    Q_OBJECT
public:
    UdevEventListener(QObject *parent = nullptr);
    ~UdevEventListener();
    void run() override;
    void stop();
signals:
    void deviceAdded(QString path);
private :
    bool done = false;
};

#endif // UDEVEVENTLISTENER_H
