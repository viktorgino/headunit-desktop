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
#ifndef QGST_CLOCK_H
#define QGST_CLOCK_H

#include "object.h"
#include "clocktime.h"

namespace QGst {

/*! \headerfile clock.h <QGst/Clock>
 * \brief Wrapper class for GstClock
 */
class QTGSTREAMER_EXPORT Clock : public Object
{
    QGST_WRAPPER(Clock)
public:
    static ClockPtr systemClock();

    ClockTime resolution() const;
    ClockTime clockTime() const;
    QTime time() const;
};

}

QGST_REGISTER_TYPE(QGst::Clock)

#endif // QGST_CLOCK_H
