/*
    Copyright (C) 2011 Collabora Ltd. <info@collabora.co.uk>
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
#ifndef QGST_CLOCKTIME_H
#define QGST_CLOCKTIME_H

#include "global.h"
class QTime;

namespace QGst {

/*! \relates ClockTime
 * A datatype to hold a time difference, measured in nanoseconds
 */
typedef qint64 ClockTimeDiff;


/*! \headerfile clocktime.h <QGst/ClockTime>
 * \brief A datatype to hold a time, measured in nanoseconds
 *
 * This is a thin wrapper around a GstClockTime, which is actually a quint64.
 */
class QTGSTREAMER_EXPORT ClockTime
{
public:
    /*! Represents an invalid time. \sa isValid() */
    static const quint64 None = -1;

    inline ClockTime(quint64 t = None) : m_clocktime(t) {}
    inline operator quint64() const { return m_clocktime; }

    /*! Returns true if this ClockTime is valid, i.e. it is not equal to ClockTime::None */
    inline bool isValid() const { return m_clocktime != None; }

    /*! This method allows you to convert this ClockTime to a QTime.
     * Be careful, as the conversion will lose some precision. ClockTime
     * holds nanoseconds, while QTime is only able to hold milliseconds.
     */
    QTime toTime() const;

    /*! Creates a ClockTime from a QTime */
    static ClockTime fromTime(const QTime & time);

    /*! Creates a ClockTime from seconds */
    inline static ClockTime fromSeconds(quint64 seconds);

    /*! Creates a ClockTime from milli seconds */
    inline static ClockTime fromMSecs(quint64 msec);

    /*! Creates a ClockTime from micro seconds */
    inline static ClockTime fromUSecs(quint64 usec);

private:
    quint64 m_clocktime;
};

inline ClockTime ClockTime::fromSeconds(quint64 seconds)
{
    return ClockTime(seconds * 1000 * 1000 * 1000);
}

inline ClockTime ClockTime::fromMSecs(quint64 msec)
{
    return ClockTime(msec * 1000 * 1000);
}

inline ClockTime ClockTime::fromUSecs(quint64 usec)
{
    return ClockTime(usec * 1000);
}


} //namspace QGst

QGST_REGISTER_TYPE(QGst::ClockTime)

#endif // QGST_CLOCKTIME_H
