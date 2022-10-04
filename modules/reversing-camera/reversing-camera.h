#ifndef SAMPLEPLUGIN_H
#define SAMPLEPLUGIN_H

#include <QDebug>
#include <plugininterface.h>

#include <QAbstractVideoBuffer>
#include <QAbstractVideoSurface>
#include <QVideoSurfaceFormat>
#include <QVideoFrame>

#include <gst/app/gstappsink.h>
#include <gst/app/gstappsrc.h>
#include <gst/gst.h>

#include "qgstvideobuffer.h"

class ReversingCamera : public QObject, PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.viktorgino.headunit.reversing-camera" FILE "config.json")
    Q_INTERFACES(PluginInterface)
    Q_PROPERTY(QAbstractVideoSurface *videoSurface READ videoSurface WRITE setVideoSurface)
public:
    explicit ReversingCamera(QObject *parent = nullptr);

    void init() override;
    QObject *getContextProperty() override;

    QAbstractVideoSurface *videoSurface() const {
        return m_surface;
    }
    void setVideoSurface(QAbstractVideoSurface *surface);

signals:
    void message(QString id, QVariant message);
    void receivedVideoFrame(const QVideoFrame &frame);
public slots: 
    void onSettingsPageDestroyed();
private slots :
    void videoFrameHandler(const QVideoFrame &frame);
private:
    QAbstractVideoSurface *m_surface = nullptr;
    GstElement *m_vid_pipeline = nullptr;
    bool m_videoStarted = false;

    static GstFlowReturn newVideoSample(GstElement *appsink, ReversingCamera *_this);
    void setupPipeline();
};

#endif // SAMPLEPLUGIN_H
