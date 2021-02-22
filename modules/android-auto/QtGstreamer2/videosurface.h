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
#ifndef QGST_QUICK_VIDEOSURFACE_H
#define QGST_QUICK_VIDEOSURFACE_H

#include "global.h"
#include <gst/gst.h>
#include <QtCore/QObject>

namespace QGst {
namespace Quick {

class VideoSurfacePrivate;

/*! \headerfile videosurface.h <QGst/Quick/VideoSurface>
 * \brief Helper class for painting video on a QtQuick2 VideoItem
 *
 * This is a helper class that represents a video surface on a QQuickView.
 * To use it, create a VideoItem from QML and connect it with this surface.
 *
 * Example:
 * \code
 * // in your C++ code
 * QQuickView *view = new QQuickView;
 * ...
 * QGst::Quick::VideoSurface *surface = new QGst::Quick::VideoSurface;
 * view->rootContext()->setContextProperty(QLatin1String("videoSurface"), surface);
 * ...
 * // and in your qml file:
 * import QtGStreamer 1.0
 * ...
 * VideoItem {
 *      id: video
 *      width: 320
 *      height: 240
 *      surface: videoSurface
 * }
 * \endcode
 *
 * \sa VideoItem
 */
class QTGSTREAMERQUICK_EXPORT VideoSurface : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(VideoSurface)
public:
    explicit VideoSurface(QObject *parent = 0);
    virtual ~VideoSurface();

    /*! Returns the video sink element that provides this surface's image.
     * The element will be constructed the first time that this function
     * is called. The surface will always keep a reference to this element.
     */
    GstElement *videoSink() const;

protected:
    QTGSTREAMERQUICK_NO_EXPORT void onUpdate();

private:
    friend class VideoItem;
    VideoSurfacePrivate * const d;
};

} // namespace Quick
} // namespace QGst

Q_DECLARE_METATYPE(QGst::Quick::VideoSurface*)

#endif // QGST_QUICK_VIDEOSURFACE_H
