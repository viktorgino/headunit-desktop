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

#ifndef QGST_QUICK_VIDEOITEM_H
#define QGST_QUICK_VIDEOITEM_H

#include "videosurface.h"
#include <QtQuick/QQuickItem>
#include <QtCore/QPointer>

namespace QGst {
namespace Quick {

/*! \headerfile videoitem.h <QGst/Quick/VideoItem>
 * \brief A QQuickItem for displaying video
 *
 * This is a QQuickItem subclass that can display video. To use it,
 * you have to create a VideoSurface and connect it with this
 * item using the setSurface() method or the surface property.
 * See the VideoSurface documentation for details and examples.
 *
 * \sa VideoSurface
 */
class QTGSTREAMERQUICK_EXPORT VideoItem : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(VideoItem)
    Q_PROPERTY(QGst::Quick::VideoSurface* surface READ surface WRITE setSurface)

public:
    explicit VideoItem(QQuickItem *parent = 0);
    virtual ~VideoItem();

    VideoSurface *surface() const;
    void setSurface(VideoSurface *surface);

protected:
    /*! Reimplemented from QQuickItem. */
    virtual QSGNode* updatePaintNode(QSGNode *oldNode,
                                     UpdatePaintNodeData *updatePaintNodeData);

private:
    struct Private;
    Private *const d;
};

} // namespace Quick
} // namespace QGst

#endif // QGST_QUICK_VIDEOITEM_H
