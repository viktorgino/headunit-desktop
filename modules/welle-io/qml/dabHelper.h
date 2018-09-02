#ifndef DAB_H
#define DAB_H

#include <QObject>
#include <QCursor>
#include <QPoint>

class DABHelper : public QObject
{
    Q_OBJECT
public:
    explicit DABHelper(QObject *parent = 0);
    Q_INVOKABLE void changeMousePos(int x, int y);
signals:

public slots:

};

#endif // DAB_H
