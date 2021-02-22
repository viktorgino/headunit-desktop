/*
    Copyright (C) 2012-2013 Collabora Ltd. <info@collabora.com>
      @author George Kiagiadakis <george.kiagiadakis@collabora.com>

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "videosurface_p.h"

#include <QtCore/QDebug>
#include <QtQuick/QQuickItem>

namespace QGst {
namespace Quick {

VideoSurface::VideoSurface(QObject *parent)
    : QObject(parent), d(new VideoSurfacePrivate)
{
}

VideoSurface::~VideoSurface()
{
    qDebug() << "Deleting VideoSurface";
    if (d->videoSink != nullptr) {
        gst_element_set_state(d->videoSink, GST_STATE_NULL);
    }

    delete d;
}

GstElement *VideoSurface::videoSink() const
{
    if (d->videoSink == nullptr) {
        d->videoSink = gst_element_factory_make("qtquick2videosink", NULL);

        if (d->videoSink == nullptr) {
            qCritical("Failed to create qtquick2videosink. Make sure it is installed correctly");
            return nullptr;
        } else {
            qDebug() << "qtquick2videosink created" << d->videoSink;

            gst_object_ref_sink(d->videoSink);
        }
        g_signal_connect(d->videoSink, "update", G_CALLBACK(&VideoSurface::updateHandler), (gpointer) this);

    } else {
        qDebug("VideoSurface already exists");
    }
    qDebug() << this;

    return d->videoSink;
}

void VideoSurface::updateHandler(GstElement * , gpointer __this){
    const VideoSurface *vidSurface = (const VideoSurface *)__this;

    vidSurface->onUpdate();
}
void VideoSurface::onUpdate() const
{
    for(QQuickItem *item : d->items){
        item->update();
    }
}

} // namespace Quick
} // namespace QGst
