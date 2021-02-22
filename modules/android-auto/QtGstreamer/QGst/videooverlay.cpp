/*
    Copyright (C) 2010  George Kiagiadakis <kiagiadakis.george@gmail.com>

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

#include "videooverlay.h"
#include "message.h"
#include <gst/video/videooverlay.h>
#include <QtCore/QRect>

namespace QGst {

void VideoOverlay::expose()
{
    gst_video_overlay_expose(object<GstVideoOverlay>());
}

void VideoOverlay::setWindowHandle(WId id)
{
#if defined(Q_WS_WIN)
    QGLIB_STATIC_ASSERT(sizeof(WId) == sizeof(guintptr),
                        "Size of WId doesn't match guintptr. Please file a bug report.");
    gst_video_overlay_set_window_handle(object<GstVideoOverlay>(), *reinterpret_cast<guintptr*>(&id));
#else
    gst_video_overlay_set_window_handle(object<GstVideoOverlay>(), id);
#endif
}

void VideoOverlay::enableEventHandling(bool enabled)
{
    gst_video_overlay_handle_events(object<GstVideoOverlay>(), enabled);
}

bool VideoOverlay::setRenderRectangle(int x, int y, int width, int height)
{
    return gst_video_overlay_set_render_rectangle(object<GstVideoOverlay>(), x, y, width, height);
}

bool VideoOverlay::setRenderRectangle(const QRect& rect)
{
    return setRenderRectangle(rect.x(), rect.y(), rect.width(), rect.height());
}

bool VideoOverlay::isPrepareWindowHandleMessage(const MessagePtr & msg)
{
    return gst_is_video_overlay_prepare_window_handle_message(msg);
}

} //namespace QGst
