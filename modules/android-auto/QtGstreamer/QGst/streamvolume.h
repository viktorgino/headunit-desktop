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
#ifndef QGST_STREAMVOLUME_H
#define QGST_STREAMVOLUME_H

#include "global.h"
#include "../QGlib/object.h"

namespace QGst {

/*! \headerfile streamvolume.h <QGst/StreamVolume>
 * \brief Wrapper class for GstStreamVolume
 */
class QTGSTREAMER_EXPORT StreamVolume : public QGlib::Interface
{
    QGST_WRAPPER(StreamVolume)
public:
    double volume(StreamVolumeFormat format = StreamVolumeFormatLinear) const;
    void setVolume(double value, StreamVolumeFormat format = StreamVolumeFormatLinear);

    bool isMuted() const;
    void setMuted(bool muted);

    static double convert(StreamVolumeFormat from, StreamVolumeFormat to, double value);
};

} //namespace QGst

QGST_REGISTER_TYPE(QGst::StreamVolume)
QGLIB_REGISTER_INTERFACE(QGst::StreamVolume)

#endif // QGST_STREAMVOLUME_H
