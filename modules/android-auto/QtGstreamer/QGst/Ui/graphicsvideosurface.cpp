/*
    Copyright (C) 2012 Collabora Ltd. <info@collabora.com>
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
#include "graphicsvideosurface_p.h"
#include "../elementfactory.h"
#include "../../QGlib/connect.h"

#ifndef QTGSTREAMER_UI_NO_OPENGL
# include <QtOpenGL/QGLWidget>
#endif

namespace QGst {
namespace Ui {

GraphicsVideoSurface::GraphicsVideoSurface(QGraphicsView *parent)
    : QObject(parent), d(new GraphicsVideoSurfacePrivate)
{
    d->view = parent;
}

GraphicsVideoSurface::~GraphicsVideoSurface()
{
    if (!d->videoSink.isNull()) {
        d->videoSink->setState(QGst::StateNull);
    }

    delete d;
}

ElementPtr GraphicsVideoSurface::videoSink() const
{
    if (d->videoSink.isNull()) {
#ifndef QTGSTREAMER_UI_NO_OPENGL
        //if the viewport is a QGLWidget, profit from it
        QGLWidget *glw = qobject_cast<QGLWidget*>(d->view->viewport());
        if (glw) {
            d->videoSink = QGst::ElementFactory::make(QTGLVIDEOSINK_NAME);

            if (!d->videoSink.isNull()) {
                glw->makeCurrent();
                d->videoSink->setProperty("glcontext", (void*) QGLContext::currentContext());
                glw->doneCurrent();

                if (d->videoSink->setState(QGst::StateReady) != QGst::StateChangeSuccess) {
                    d->videoSink.clear();
                }
            }
        }
#endif

        if (d->videoSink.isNull()) {
            d->videoSink = QGst::ElementFactory::make(QTVIDEOSINK_NAME);

            if (d->videoSink.isNull()) {
                qCritical("Failed to create qtvideosink. Make sure it is installed correctly");
                return ElementPtr();
            }
        }

        QGlib::connect(d->videoSink, "update",
                       const_cast<GraphicsVideoSurface*>(this),
                       &GraphicsVideoSurface::onUpdate);
    }

    return d->videoSink;
}

void GraphicsVideoSurface::onUpdate()
{
    Q_FOREACH(GraphicsVideoWidget *item, d->items) {
        item->update(item->rect());
    }
}

} // namespace Ui
} // namespace QGst
