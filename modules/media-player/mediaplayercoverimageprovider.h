#ifndef MEDIAPLAYERCOVERIMAGEPROVIDER_H
#define MEDIAPLAYERCOVERIMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QDebug>
#include <QFileInfo>
#include <QDirIterator>
#include <QRandomGenerator>
#include <QPainter>
#include <QRectF>

#include <taglib/taglib.h>
#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/tpropertymap.h>
#include <taglib/id3v1tag.h>
#include <taglib/id3v2tag.h>
#include <taglib/mpegfile.h>
#include <taglib/tmap.h>
#include <taglib/tbytevector.h>
#include <taglib/commentsframe.h>
#include <taglib/attachedpictureframe.h>
#include <string.h>

class MediaPlayerCoverImageProvider : public QQuickImageProvider
{
public:
    MediaPlayerCoverImageProvider();

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
private :
    QImage getImage(QString url);
    QImage getRandomImage();
};

#endif // MEDIAPLAYERCOVERIMAGEPROVIDER_H
