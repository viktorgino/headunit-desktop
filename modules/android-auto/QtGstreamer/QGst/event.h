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
#ifndef QGST_EVENT_H
#define QGST_EVENT_H

#include "miniobject.h"
#include "structure.h"
#include "clocktime.h"
#include "taglist.h"

namespace QGst {

/*! \headerfile event.h <QGst/Event>
 * \brief Wrapper class for GstEvent
 *
 * Events are passed between elements in parallel to the data stream. Some events are serialized
 * with buffers, others are not. Some events only travel downstream, others only upstream.
 * Some events can travel both upstream and downstream.
 *
 * The events are used to signal special conditions in the datastream such as EOS (end of stream) or
 * the start of a new stream-segment. Events are also used to flush the pipeline of pending data.
 *
 * Events are implemented as a subclass of MiniObject with a generic GstStructure as the
 * content. Notice that the source property is set by GStreamer when the event is passed to the
 * Pad with send() or push(). In the case of Element::sendEvent() the behavior is similar, as this
 * internally translates to searching for a random pad with the correct direction and then pushing
 * the event to it. So there is no need to set the source of the event in QtGStreamer.
 *
 * In these bindings, for convenience, each event type has its own Event subclass. This
 * does not reflect 1-1 the native C API, where there is only one Event class with tens of
 * 'new_foo' and 'parse_foo' methods. You can use RefPointer::dynamicCast() to cast a EventPtr
 * to a RefPointer of one of the Event subclasses and it will behave as expected (i.e. it will
 * only succeed if the event type matches the event type that the subclass handles). Note
 * however that the Event subclasses \em cannot be used with Value::get(), since a GValue
 * will actually contain a GstEvent (the subclasses do not exist in C) and Value::get()
 * is not able to do dynamic casts. As a result of that, Event subclasses also \em cannot be
 * used as arguments in slots connected to GObject signals, even though you may know that your
 * slot will only be called with that type of event.
 */
class QTGSTREAMER_EXPORT Event : public MiniObject
{
    QGST_WRAPPER(Event)
public:
    quint64 timestamp() const;
    EventType type() const;
    QString typeName() const;

    StructureConstPtr internalStructure();

    bool hasName(const char *name) const;

    quint32 sequenceNumber() const;
    void setSequenceNumber(quint32 num);

    EventPtr copy() const;

};

/*! \headerfile event.h <QGst/Event>
 * \brief Wrapper class for events of type QGst::FlushStartEvent
 */
class QTGSTREAMER_EXPORT FlushStartEvent : public Event
{
    QGST_WRAPPER_FAKE_SUBCLASS(FlushStart, Event)
public:
    static FlushStartEventPtr create();
};

/*! \headerfile event.h <QGst/Event>
 * \brief Wrapper class for events of type QGst::FlushStopEvent
 */
class QTGSTREAMER_EXPORT FlushStopEvent : public Event
{
    QGST_WRAPPER_FAKE_SUBCLASS(FlushStop, Event)
public:
    static FlushStopEventPtr create(bool reset_time=true);

    bool resetTime() const;
};

/*! \headerfile event.h <QGst/Event>
 * \brief Wrapper class for events of type QGst::EosEvent
 */
class QTGSTREAMER_EXPORT EosEvent : public Event
{
    QGST_WRAPPER_FAKE_SUBCLASS(Eos, Event)
public:
    static EosEventPtr create();
};

/*! \headerfile event.h <QGst/Event>
 * \brief Wrapper class for events of type QGst::EventCaps
 */
 class QTGSTREAMER_EXPORT CapsEvent : public Event
 {
     QGST_WRAPPER_FAKE_SUBCLASS(Caps, Event)
public:
     static CapsEventPtr create(const CapsPtr & caps);

     CapsPtr caps() const;
 };

/*! \headerfile event.h <QGst/Event>
 * \brief Wrapper class for events of type QGst::SegmentEvent
 */
class QTGSTREAMER_EXPORT SegmentEvent : public Event
{
    QGST_WRAPPER_FAKE_SUBCLASS(Segment, Event)
public:
    static SegmentEventPtr create(const Segment & segment);

    Segment segment() const;
};

/*! \headerfile event.h <QGst/Event>
 * \brief Wrapper class for events of type QGst::TagEvent
 */
class QTGSTREAMER_EXPORT TagEvent : public Event
{
    QGST_WRAPPER_FAKE_SUBCLASS(Tag, Event)
public:
    static TagEventPtr create(const TagList & taglist);

