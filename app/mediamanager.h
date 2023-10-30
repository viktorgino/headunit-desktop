#ifndef MEDIAMANAGER_H
#define MEDIAMANAGER_H

#include <QObject>
#include <QHash>
#include <QSettings>
#include <QQmlPropertyMap>
#include <QDebug>
#include <QSettings>
#include <QMetaObject>
#include <QMetaMethod>

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

    void init();
    void addInterface(QString name, QObject *object);
    void mediaInput(QString input);

signals:
    void intefacesChanged();
    void activeMediaPlayerChanged();
    void message(QString sender, QString message, QVariant parameter);

public slots:
    void setActiveMediaPlayer(QString name);
    void start();
    void stop();
    void prevTrack();
    void nextTrack();
    void setVolume(uint8_t volume);
    void onMediaPositionChanged(quint32 position);
    void onTrackChanged(QVariantMap track);

private slots:
    void setVoiceVolume(QString interface, QVariant value);
    void setMediaVolume(QString interface, QVariant value);
    void playbackStartedHandler();

private:
    QHash<QString, MediaInterface *> m_mediaInterfaces;

    QString m_activeMediaPlayer;
    QQmlPropertyMap *m_mediaVolumes;
    QQmlPropertyMap *m_voiceVolumes;

    QStringList getInterfaces();
    QString getActiveMediaPlayer();
};

#endif // MEDIAMANAGER_H
