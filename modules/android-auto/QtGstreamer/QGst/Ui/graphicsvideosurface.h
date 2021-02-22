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
#ifndef QGST_UI_GRAPHICSVIDEOSURFACE_H
#define QGST_UI_GRAPHICSVIDEOSURFACE_H

#include "global.h"
#include "../element.h"

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
# include <QtWidgets/QGraphicsView>
#else
# include <QtGui/QGraphicsView>
#endif

namespace QGst {
namespace Ui {

class GraphicsVideoWidget;
class GraphicsVideoSurfacePrivate;

/*! \headerfile graphicsvideosurface.h <QGst/Ui/GraphicsVideoSurface>
 * \brief Helper class for painting video on a QGraphicsView
 *
 * This is a helper class that represents a video surface on a QGraphicsView.
 * This is not a QGraphicsItem, though, it is just a helper class to bind
 * the video sink to a specific view. To use it, create a GraphicsVideoWidget,
 * add it to your scene and connect it with this surface.
 *
 * Example
 * \code
 * QGraphicsScene *scene = new QGraphicsScene;
 * QGraphicsView *view = new QGraphicsView (scene);
 * view->setViewport(new QGLWidget); //recommended
 * QGst::Ui::GraphicsVideoSurface *surface = new QGst::Ui::GraphicsVideoSurface(view);
 * ...
 * QGst::Ui::GraphicsVideoWidget *widget = new QGst::Ui::GraphicsVideoWidget;
 * widget->setSurface(surface);
 * scene->addItem(widget);
 * \endcode
 *
 * This class internally creates and uses either a "qtglvideosink" or a "qtvideosink"
 * element ("qt5glvideosink" / "qt5videosink" in Qt5). This element is created the
 * first time it is requested and a reference is kept internally.
 *
 * To make use of OpenGL hardware acceleration (using qtglvideosink), you need to set
 * a QGLWidget as the viewport of the QGraphicsView. Note that you must do this before
 * the video sink element is requested for the first time using the videoSink() method,
 * as this method needs to find a GL context to be able to construct qtglvideosink and
 * query the hardware about supported features. Using OpenGL acceleration is recommended.
 * If you don't use it, painting will be done in software with QImage and QPainter
 * (using qtvideosink).
 *
 * This class can also be used to paint video on QML.
 *
 * Example:
 * \code
 * // in your C++ code
 * QDeclarativeView *view = new QDeclarativeView;
 * view->setViewport(new QGLWidget); //recommended
 * QGst::Ui::GraphicsVideoSurface *surface = new QGst::Ui::GraphicsVideoSurface(view);
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
 * \sa GraphicsVideoWidget
 */
class QTGSTREAMERUI_EXPORT GraphicsVideoSurface : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(GraphicsVideoSurface)
public:
    explicit GraphicsVideoSurface(QGraphicsView *parent);
    virtual ~GraphicsVideoSurface();

    /*! Returns the video sink element that provides this surface's image.
     * The element will be constructed the first time that this function
     * is called. The surface will always keep a reference to this element.
     */
    ElementPtr videoSink() const;

private:
    QTGSTREAMERUI_NO_EXPORT void onUpdate();

private:
    friend class GraphicsVideoWidget;
    GraphicsVideoSurfacePrivate * const d;
};

} // namespace Ui
} // namespace QGst

Q_DECLARE_METATYPE(QGst::Ui::GraphicsVideoSurface*)

#endif // QGST_UI_GRAPHICSVIDEOSURFACE_H
