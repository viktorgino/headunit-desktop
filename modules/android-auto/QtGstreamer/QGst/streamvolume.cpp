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
#include "streamvolume.h"
#include <gst/audio/streamvolume.h>

namespace QGst {

double StreamVolume::volume(StreamVolumeFormat format) const
{
    return gst_stream_volume_get_volume(object<GstStreamVolume>(),
                                        static_cast<GstStreamVolumeFormat>(format));
}

void StreamVolume::setVolume(double value, StreamVolumeFormat format)
{
    gst_stream_volume_set_volume(object<GstStreamVolume>(),
                                 static_cast<GstStreamVolumeFormat>(format), value);
}

bool StreamVolume::isMuted() const
{
    return gst_stream_volume_get_mute(object<GstStreamVolume>());
}

void StreamVolume::setMuted(bool muted)
{
    gst_stream_volume_set_mute(object<GstStreamVolume>(), muted);
}

double StreamVolume::convert(StreamVolumeFormat from, StreamVolumeFormat to, double value)
{
    return gst_stream_volume_convert_volume(static_cast<GstStreamVolumeFormat>(from),
                                            static_cast<GstStreamVolumeFormat>(to), value);
}

} //namespace QGst