    TagList taglist() const;
};

/*! \headerfile event.h <QGst/Event>
 * \brief Wrapper class for events of type QGst::BufferSizeEvent
 */
class QTGSTREAMER_EXPORT BufferSizeEvent : public Event
{
    QGST_WRAPPER_FAKE_SUBCLASS(BufferSize, Event)
public:
    static BufferSizeEventPtr create(Format format, qint64 minSize, qint64 maxSize, bool isAsync);

    Format format() const;
    qint64 minSize() const;
    qint64 maxSize() const;
    bool isAsync() const;
};

/*! \headerfile event.h <QGst/Event>
 * \brief Wrapper class for events of type QGst::SinkMessageEvent
 */
class QTGSTREAMER_EXPORT SinkMessageEvent : public Event
{
    QGST_WRAPPER_FAKE_SUBCLASS(SinkMessage, Event)
public:
    static SinkMessageEventPtr create(const QString &name, const MessagePtr & msg);

    MessagePtr message() const;
};

/*! \headerfile event.h <QGst/Event>
 * \brief Wrapper class for events of type QGst::QosEvent
 */
class QTGSTREAMER_EXPORT QosEvent : public Event
{
    QGST_WRAPPER_FAKE_SUBCLASS(Qos, Event)
public:
    static QosEventPtr create(QosType qos, double proportion, ClockTimeDiff diff, ClockTime timestamp);

    QosType qosType() const;
    double proportion() const;
    ClockTimeDiff diff() const;
    ClockTime timestamp() const;
};

/*! \headerfile event.h <QGst/Event>
 * \brief Wrapper class for events of type QGst::SeekEvent
 */
class QTGSTREAMER_EXPORT SeekEvent : public Event
{
    QGST_WRAPPER_FAKE_SUBCLASS(Seek, Event)
public:
    static SeekEventPtr create(double rate, Format format, SeekFlags flags, SeekType startType,
                               qint64 start, SeekType stopType, qint64 stop);

    double rate() const;
    Format format() const;
    SeekFlags flags() const;
    SeekType startType() const;
    qint64 start() const;
    SeekType stopType() const;
    qint64 stop() const;
};

/*! \headerfile event.h <QGst/Event>
 * \brief Wrapper class for events of type QGst::NavigationEvent
 */
class QTGSTREAMER_EXPORT NavigationEvent : public Event
{
    QGST_WRAPPER_FAKE_SUBCLASS(Navigation, Event)
public:
    static NavigationEventPtr create(const Structure & structure  = Structure());
};

/*! \headerfile event.h <QGst/Event>
 * \brief Wrapper class for events of type QGst::LatencyEvent
 */
class QTGSTREAMER_EXPORT LatencyEvent : public Event
{
    QGST_WRAPPER_FAKE_SUBCLASS(Latency, Event)
public:
    static LatencyEventPtr create(ClockTime latency);

    ClockTime latency() const;
};

/*! \headerfile event.h <QGst/Event>
 * \brief Wrapper class for events of type QGst::StepEvent
 */
class QTGSTREAMER_EXPORT StepEvent : public Event
{
    QGST_WRAPPER_FAKE_SUBCLASS(Step, Event)
public:
    static StepEventPtr create(Format format, quint64 amount, double rate,
                            bool flush, bool intermediate);

    Format format() const;
    quint64 amount() const;
    double rate() const;
    bool flush() const;
    bool intermediate() const;
};

} //namespace QGst

QGST_REGISTER_TYPE(QGst::Event)
QGST_REGISTER_SUBCLASS(Event, FlushStart)
QGST_REGISTER_SUBCLASS(Event, FlushStop)
QGST_REGISTER_SUBCLASS(Event, Eos)
QGST_REGISTER_SUBCLASS(Event, Caps)
QGST_REGISTER_SUBCLASS(Event, Segment)
QGST_REGISTER_SUBCLASS(Event, Tag)
QGST_REGISTER_SUBCLASS(Event, BufferSize)
QGST_REGISTER_SUBCLASS(Event, SinkMessage)
QGST_REGISTER_SUBCLASS(Event, Qos)
QGST_REGISTER_SUBCLASS(Event, Seek)
QGST_REGISTER_SUBCLASS(Event, Navigation)
QGST_REGISTER_SUBCLASS(Event, Latency)
QGST_REGISTER_SUBCLASS(Event, Step)

#endif
