#ifndef MEDIALIBRARY_H
#define MEDIALIBRARY_H

#include <QObject>

class MediaLibrary : public QObject
{
    Q_OBJECT
public:
    explicit MediaLibrary(QObject *parent = 0);

signals:

public slots:
};

#endif // MEDIALIBRARY_H