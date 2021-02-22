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
#ifndef QGST_QUICK_VIDEOSURFACE_P_H
#define QGST_QUICK_VIDEOSURFACE_P_H

#include "videosurface.h"
#include "videoitem.h"

namespace QGst {
namespace Quick {

class QTGSTREAMERQUICK_NO_EXPORT VideoSurfacePrivate
{
public:
    QSet<VideoItem*> items;
    GstElement * videoSink;
};

} // namespace Quick
} // namespace QGst

#endif // QGST_QUICK_VIDEOSURFACE_P_H
