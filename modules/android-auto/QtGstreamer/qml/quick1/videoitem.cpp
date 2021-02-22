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
#include "videoitem.h"
#include "../../QGst/Ui/graphicsvideowidget.h"

VideoItem::VideoItem(QDeclarativeItem *parent)
    : QDeclarativeItem(parent)
{
    m_widget = new QGst::Ui::GraphicsVideoWidget(this);
}

VideoItem::~VideoItem()
{
}

QGst::Ui::GraphicsVideoSurface *VideoItem::surface() const
{
    return m_widget->surface();
}

void VideoItem::setSurface(QGst::Ui::GraphicsVideoSurface *surface)
{
    m_widget->setSurface(surface);
}

void VideoItem::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry)
{
    m_widget->setGeometry(newGeometry);
    QDeclarativeItem::geometryChanged(newGeometry, oldGeometry);
}
