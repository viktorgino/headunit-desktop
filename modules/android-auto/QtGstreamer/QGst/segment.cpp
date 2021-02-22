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
#include "segment.h"
#include <gst/gst.h>

namespace QGst {

#ifndef DOXYGEN_RUN

struct QTGSTREAMER_NO_EXPORT Segment::Data : public QSharedData
{
    Data() : QSharedData(), segment(NULL) {}
    Data(const Data & other);
    virtual ~Data();

    GstSegment *segment;
};

Segment::Data::Data(const Segment::Data & other)
    : QSharedData(other), segment(NULL)
{
    if (other.segment) {
        segment = gst_segment_copy(other.segment);
    }
}

Segment::Data::~Data()
{
    if (segment) {
        gst_segment_free(segment);
    }
}

#endif //DOXYGEN_RUN

Segment::Segment()
    : d(new Data)
{
}

Segment::Segment(Format fmt)
    : d(new Data)
{
    d->segment = gst_segment_new();
    init(fmt);
}

Segment::Segment(const GstSegment * segment)
    : d(new Data)
{
    d->segment = gst_segment_copy(segment);
}

Segment::Segment(const Segment & other)
    : d(other.d)
{
}

Segment & Segment::operator=(const Segment & other)
{
    d = other.d;
    return *this;
}

Segment::~Segment()
{
}

bool Segment::isValid() const
{
    return d->segment != NULL;
}

void Segment::init(Format fmt)
{
    gst_segment_init(d->segment, static_cast<GstFormat>(fmt));
}

SegmentFlags Segment::flags() const
{
    return d->segment ? static_cast<SegmentFlag>(d->segment->flags) : SegmentFlagNone;
}

double Segment::rate() const
{
    return d->segment ? d->segment->rate : 1.0;
}

double Segment::appliedRate() const
{
    return d->segment ? d->segment->applied_rate : 1.0;
}

Format Segment::format() const
{
    return d->segment ? static_cast<Format>(d->segment->format) : FormatUndefined;
}

quint64 Segment::base() const
{
    return d->segment ? d->segment->base : 0;
}

quint64 Segment::offset() const
{
    return d->segment ? d->segment->offset : 0;
}

quint64 Segment::start() const
{
    return d->segment ? d->segment->start : 0;
}

quint64 Segment::stop() const
{
    return d->segment ? d->segment->stop : -1;
}

quint64 Segment::time() const
{
    return d->segment ? d->segment->time : 0;
}

quint64 Segment::position() const
{
    return d->segment ? d->segment->position : 0;
}

quint64 Segment::duration() const
{
    return d->segment ? d->segment->duration : -1;
}

Segment::operator GstSegment*()
{
    return d->segment;
}

Segment::operator const GstSegment*() const
{
    return d->segment;
}

} //namespace QGst
