#ifndef MEDIAMANAGER_H
#define MEDIAMANAGER_H

#include <QObject>
#include <QHash>
#include <QSettings>
#include <QQmlPropertyMap>
#include <QDebug>

#include "../includes/mediainterface.h"

class MediaManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList Interfaces READ getInterfaces NOTIFY intefacesChanged)
    Q_PROPERTY(QString ActiveMediaPlayer READ getActiveMediaPlayer NOTIFY activeMediaPlayerChanged)
    Q_PROPERTY(QQmlPropertyMap * MediaVolumes MEMBER m_mediaVolumes CONSTANT)
    Q_PROPERTY(QQmlPropertyMap * VoiceVolumes MEMBER m_voiceVolumes CONSTANT)
public:
    explicit MediaManager(QObject *parent = nullptr);

    void addInterface(QString name, MediaInterface * object);


signals:
    void intefacesChanged();
    void activeMediaPlayerChanged();
public slots:
    void setActiveMediaPlayer(QString name);
    void start();
    void stop();
    void prevTrack();
    void nextTrack();
    void setVolume(uint8_t volume);

private slots:
    void setVoiceVolume(QString interface, QVariant value);
    void setMediaVolume(QString interface, QVariant value);

private:
    QHash<QString, MediaInterface *> m_mediaInterfaces;

    QString m_activeMediaPlayer;
    QQmlPropertyMap *m_mediaVolumes;
    QQmlPropertyMap *m_voiceVolumes;

    QStringList getInterfaces();
    QString getActiveMediaPlayer();
};

#endif // MEDIAMANAGER_H
