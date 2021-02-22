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
#include "caps.h"
#include "event.h"
#include "message.h"
#include "object.h"
#include "segment.h"
#include <QtCore/QDebug>
#include <gst/gst.h>

namespace QGst {

quint64 Event::timestamp() const
{
    return object<GstEvent>()->timestamp;
}

EventType Event::type() const
{
    return static_cast<EventType>(GST_EVENT_TYPE(object<GstEvent>()));
}

QString Event::typeName() const
{
    return QString::fromUtf8(GST_EVENT_TYPE_NAME(object<GstEvent>()));
}

StructureConstPtr Event::internalStructure()
{
    const GstStructure *structure = gst_event_get_structure(object<GstEvent>());
    return SharedStructure::fromMiniObject(const_cast<GstStructure *>(structure), MiniObjectPtr(this));
}

bool Event::hasName(const char *name) const
{
    return gst_event_has_name(object<GstEvent>(), name);
}

quint32 Event::sequenceNumber() const
{
    return gst_event_get_seqnum(object<GstEvent>());
}

void Event::setSequenceNumber(quint32 num)
{
    gst_event_set_seqnum(object<GstEvent>(), num);
}

EventPtr Event::copy() const
{
    return EventPtr::wrap(gst_event_copy(object<GstEvent>()), false);
}

//********************************************************

FlushStartEventPtr FlushStartEvent::create()
{
    return FlushStartEventPtr::wrap(gst_event_new_flush_start(), false);
}

//********************************************************

FlushStopEventPtr FlushStopEvent::create(bool reset_time)
{
    return FlushStopEventPtr::wrap(gst_event_new_flush_stop(reset_time), false);
}

bool FlushStopEvent::resetTime() const
{
    gboolean r;
    gst_event_parse_flush_stop(object<GstEvent>(), &r);
    return r;
}

//********************************************************

EosEventPtr EosEvent::create()
{
    return EosEventPtr::wrap(gst_event_new_eos(), false);
}

//********************************************************
CapsEventPtr CapsEvent::create(const CapsPtr &caps)
{
    return CapsEventPtr::wrap(gst_event_new_caps(caps), false);
}

CapsPtr CapsEvent::caps() const
{
    GstCaps *c;
    gst_event_parse_caps (object<GstEvent>(), &c);
    return CapsPtr::wrap (c);
}

//********************************************************

SegmentEventPtr SegmentEvent::create(const Segment & segment)
{
    return SegmentEventPtr::wrap(gst_event_new_segment(segment), false);
}

Segment SegmentEvent::segment() const
{
    const GstSegment *s;
    gst_event_parse_segment(object<GstEvent>(), &s);

    return Segment(s);
}

//********************************************************
TagEventPtr TagEvent::create(const TagList & taglist)
{
    GstEvent * e = gst_event_new_tag(gst_tag_list_copy(taglist));
    return TagEventPtr::wrap(e, false);
}

TagList TagEvent::taglist() const
{
    GstTagList * t;
    gst_event_parse_tag(object<GstEvent>(), &t);
    TagList tl(t);
    return tl;
}

//********************************************************

BufferSizeEventPtr BufferSizeEvent::create(Format format, qint64 minSize, qint64 maxSize,
                                           bool isAsync)
{
    GstEvent * e = gst_event_new_buffer_size(static_cast<GstFormat>(format), minSize, maxSize,
                                                  isAsync);

    return BufferSizeEventPtr::wrap(e, false);
}

Format BufferSizeEvent::format() const
{
    GstFormat f;
    gst_event_parse_buffer_size(object<GstEvent>(), &f, NULL, NULL, NULL);
    return static_cast<Format>(f);
}

qint64 BufferSizeEvent::minSize() const
{
    gint64 s;
    gst_event_parse_buffer_size(object<GstEvent>(), NULL, &s, NULL, NULL);
    return s;
}

qint64 BufferSizeEvent::maxSize() const
{
    gint64 s;
    gst_event_parse_buffer_size(object<GstEvent>(), NULL, NULL, &s, NULL);
    return s;
}

bool BufferSizeEvent::isAsync() const
{
    gboolean u;
    gst_event_parse_buffer_size(object<GstEvent>(), NULL, NULL, NULL, &u);
    return u;
}

//********************************************************

SinkMessageEventPtr SinkMessageEvent::create(const QString &name, const MessagePtr & msg)
{
    GstEvent * e = gst_event_new_sink_message(name.toUtf8().constData(), msg);
    return SinkMessageEventPtr::wrap(e, false);
}

MessagePtr SinkMessageEvent::message() const
{
    GstMessage * msg;
    gst_event_parse_sink_message(object<GstEvent>(), &msg);
    //Wrap message (refcount was already increased), will unref() when MessagePtr is destroyed
    return MessagePtr::wrap(msg, false);
}

//********************************************************

QosEventPtr QosEvent::create(QosType qos, double proportion, ClockTimeDiff diff, ClockTime timeStamp)
{
    GstEvent * e = gst_event_new_qos(static_cast<GstQOSType>(qos), proportion, diff, static_cast<GstClockTime>(timeStamp));
    return QosEventPtr::wrap(e, false);
}

QosType QosEvent::qosType() const
{
    GstQOSType t;
    gst_event_parse_qos(object<GstEvent>(), &t, NULL, NULL, NULL);
    return static_cast<QosType>(t);
}

double QosEvent::proportion() const
{
    double d;
    gst_event_parse_qos(object<GstEvent>(), NULL, &d, NULL, NULL);
    return d;
}

ClockTimeDiff QosEvent::diff() const
{
    GstClockTimeDiff c;
    gst_event_parse_qos(object<GstEvent>(), NULL, NULL, &c, NULL);
    return c;
}

ClockTime QosEvent::timestamp() const
{
    GstClockTime c;
    gst_event_parse_qos(object<GstEvent>(), NULL, NULL, NULL, &c);
    return c;
}

//********************************************************

SeekEventPtr SeekEvent::create(double rate, Format format, SeekFlags flags, SeekType startType,
                               qint64 start, SeekType stopType, qint64 stop)
{
    GstEvent * e = gst_event_new_seek(rate, static_cast<GstFormat>(format),
                                      static_cast<GstSeekFlags>(static_cast<int>(flags)),
                                      static_cast<GstSeekType>(startType), start,
                                      static_cast<GstSeekType>(stopType), stop );
    return SeekEventPtr::wrap(e, false);
}

double SeekEvent::rate() const
{
    double d;
    gst_event_parse_seek(object<GstEvent>(), &d, NULL, NULL, NULL, NULL, NULL, NULL);
    return d;
}

Format SeekEvent::format() const
{
    GstFormat f;
    gst_event_parse_seek(object<GstEvent>(), NULL, &f, NULL, NULL, NULL, NULL, NULL);
    return static_cast<Format>(f);
}

SeekFlags SeekEvent::flags() const
{
    GstSeekFlags f;
    gst_event_parse_seek(object<GstEvent>(), NULL, NULL, &f, NULL, NULL, NULL, NULL);
    return static_cast<SeekFlags>(f);
}

SeekType SeekEvent::startType() const
{
    GstSeekType t;
    gst_event_parse_seek(object<GstEvent>(), NULL, NULL, NULL, &t, NULL, NULL, NULL);
    return static_cast<SeekType>(t);
}

qint64 SeekEvent::start() const
{
    gint64 s;
    gst_event_parse_seek(object<GstEvent>(), NULL, NULL, NULL, NULL, &s, NULL, NULL);
    return s;
}

SeekType SeekEvent::stopType() const
{
    GstSeekType t;
    gst_event_parse_seek(object<GstEvent>(), NULL, NULL, NULL, NULL, NULL, &t, NULL);
    return static_cast<SeekType>(t);
}

qint64 SeekEvent::stop() const
{
    gint64 s;
    gst_event_parse_seek(object<GstEvent>(), NULL, NULL, NULL, NULL, NULL, NULL, &s);
    return s;
}

//********************************************************

NavigationEventPtr NavigationEvent::create(const Structure & structure)
{
    GstStructure * s = structure.isValid() ? gst_structure_copy(structure) : NULL;
    GstEvent * e = gst_event_new_navigation(s);
    return NavigationEventPtr::wrap(e, false);
}

//********************************************************

LatencyEventPtr LatencyEvent::create(ClockTime latency)
{
    GstEvent * e = gst_event_new_latency(latency);
    return LatencyEventPtr::wrap(e, false);
}

ClockTime LatencyEvent::latency() const
{
    GstClockTime c;
    gst_event_parse_latency(object<GstEvent>(), &c);
    return c;
}

//********************************************************

StepEventPtr StepEvent::create(Format format, quint64 amount, double rate, bool flush,
                               bool intermediate)
{
    GstEvent * e = gst_event_new_step(static_cast<GstFormat>(format), amount, rate, flush,
                                      intermediate);
    return StepEventPtr::wrap(e, false);
}

Format StepEvent::format() const
{
    GstFormat f;
    gst_event_parse_step(object<GstEvent>(), &f, NULL, NULL, NULL, NULL);
    return static_cast<Format>(f);
}

quint64 StepEvent::amount() const
{
    guint64 a;
    gst_event_parse_step(object<GstEvent>(), NULL, &a, NULL, NULL, NULL);
    return a;
}

double StepEvent::rate() const
{
    double d;
    gst_event_parse_step(object<GstEvent>(), NULL, NULL, &d, NULL, NULL);
    return d;

}

bool StepEvent::flush() const
{
    gboolean f;
    gst_event_parse_step(object<GstEvent>(), NULL, NULL, NULL, &f, NULL);
    return f;
}

bool StepEvent::intermediate() const
{
    gboolean i;
    gst_event_parse_step(object<GstEvent>(), NULL, NULL, NULL, NULL, &i);
    return i;
}

} //namespace QGst
