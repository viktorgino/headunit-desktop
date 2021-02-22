/*
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
#include "clock.h"

#include <QtCore/QTime>
#include <gst/gst.h>

namespace QGst {

ClockPtr Clock::systemClock()
{
    return ClockPtr::wrap(gst_system_clock_obtain(), false);
}

ClockTime Clock::clockTime() const
{
    GstClockTime t = gst_clock_get_time(object<GstClock>());
    return t;
}

ClockTime Clock::resolution() const
{
    GstClockTime t = gst_clock_get_resolution(object<GstClock>());
    return t;
}

QTime Clock::time() const
{
    return clockTime().toTime();
}

}
