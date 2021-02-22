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
#ifndef QGST_UI_GRAPHICSVIDEOWIDGET_H
#define QGST_UI_GRAPHICSVIDEOWIDGET_H

#include "graphicsvideosurface.h"
#include <QtCore/QPointer>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
# include <QtWidgets/QGraphicsWidget>
#else
# include <QtGui/QGraphicsWidget>
#endif

namespace QGst {
namespace Ui {

/*! \headerfile graphicsvideowidget.h <QGst/Ui/GraphicsVideoWidget>
 * \brief A QGraphicsWidget for displaying video on a QGraphicsScene
 *
 * This is a QGraphicsWidget subclass that can display video. To use it,
 * you have to create a GraphicsVideoSurface and connect it with this
 * widget using the setSurface() method or the surface property.
 * See the GraphicsVideoSurface documentation for details and examples.
 *
 * \sa GraphicsVideoSurface
 */
class QTGSTREAMERUI_EXPORT GraphicsVideoWidget : public QGraphicsWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(GraphicsVideoWidget)
    Q_PROPERTY(QGst::Ui::GraphicsVideoSurface* surface READ surface WRITE setSurface)
public:
    explicit GraphicsVideoWidget(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);
    virtual ~GraphicsVideoWidget();

    /*! Reimplemented from QGraphicsWidget. Do not call directly. */
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    GraphicsVideoSurface *surface() const;
    void setSurface(GraphicsVideoSurface *surface);

private:
    QPointer<GraphicsVideoSurface> m_surface;
};

} // namespace Ui
} // namespace QGst

#endif // QGST_UI_GRAPHICSVIDEOWIDGET_H
