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
#include "graphicsvideowidget.h"
#include "graphicsvideosurface_p.h"
#include "../../QGlib/Signal"

namespace QGst {
namespace Ui {

GraphicsVideoWidget::GraphicsVideoWidget(QGraphicsItem *parent, Qt::WindowFlags wFlags)
    : QGraphicsWidget(parent, wFlags)
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);
}

GraphicsVideoWidget::~GraphicsVideoWidget()
{
    setSurface(0);
}

GraphicsVideoSurface *GraphicsVideoWidget::surface() const
{
    return m_surface.data();
}

void GraphicsVideoWidget::setSurface(GraphicsVideoSurface *surface)
{
    if (m_surface) {
        m_surface.data()->d->items.remove(this);
    }

    m_surface = surface;

    if (m_surface) {
        m_surface.data()->d->items.insert(this);
    }
}

void GraphicsVideoWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);

    QRectF r = rect();

    if (!m_surface || m_surface.data()->d->videoSink.isNull() ||
        widget != m_surface.data()->d->view->viewport()
    ) {
        painter->fillRect(r, Qt::black);
    } else {
        QGlib::emit<void>(m_surface.data()->d->videoSink, "paint",
                          (void*)painter, r.x(), r.y(), r.width(), r.height());
    }
}

} // namespace Ui
} // namespace QGst
