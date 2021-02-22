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

#include "../elementfactory.h"
#include "../../QGlib/connect.h"

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
    if (!d->videoSink.isNull()) {
        d->videoSink->setState(QGst::StateNull);
    }

    delete d;
}

GstElement *VideoSurface::videoSink() const
{
    if (d->videoSink.isNull()) {
//        d->videoSink = QGst::ElementFactory::make("qtquick2videosink");
        GstElement *e = gst_element_factory_make("qtquick2videosink", NULL);
        d->videoSink = e;
        if (e) {
            gst_object_ref_sink(e);
        }
        if (d->videoSink.isNull()) {
            qCritical("Failed to create qtquick2videosink. Make sure it is installed correctly");
            return nullptr;
        }

        QGlib::connect(d->videoSink, "update",
                       const_cast<VideoSurface*>(this),
                       &VideoSurface::onUpdate);

        QObject *qreceiver = reinterpret_cast<QObject*>(d->videoSink);
        return QObject::connect(qreceiver, SIGNAL(destroyed(QObject*)),
                                const_cast<VideoSurface*>(this), &VideoSurface::onUpdate, Qt::DirectConnection);

    }

    return d->videoSink;
}

void VideoSurface::onUpdate()
{
    Q_FOREACH(QQuickItem *item, d->items) {
        item->update();
    }
}

} // namespace Quick
} // namespace QGst
