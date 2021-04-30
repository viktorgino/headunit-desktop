#include "mediaplayercoverimageprovider.h"

MediaPlayerCoverImageProvider::MediaPlayerCoverImageProvider() : QQuickImageProvider(QQuickImageProvider::Image)
{

}
QImage MediaPlayerCoverImageProvider::requestImage(const QString &id, QSize */*size*/, const QSize &/*requestedSize*/)
{
    QString filePath = id;
    filePath.replace("file://","");
    QFileInfo file(filePath);
    if (file.exists()) {

        QString path = file.absoluteFilePath();

        TagLib::MPEG::File f(path.toUtf8().data());
        if(f.ID3v2Tag()) {
            TagLib::ID3v2::AttachedPictureFrame *frame
              = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame*>(f.ID3v2Tag()->frameListMap()["APIC"].front());
            if(frame && !frame->picture().isNull()) {
                QByteArray dataBuf(frame->picture().data(),frame->picture().size());
                return QImage::fromData(dataBuf);
            }
        }
    }

    QDir artworkFolder(":/MediaPlayer/images/artwork");
    artworkFolder.setFilter(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    artworkFolder.setNameFilters(QStringList({ "*.jpg", "*.png"}));

    QFileInfoList list = artworkFolder.entryInfoList();

    if(list.size() <= 0){
        QPixmap pixmap(1,1);

        int r = QRandomGenerator::global()->bounded(256);
        int g = QRandomGenerator::global()->bounded(256);
        int b = QRandomGenerator::global()->bounded(256);
        pixmap.fill(QColor(r,g,b));
        return pixmap.toImage();
    }

    int randomNo = QRandomGenerator::global()->bounded(list.size());
    QFileInfo randomFile = list[randomNo];

    return QImage(randomFile.absoluteFilePath());
}
