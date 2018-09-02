#ifndef ODBCONTROLLER_H
#define ODBCONTROLLER_H

#include <QObject>
#include <QSettings>

class OdbController : public QObject
{
    Q_OBJECT
public:
    explicit OdbController(QObject *parent = nullptr);
private:
    QSettings settings;
signals:

public slots:
};

#endif // ODBCONTROLLER_H
