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
#include "clocktime.h"

#include <QtCore/QTime>
#include <gst/gst.h>

namespace QGst {

QTime ClockTime::toTime() const
{
    ClockTime asSeconds = GST_TIME_AS_SECONDS(m_clocktime);
    ClockTime h = (asSeconds / 3600) % 24;
    ClockTime min = (asSeconds / 60) % 60;
    ClockTime sec = asSeconds % 60;
    ClockTime msec = GST_TIME_AS_MSECONDS(m_clocktime) % 1000;
    return QTime(h, min, sec, msec);
}

ClockTime ClockTime::fromTime(const QTime & time)
{
    return (time.hour()*3600*Q_UINT64_C(1000000000)) + (time.minute()*60*Q_UINT64_C(1000000000))
            + (time.second()*Q_UINT64_C(1000000000)) + (time.msec()*Q_UINT64_C(1000000));
}

} //namespace QGst
