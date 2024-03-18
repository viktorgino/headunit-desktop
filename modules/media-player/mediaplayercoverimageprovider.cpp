#include "mediaplayercoverimageprovider.h"

MediaPlayerCoverImageProvider::MediaPlayerCoverImageProvider() : QQuickImageProvider(QQuickImageProvider::Image)
{

}
QImage MediaPlayerCoverImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    QStringList idList = id.split("/");
    if(idList.size() > 0 && idList[0] == "background") {
        QString path = id;
        path.replace("background/", "");
        QImage originalImage = getImage(path);
        if(originalImage.isNull()){
            originalImage = getRandomImage();
        }

        QSize imageSize(1440,720);

        QImage image = originalImage.scaled(imageSize.height(),imageSize.height(), Qt::KeepAspectRatioByExpanding);

        QColor bgColor = originalImage.scaled(1,1).pixelColor(0,0);
        QColor bgTransparentColor(bgColor);
        bgTransparentColor.setAlpha(0);

        QImage finalImage(imageSize.width(), imageSize.height(), QImage::Format_RGB32);
        QPainter painter(&finalImage);

        painter.drawImage(QRectF(imageSize.width() * 0.25, -imageSize.width() * 0.25, imageSize.width() * 0.75, imageSize.width() * 0.75),image);
        QLinearGradient gradient(0,0, imageSize.width() / 2,0);
        gradient.setColorAt(0, bgColor);
        gradient.setColorAt(0.75, bgColor);
        gradient.setColorAt(1, bgTransparentColor);

        painter.fillRect(QRectF(0, 0, imageSize.width()/2, imageSize.height()), gradient);

        painter.fillRect(QRectF(0, 0, imageSize.width(), imageSize.height()), "#80000000");


        return finalImage;
    } else if(id == "random"){
        return getRandomImage();
    } else {
        QImage ret = getImage(id);
        if(ret.isNull()){
            QPixmap pixmap(500,500);

            int r = QRandomGenerator::global()->bounded(256);
            int g = QRandomGenerator::global()->bounded(256);
            int b = QRandomGenerator::global()->bounded(256);
            pixmap.fill(QColor(r,g,b));

            QPainter painter(&pixmap);

            QImage noteImage(":/MediaPlayer/images/musical-notes-sharp.svg");
            painter.drawImage(QRectF(100, 100, 300, 300),noteImage);
            return pixmap.toImage();
        }
        return ret;
    }
}

QImage MediaPlayerCoverImageProvider::getRandomImage() {
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

QImage MediaPlayerCoverImageProvider::getImage(QString url) {
    url.replace("file://","");
    QFileInfo file(url);
    if (file.exists() && file.completeSuffix() == "mp3") {
        QString path = file.absoluteFilePath();

        TagLib::MPEG::File f(path.toUtf8().data());
        if(f.ID3v2Tag()) {
            TagLib::ID3v2::AttachedPictureFrame *frame
              = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame*>(f.ID3v2Tag()->frameListMap()["APIC"].front());
            if(frame && !frame->picture().isEmpty()) {
                QByteArray dataBuf(frame->picture().data(),frame->picture().size());
                return QImage::fromData(dataBuf);
            }
        }
    }
    return QImage();
}
