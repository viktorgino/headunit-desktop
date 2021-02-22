/*
    Copyright (C) 2010  George Kiagiadakis <kiagiadakis.george@gmail.com>
    Copyright (C) 2010  Collabora Multimedia.
      @author Mauricio Piacentini <mauricio.piacentini@collabora.co.uk>

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
#ifndef QGST_VIDEOORIENTATION_H
#define QGST_VIDEOORIENTATION_H

#include "global.h"
#include "../QGlib/object.h"

namespace QGst {

/*! \interface VideoOrientation videoorientation.h <QGst/VideoOrientation>
 * \brief Wrapper class for GstVideoOrientation
 */
class QTGSTREAMER_EXPORT VideoOrientation : public QGlib::Interface
{
    QGST_WRAPPER(VideoOrientation)
public:
    bool horizontalFlipEnabled() const;
    bool verticalFlipEnabled() const;
    int horizontalCenter() const;
    int verticalCenter() const;
    bool enableHorizontalFlip(bool enabled);
    bool enableVerticalFlip(bool enabled);
    bool setHorizontalCenter(int center);
    bool setVerticalCenter(int center);
};

} //namespace QGst

QGST_REGISTER_TYPE(QGst::VideoOrientation)
QGLIB_REGISTER_INTERFACE(QGst::VideoOrientation)

#endif // QGST_VIDEOORIENTATION_H
