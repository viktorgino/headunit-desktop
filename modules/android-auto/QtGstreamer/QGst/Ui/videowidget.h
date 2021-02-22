/*
    Copyright (C) 2010 George Kiagiadakis <kiagiadakis.george@gmail.com>
    Copyright (C) 2011 Collabora Ltd.
      @author George Kiagiadakis <george.kiagiadakis@collabora.co.uk>

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
#ifndef QGST_UI_VIDEOWIDGET_H
#define QGST_UI_VIDEOWIDGET_H

#include "global.h"
#include "../element.h"

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
# include <QtWidgets/QWidget>
#else
# include <QtGui/QWidget>
#endif

namespace QGst {
namespace Ui {

class AbstractRenderer;

/*! \headerfile videowidget.h <QGst/Ui/VideoWidget>
 * \brief A generic video widget that can embed a video sink
 *
 * This widget allows you to embed a video sink on a Qt user interface.
 *
 * There are two ways of using this widget:
 * \li Create a video sink yourself and set it with the setVideoSink() method.
 * This will work for all sinks that implement the VideoOverlay interface, plus
 * "qtvideosink", "qtglvideosink" and "qwidgetvideosink" (or "qt5videosink",
 * "qt5glvideosink" and "qwidget5videosink" in Qt5), which paint directly
 * on the widget.
 * \li Create a pipeline and let the widget watch the pipeline using the
 * watchPipeline() method. This will cause the widget to watch the bus for
 * the "prepare-window-handle" that all VideoOverlay sinks send right before
 * creating a window and will embed any sink that sends this message.
 * You need to make sure however that there can only be one video sink in
 * this pipeline. If there are more than one, you should handle them yourself
 * and set them with the setVideoSink() method to different widgets.
 *
 * Nearly all the methods of this class \em must be called from Qt's GUI thread.
 * Also, you cannot start or stop watching a pipeline that is in PLAYING state.
 * Doing so may crash the widget.
 *
 * This widget will always keep a strong reference to the element that it is given,
 * whether this is a video sink or a pipeline. If you want to destroy this element
 * or pipeline, you need to call releaseVideoSink() or stopPipelineWatch() respectively.
 *
 * \note Autoplug video sinks such as autovideosink are not supported due
 * to the complexity of handling them correctly. If you wish to use autovideosink,
 * you can either set it to READY state and get its child VideoOverlay element
 * or just watch the pipeline in which you plug it.
 */
class QTGSTREAMERUI_EXPORT VideoWidget : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(VideoWidget)
public:
    explicit VideoWidget(QWidget *parent = 0, Qt::WindowFlags f = 0);
    virtual ~VideoWidget();


    /*! Returns the video sink element that is currently providing this
     * widget's image, or a null ElementPtr if no sink has been set.
     */
    ElementPtr videoSink() const;

    /*! Sets the video sink element that is going to be embedded.
     * Any sink that implements the VideoOverlay interface will work, as well as
     * "qtvideosink", "qtglvideosink" and "qwidgetvideosink" (or "qt5videosink",
     * "qt5glvideosink" and "qwidget5videosink" in Qt5)
     * \note
     * \li This method \em must be called from Qt's GUI thread.
     * \li Passing a null ElementPtr has the same effect as calling releaseVideoSink().
     * \li You cannot set a new sink if the previous one has not been released first.
     */
    void setVideoSink(const ElementPtr & sink);

    /*! Detaches the current video sink from the widget and drops any references to it.
     * \note This method \em must be called from Qt's GUI thread.
     */
    void releaseVideoSink();


    /*! Starts watching a pipeline for any attached VideoOverlay sinks. If such
     * a sink is found while the pipeline prepares itself to start playing,
     * it is embedded to the widget.
     * \note
     * \li This method \em must be called from Qt's GUI thread.
     * \li Passing a null PipelinePtr has the same effect as calling stopPipelineWatch().
     * \li You cannot start watching a new pipeline if you don't stop watching the previous
     * one first with stopPipelineWatch().
     */
    void watchPipeline(const PipelinePtr & pipeline);

    /*! Stops watching a pipeline and also detaches the sink
     * that was discovered in the pipeline, if any.
     * \note This method \em must be called from Qt's GUI thread.
     */
    void stopPipelineWatch();

protected:
    virtual void paintEvent(QPaintEvent *event);

private:
    AbstractRenderer *d;
};

} //namespace Ui
} //namespace QGst

#endif // QGST_UI_VIDEOWIDGET_H
