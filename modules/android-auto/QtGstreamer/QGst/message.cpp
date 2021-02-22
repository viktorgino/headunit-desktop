/*
    Copyright (C) 2009-2010  George Kiagiadakis <kiagiadakis.george@gmail.com>

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
#include "message.h"
#include "element.h"
#include "../QGlib/error.h"
#include "../QGlib/string_p.h"
#include <QtCore/QDebug>
#include <gst/gst.h>

namespace QGst {

ObjectPtr Message::source() const
{
    return ObjectPtr::wrap(GST_MESSAGE_SRC(object<GstMessage>()));
}

quint64 Message::timestamp() const
{
    return object<GstMessage>()->timestamp;
}

QString Message::typeName() const
{
    return QString::fromUtf8(GST_MESSAGE_TYPE_NAME(object<GstMessage>()));
}

MessageType Message::type() const
{
    return static_cast<MessageType>(GST_MESSAGE_TYPE(object<GstMessage>()));
}

StructureConstPtr Message::internalStructure()
{
    const GstStructure *structure = gst_message_get_structure(object<GstMessage>());
    return SharedStructure::fromMiniObject(const_cast<GstStructure *>(structure), MiniObjectPtr(this));
}

quint32 Message::sequenceNumber() const
{
    return gst_message_get_seqnum(object<GstMessage>());
}

void Message::setSequenceNumber(quint32 num)
{
    gst_message_set_seqnum(object<GstMessage>(), num);
}

//********************************************************

EosMessagePtr EosMessage::create(const ObjectPtr & source)
{
    return EosMessagePtr::wrap(gst_message_new_eos(source), false);
}

//********************************************************

ErrorMessagePtr ErrorMessage::create(const ObjectPtr & source,
                                     const QGlib::Error & error, const char *debug)
{
    //stupid GstMessage api takes non-const GError while it should
    GError *e = const_cast<GError*>(static_cast<const GError*>(error));
    return ErrorMessagePtr::wrap(gst_message_new_error(source, e, debug), false);
}

QGlib::Error ErrorMessage::error() const
{
    GError *e;
    gst_message_parse_error(object<GstMessage>(), &e, NULL);
    return QGlib::Error(e);
}

QString ErrorMessage::debugMessage() const
{
    gchar *debug;
    GError *e;
    //Passing a NULL pointer for the GError is not supported
    gst_message_parse_error(object<GstMessage>(), &e, &debug);
    if (e) {
        g_error_free (e);
    }
    return QGlib::Private::stringFromGCharPtr(debug);
}

//********************************************************

WarningMessagePtr WarningMessage::create(const ObjectPtr & source,
                                         const QGlib::Error & error, const char *debug)
{
    //stupid GstMessage api takes non-const GError while it should
    GError *e = const_cast<GError*>(static_cast<const GError*>(error));
    return WarningMessagePtr::wrap(gst_message_new_warning(source, e, debug), false);
}

QGlib::Error WarningMessage::error() const
{
    GError *e;
    gst_message_parse_warning(object<GstMessage>(), &e, NULL);
    return QGlib::Error(e);
}

QString WarningMessage::debugMessage() const
{
    gchar *debug;
    GError *e;
    //Passing a NULL pointer for the GError is not supported
    gst_message_parse_warning(object<GstMessage>(), &e, &debug);
    if (e) {
        g_error_free (e);
    }
    return QGlib::Private::stringFromGCharPtr(debug);
}

//********************************************************

InfoMessagePtr InfoMessage::create(const ObjectPtr & source,
                                   const QGlib::Error & error, const char *debug)
{
    //stupid GstMessage api takes non-const GError while it should
    GError *e = const_cast<GError*>(static_cast<const GError*>(error));
    return InfoMessagePtr::wrap(gst_message_new_info(source, e, debug), false);
}

QGlib::Error InfoMessage::error() const
{
    GError *e;
    gst_message_parse_info(object<GstMessage>(), &e, NULL);
    return QGlib::Error(e);
}

QString InfoMessage::debugMessage() const
{
    gchar *debug;
    GError *e;
    //Passing a NULL pointer for the GError is not supported
    gst_message_parse_info(object<GstMessage>(), &e, &debug);
    if (e) {
        g_error_free (e);
    }
    return QGlib::Private::stringFromGCharPtr(debug);
}

//********************************************************

TagMessagePtr TagMessage::create(const ObjectPtr & source, const TagList & taglist)
{
    GstMessage *m = gst_message_new_tag(source, gst_tag_list_copy(taglist));
    return TagMessagePtr::wrap(m, false);
}

TagList TagMessage::taglist() const
{
    GstTagList * t;
    gst_message_parse_tag(object<GstMessage>(), &t);
    TagList tl(t);
    gst_tag_list_free(t);
    return tl;
}

//********************************************************

BufferingMessagePtr BufferingMessage::create(const ObjectPtr & source, int percent)
{
    GstMessage *m = gst_message_new_buffering(source, percent);
    return BufferingMessagePtr::wrap(m, false);
}

int BufferingMessage::percent() const
{
    gint p;
    gst_message_parse_buffering(object<GstMessage>(), &p);
    return p;
}

BufferingMode BufferingMessage::mode() const
{
    GstBufferingMode m;
    gst_message_parse_buffering_stats(object<GstMessage>(), &m, NULL, NULL, NULL);
    return static_cast<BufferingMode>(m);
}

int BufferingMessage::averageInputRate() const
{
    gint a;
    gst_message_parse_buffering_stats(object<GstMessage>(), NULL, &a, NULL, NULL);
    return a;
}

int BufferingMessage::averageOutputRate() const
{
    gint a;
    gst_message_parse_buffering_stats(object<GstMessage>(), NULL, NULL, &a, NULL);
    return a;
}

qint64 BufferingMessage::bufferingTimeLeft() const
{
    gint64 a;
    gst_message_parse_buffering_stats(object<GstMessage>(), NULL, NULL, NULL, &a);
    return a;
}

void BufferingMessage::setStats(BufferingMode mode, int avgIn, int avgOut, qint64 bufferingLeft)
{
    gst_message_set_buffering_stats(object<GstMessage>(), static_cast<GstBufferingMode>(mode),
                                    avgIn, avgOut, bufferingLeft);
}

//********************************************************

StateChangedMessagePtr StateChangedMessage::create(const ObjectPtr & source,
                                                   State oldState, State newState, State pending)
{
    GstMessage *m = gst_message_new_state_changed(source, static_cast<GstState>(oldState),
                                                  static_cast<GstState>(newState),
                                                  static_cast<GstState>(pending));
    return StateChangedMessagePtr::wrap(m, false);
}

State StateChangedMessage::oldState() const
{
    GstState s;
    gst_message_parse_state_changed(object<GstMessage>(), &s, NULL, NULL);
    return static_cast<State>(s);
}

State StateChangedMessage::newState() const
{
    GstState s;
    gst_message_parse_state_changed(object<GstMessage>(), NULL, &s, NULL);
    return static_cast<State>(s);
}

State StateChangedMessage::pendingState() const
{
    GstState s;
    gst_message_parse_state_changed(object<GstMessage>(), NULL, NULL, &s);
    return static_cast<State>(s);
}

//********************************************************

StepDoneMessagePtr StepDoneMessage::create(const ObjectPtr & source, Format format,
                                           quint64 amount, double rate, bool flush,
                                           bool intermediate, quint64 duration, bool eos)
{
    GstMessage *m = gst_message_new_step_done(source, static_cast<GstFormat>(format), amount,
                                              rate, flush, intermediate, duration, eos);
    return StepDoneMessagePtr::wrap(m, false);
}

Format StepDoneMessage::format() const
{
    GstFormat f;
    gst_message_parse_step_done(object<GstMessage>(), &f, NULL, NULL, NULL, NULL, NULL, NULL);
    return static_cast<Format>(f);
}

quint64 StepDoneMessage::amount() const
{
    guint64 a;
    gst_message_parse_step_done(object<GstMessage>(), NULL, &a, NULL, NULL, NULL, NULL, NULL);
    return a;
}

double StepDoneMessage::rate() const
{
    gdouble d;
    gst_message_parse_step_done(object<GstMessage>(), NULL, NULL, &d, NULL, NULL, NULL, NULL);
    return d;
}

bool StepDoneMessage::isFlushingStep() const
{
    gboolean b;
    gst_message_parse_step_done(object<GstMessage>(), NULL, NULL, NULL, &b, NULL, NULL, NULL);
    return b;
}

bool StepDoneMessage::isIntermediateStep() const
{
    gboolean b;
    gst_message_parse_step_done(object<GstMessage>(), NULL, NULL, NULL, NULL, &b, NULL, NULL);
    return b;
}

quint64 StepDoneMessage::duration() const
{
    guint64 d;
    gst_message_parse_step_done(object<GstMessage>(), NULL, NULL, NULL, NULL, NULL, &d, NULL);
    return d;
}

bool StepDoneMessage::causedEos() const
{
    gboolean e;
    gst_message_parse_step_done(object<GstMessage>(), NULL, NULL, NULL, NULL, NULL, NULL, &e);
    return e;
}

//********************************************************

StreamStatusMessagePtr StreamStatusMessage::create(const ObjectPtr & source,
                                                   StreamStatusType type, const ElementPtr & owner)
{
    GstMessage *m = gst_message_new_stream_status(source, static_cast<GstStreamStatusType>(type), owner);
    return StreamStatusMessagePtr::wrap(m, false);
}

StreamStatusType StreamStatusMessage::statusType() const
{
    GstStreamStatusType t;
    gst_message_parse_stream_status(object<GstMessage>(), &t, NULL);
    return static_cast<StreamStatusType>(t);
}

ElementPtr StreamStatusMessage::owner() const
{
    GstElement *e;
    gst_message_parse_stream_status(object<GstMessage>(), NULL, &e);
    return ElementPtr::wrap(e);
}

QGlib::Value StreamStatusMessage::streamStatusObject() const
{
    return QGlib::Value(gst_message_get_stream_status_object(object<GstMessage>()));
}

void StreamStatusMessage::setStreamStatusObject(const QGlib::Value & obj)
{
    gst_message_set_stream_status_object(object<GstMessage>(), obj);
}

//********************************************************

ApplicationMessagePtr ApplicationMessage::create(const ObjectPtr & source, const Structure & structure)
{
    GstStructure *s = structure.isValid() ? gst_structure_copy(structure) : gst_structure_new_empty("null");
    return ApplicationMessagePtr::wrap(gst_message_new_application(source, s), false);
}

//********************************************************

ElementMessagePtr ElementMessage::create(const ObjectPtr & source, const Structure & structure)
{
    GstStructure *s = structure.isValid() ? gst_structure_copy(structure) : gst_structure_new_empty("null");
    return ElementMessagePtr::wrap(gst_message_new_element(source, s), false);
}

//********************************************************

SegmentDoneMessagePtr SegmentDoneMessage::create(const ObjectPtr & source, Format format, qint64 position)
{
    GstMessage *m = gst_message_new_segment_done(source, static_cast<GstFormat>(format), position);
    return SegmentDoneMessagePtr::wrap(m, false);
}

Format SegmentDoneMessage::format() const
{
    GstFormat f;
    gst_message_parse_segment_done(object<GstMessage>(), &f, NULL);
    return static_cast<Format>(f);
}

qint64 SegmentDoneMessage::position() const
{
    gint64 p;
    gst_message_parse_segment_done(object<GstMessage>(), NULL, &p);
    return p;
}

//********************************************************

DurationChangedMessagePtr DurationChangedMessage::create(const ObjectPtr & source)
{
    GstMessage *m = gst_message_new_duration_changed(source);
    return DurationChangedMessagePtr::wrap(m, false);
}

//********************************************************

LatencyMessagePtr LatencyMessage::create(const ObjectPtr & source)
{
    return LatencyMessagePtr::wrap(gst_message_new_latency(source), false);
}

//********************************************************

AsyncDoneMessagePtr AsyncDoneMessage::create(const ObjectPtr & source, ClockTime running_time)
{
    return AsyncDoneMessagePtr::wrap(gst_message_new_async_done(source, running_time), false);
}

ClockTime AsyncDoneMessage::runningTime() const
{
    GstClockTime c;
    gst_message_parse_async_done(object<GstMessage>(), &c);
    return static_cast<ClockTime>(c);
}

//********************************************************

RequestStateMessagePtr RequestStateMessage::create(const ObjectPtr & source, State state)
{
    GstMessage *m = gst_message_new_request_state(source, static_cast<GstState>(state));
    return RequestStateMessagePtr::wrap(m, false);
}

State RequestStateMessage::state() const
{
    GstState s;
    gst_message_parse_request_state(object<GstMessage>(), &s);
    return static_cast<State>(s);
}

//********************************************************

StepStartMessagePtr StepStartMessage::create(const ObjectPtr & source, bool active, Format format,
                                             quint64 amount, double rate, bool flush, bool intermediate)
{
    GstMessage *m = gst_message_new_step_start(source, active, static_cast<GstFormat>(format),
                                               amount, rate, flush, intermediate);
    return StepStartMessagePtr::wrap(m, false);
}

bool StepStartMessage::isActive() const
{
    gboolean a;
    gst_message_parse_step_start(object<GstMessage>(), &a, NULL, NULL, NULL, NULL, NULL);
    return a;
}

Format StepStartMessage::format() const
{
    GstFormat f;
    gst_message_parse_step_start(object<GstMessage>(), NULL, &f, NULL, NULL, NULL, NULL);
    return static_cast<Format>(f);
}

quint64 StepStartMessage::amount() const
{
    guint64 a;
    gst_message_parse_step_start(object<GstMessage>(), NULL, NULL, &a, NULL, NULL, NULL);
    return a;
}

double StepStartMessage::rate() const
{
    gdouble d;
    gst_message_parse_step_start(object<GstMessage>(), NULL, NULL, NULL, &d, NULL, NULL);
    return d;
}

bool StepStartMessage::isFlushingStep() const
{
    gboolean b;
    gst_message_parse_step_start(object<GstMessage>(), NULL, NULL, NULL, NULL, &b, NULL);
    return b;
}

bool StepStartMessage::isIntermediateStep() const
{
    gboolean b;
    gst_message_parse_step_start(object<GstMessage>(), NULL, NULL, NULL, NULL, NULL, &b);
    return b;
}

//********************************************************

QosMessagePtr QosMessage::create(const ObjectPtr & source, bool live, quint64 runningTime,
                                 quint64 streamTime, quint64 timestamp, quint64 duration)
{
    GstMessage *m = gst_message_new_qos(source, live, runningTime, streamTime, timestamp, duration);
    return QosMessagePtr::wrap(m, false);
}

bool QosMessage::live() const
{
    gboolean l;
    gst_message_parse_qos(object<GstMessage>(), &l, NULL, NULL, NULL, NULL);
    return l;
}

quint64 QosMessage::runningTime() const
{
    guint64 t;
    gst_message_parse_qos(object<GstMessage>(), NULL, &t, NULL, NULL, NULL);
    return t;
}

quint64 QosMessage::streamTime() const
{
    guint64 t;
    gst_message_parse_qos(object<GstMessage>(), NULL, NULL, &t, NULL, NULL);
    return t;
}

quint64 QosMessage::timestamp() const
{
    guint64 t;
    gst_message_parse_qos(object<GstMessage>(), NULL, NULL, NULL, &t, NULL);
    return t;
}

quint64 QosMessage::duration() const
{
    guint64 t;
    gst_message_parse_qos(object<GstMessage>(), NULL, NULL, NULL, NULL, &t);
    return t;
}

qint64 QosMessage::jitter() const
{
    gint64 j;
    gst_message_parse_qos_values(object<GstMessage>(), &j, NULL, NULL);
    return j;
}

double QosMessage::proportion() const
{
    double d;
    gst_message_parse_qos_values(object<GstMessage>(), NULL, &d, NULL);
    return d;
}

int QosMessage::quality() const
{
    gint q;
    gst_message_parse_qos_values(object<GstMessage>(), NULL, NULL, &q);
    return q;
}

void QosMessage::setValues(qint64 jitter, double proportion, int quality)
{
    gst_message_set_qos_values(object<GstMessage>(), jitter, proportion, quality);
}

Format QosMessage::format() const
{
    GstFormat f;
    gst_message_parse_qos_stats(object<GstMessage>(), &f, NULL, NULL);
    return static_cast<Format>(f);
}

quint64 QosMessage::processed() const
{
    guint64 p;
    gst_message_parse_qos_stats(object<GstMessage>(), NULL, &p, NULL);
    return p;
}

quint64 QosMessage::dropped() const
{
    guint64 p;
    gst_message_parse_qos_stats(object<GstMessage>(), NULL, NULL, &p);
    return p;
}

void QosMessage::setStats(Format format, quint64 processed, quint64 dropped)
{
    gst_message_set_qos_stats(object<GstMessage>(), static_cast<GstFormat>(format), processed,
                              dropped);
}

//********************************************************

DeviceAddedMessagePtr DeviceAddedMessage::create(const ObjectPtr & source, const DevicePtr & device)
{
    GstMessage *m = gst_message_new_device_added(source, device);
    return DeviceAddedMessagePtr::wrap(m, false);
}

DevicePtr DeviceAddedMessage::device() const
{
    GstDevice *d;
    gst_message_parse_device_added(object<GstMessage>(), &d);
    return DevicePtr::wrap(d, false);
}

//********************************************************

DeviceRemovedMessagePtr DeviceRemovedMessage::create(const ObjectPtr & source, const DevicePtr & device)
{
    GstMessage *m = gst_message_new_device_removed(source, device);
    return DeviceRemovedMessagePtr::wrap(m, false);
}

DevicePtr DeviceRemovedMessage::device() const
{
    GstDevice *d;
    gst_message_parse_device_removed(object<GstMessage>(), &d);
    return DevicePtr::wrap(d, false);
}

} //namespace QGst
