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
#ifndef QGST_VIDEOOVERLAY_H
#define QGST_VIDEOOVERLAY_H

#include "global.h"
#include "../QGlib/object.h"
#include <QtGui/qwindowdefs.h>

class QRect;

namespace QGst {

/*! \interface VideoOverlay videooverlay.h <QGst/VideoOverlay>
 * \brief Wrapper class for GstVideoOverlay
 */
class QTGSTREAMER_EXPORT VideoOverlay : public QGlib::Interface
{
    QGST_WRAPPER(VideoOverlay)
public:
    void expose();
    void setWindowHandle(WId id);
    void enableEventHandling(bool enabled);
    bool setRenderRectangle(int x, int y, int width, int height);
    bool setRenderRectangle(const QRect & rect);

    static bool isPrepareWindowHandleMessage(const MessagePtr & msg);
};

} //namespace QGst

QGST_REGISTER_TYPE(QGst::VideoOverlay)
QGLIB_REGISTER_INTERFACE(QGst::VideoOverlay)

#endif // QGST_VIDEOOVERLAY_H
