/*
    Copyright (C) 2014 George Kiagiadakis <kiagiadakis.george@gmail.com>

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
#ifndef QGST_SEGMENT_H
#define QGST_SEGMENT_H

#include "global.h"

namespace QGst {

/*! \headerfile segment.h <QGst/Segment>
 * \brief Wrapper for GstSegment
 */
class QTGSTREAMER_EXPORT Segment
{
public:
    Segment();
    explicit Segment(Format fmt);
    explicit Segment(const GstSegment *segment);
    Segment(const Segment & other);
    virtual ~Segment();

    Segment & operator=(const Segment & other);

    bool isValid() const;
    void init(Format fmt);

    // fields
    SegmentFlags flags() const;

    double rate() const;
    double appliedRate() const;

    Format format() const;
    quint64 base() const;
    quint64 offset() const;
    quint64 start() const;
    quint64 stop() const;
    quint64 time() const;

    quint64 position() const;
    quint64 duration() const;

    operator GstSegment*();
    operator const GstSegment*() const;

private:
    struct Data;
    QSharedDataPointer<Data> d;
};

} //namespace QGst

QGST_REGISTER_TYPE(QGst::Segment)

#endif
