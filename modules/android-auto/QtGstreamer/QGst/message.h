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
#ifndef QGST_MESSAGE_H
#define QGST_MESSAGE_H

#include "clocktime.h"
#include "device.h"
#include "miniobject.h"
#include "structure.h"
#include "taglist.h"

namespace QGst {

/*! \headerfile message.h <QGst/Message>
 * \brief Wrapper class for GstMessage
 *
 * Messages are lightweight objects to signal the application of pipeline events. They
 * are posted by objects in the pipeline and are passed to the application using the Bus.
 *
 * Messages are implemented as a subclass of MiniObject with a generic GstStructure as the
 * content. This allows for writing custom messages without requiring an API change while
 * allowing a wide range of different types of messages.
 *
 * In these bindings, for convenience, each message type has its own Message subclass. This
 * does not reflect 1-1 the native C API, where there is only one Message class with tens of
 * 'new_foo' and 'parse_foo' methods. You can use RefPointer::dynamicCast() to cast a MessagePtr
 * to a RefPointer of one of the Message subclasses and it will behave as expected (i.e. it will
 * only succeed if the message type matches the message type that the subclass handles). Note
 * however that the Message subclasses \em cannot be used with Value::get(), since a GValue
 * will actually contain a GstMessage (the subclasses do not exist in C) and Value::get()
 * is not able to do dynamic casts. As a result of that, Message subclasses also \em cannot be
 * used as arguments in slots connected to GObject signals, even though you may know that your
 * slot will only be called with that type of message.
 */
class QTGSTREAMER_EXPORT Message : public MiniObject
{
    QGST_WRAPPER(Message)
public:
    ObjectPtr source() const;
    quint64 timestamp() const;
    QString typeName() const;
    MessageType type() const;

    StructureConstPtr internalStructure();

    quint32 sequenceNumber() const;
    void setSequenceNumber(quint32 num);
};

/*! \headerfile message.h <QGst/Message>
 * \brief Wrapper class for messages of type QGst::MessageEos
 */
class QTGSTREAMER_EXPORT EosMessage : public Message
{
    QGST_WRAPPER_FAKE_SUBCLASS(Eos, Message)
public:
    static EosMessagePtr create(const ObjectPtr & source);
};

/*! \headerfile message.h <QGst/Message>
 * \brief Wrapper class for messages of type QGst::MessageError
 */
class QTGSTREAMER_EXPORT ErrorMessage : public Message
{
    QGST_WRAPPER_FAKE_SUBCLASS(Error, Message)
public:
    static ErrorMessagePtr create(const ObjectPtr & source,
                                  const QGlib::Error & error, const char *debug);

    QGlib::Error error() const;
    QString debugMessage() const;
};

/*! \headerfile message.h <QGst/Message>
 * \brief Wrapper class for messages of type QGst::MessageWarning
 */
class QTGSTREAMER_EXPORT WarningMessage : public Message
{
    QGST_WRAPPER_FAKE_SUBCLASS(Warning, Message)
public:
    static WarningMessagePtr create(const ObjectPtr & source,
                                    const QGlib::Error & error, const char *debug);

    QGlib::Error error() const;
    QString debugMessage() const;
};

/*! \headerfile message.h <QGst/Message>
 * \brief Wrapper class for messages of type QGst::MessageInfo
 */
class QTGSTREAMER_EXPORT InfoMessage : public Message
{
    QGST_WRAPPER_FAKE_SUBCLASS(Info, Message)
public:
    static InfoMessagePtr create(const ObjectPtr & source,
                                 const QGlib::Error & error, const char *debug);

    QGlib::Error error() const;
    QString debugMessage() const;
};

/*! \headerfile message.h <QGst/Message>
 * \brief Wrapper class for messages of type QGst::MessageTag
 */
class QTGSTREAMER_EXPORT TagMessage : public Message
{
    QGST_WRAPPER_FAKE_SUBCLASS(Tag, Message)
public:
    static TagMessagePtr create(const ObjectPtr & source, const TagList & taglist);

    TagList taglist() const;
};

/*! \headerfile message.h <QGst/Message>
 * \brief Wrapper class for messages of type QGst::MessageBuffering
 */
class QTGSTREAMER_EXPORT BufferingMessage : public Message
{
    QGST_WRAPPER_FAKE_SUBCLASS(Buffering, Message)
public:
    static BufferingMessagePtr create(const ObjectPtr & source, int percent);

    int percent() const;
    BufferingMode mode() const;
    int averageInputRate() const;
    int averageOutputRate() const;
    qint64 bufferingTimeLeft() const;

    void setStats(BufferingMode mode, int avgIn, int avgOut, qint64 bufferingLeft);
};

/*! \headerfile message.h <QGst/Message>
 * \brief Wrapper class for messages of type QGst::MessageStateChanged
 */
class QTGSTREAMER_EXPORT StateChangedMessage : public Message
{
    QGST_WRAPPER_FAKE_SUBCLASS(StateChanged, Message)
public:
    static StateChangedMessagePtr create(const ObjectPtr & source,
                                         State oldState, State newState, State pending);

    State oldState() const;
    State newState() const;
    State pendingState() const;
};

//won't do: STATE_DIRTY (deprecated)

/*! \headerfile message.h <QGst/Message>
 * \brief Wrapper class for messages of type QGst::MessageStepDone
 */
class QTGSTREAMER_EXPORT StepDoneMessage : public Message
{
    QGST_WRAPPER_FAKE_SUBCLASS(StepDone, Message)
public:
    static StepDoneMessagePtr create(const ObjectPtr & source, Format format, quint64 amount,
                                     double rate, bool flush, bool intermediate, quint64 duration, bool eos);

    Format format() const;
    quint64 amount() const;
    double rate() const;
    bool isFlushingStep() const;
    bool isIntermediateStep() const;
    quint64 duration() const;
    bool causedEos() const;
};

//TODO CLOCK_PROVIDE, CLOCK_LOST, NEW_CLOCK
//maybe do: STRUCTURE_CHANGE (internal)

/*! \headerfile message.h <QGst/Message>
 * \brief Wrapper class for messages of type QGst::MessageStreamStatus
 */
class QTGSTREAMER_EXPORT StreamStatusMessage : public Message
{
    QGST_WRAPPER_FAKE_SUBCLASS(StreamStatus, Message)
public:
    static StreamStatusMessagePtr create(const ObjectPtr & source,
                                         StreamStatusType type, const ElementPtr & owner);

    StreamStatusType statusType() const;
    ElementPtr owner() const;
    QGlib::Value streamStatusObject() const;
    void setStreamStatusObject(const QGlib::Value & object);
};

/*! \headerfile message.h <QGst/Message>
 * \brief Wrapper class for messages of type QGst::MessageApplication
 */
class QTGSTREAMER_EXPORT ApplicationMessage : public Message
{
    QGST_WRAPPER_FAKE_SUBCLASS(Application, Message)
public:
    static ApplicationMessagePtr create(const ObjectPtr & source,
                                        const Structure & structure=Structure());
};

/*! \headerfile message.h <QGst/Message>
 * \brief Wrapper class for messages of type QGst::MessageElement
 */
class QTGSTREAMER_EXPORT ElementMessage : public Message
{
    QGST_WRAPPER_FAKE_SUBCLASS(Element, Message)
public:
    static ElementMessagePtr create(const ObjectPtr & source,
                                    const Structure & structure=Structure());
};

//maybe do: SEGMENT_START (internal)

/*! \headerfile message.h <QGst/Message>
 * \brief Wrapper class for messages of type QGst::MessageSegmentDone
 */
class QTGSTREAMER_EXPORT SegmentDoneMessage : public Message
{
    QGST_WRAPPER_FAKE_SUBCLASS(SegmentDone, Message)
public:
    static SegmentDoneMessagePtr create(const ObjectPtr & source, Format format, qint64 position);

    Format format() const;
    qint64 position() const;
};

/*! \headerfile message.h <QGst/Message>
 * \brief Wrapper class for messages of type QGst::MessageDuration
 */
class QTGSTREAMER_EXPORT DurationChangedMessage : public Message
{
    QGST_WRAPPER_FAKE_SUBCLASS(DurationChanged, Message)
public:
    static DurationChangedMessagePtr create(const ObjectPtr & source);

};

/*! \headerfile message.h <QGst/Message>
 * \brief Wrapper class for messages of type QGst::MessageLatency
 */
class QTGSTREAMER_EXPORT LatencyMessage : public Message
{
    QGST_WRAPPER_FAKE_SUBCLASS(Latency, Message)
public:
    static LatencyMessagePtr create(const ObjectPtr & source);
};

//maybe do: ASYNC_START (internal)

/*! \headerfile message.h <QGst/Message>
 * \brief Wrapper class for messages of type QGst::MessageAsyncDone
 */
class QTGSTREAMER_EXPORT AsyncDoneMessage : public Message
{
    QGST_WRAPPER_FAKE_SUBCLASS(AsyncDone, Message)
public:
    static AsyncDoneMessagePtr create(const ObjectPtr & source, ClockTime running_time);

    ClockTime runningTime() const;
};

/*! \headerfile message.h <QGst/Message>
 * \brief Wrapper class for messages of type QGst::MessageRequestState
 */
class QTGSTREAMER_EXPORT RequestStateMessage : public Message
{
    QGST_WRAPPER_FAKE_SUBCLASS(RequestState, Message)
public:
    static RequestStateMessagePtr create(const ObjectPtr & source, State state);

    State state() const;
};

/*! \headerfile message.h <QGst/Message>
 * \brief Wrapper class for messages of type QGst::MessageStepStart
 */
class QTGSTREAMER_EXPORT StepStartMessage : public Message
{
    QGST_WRAPPER_FAKE_SUBCLASS(StepStart, Message)
public:
    static StepStartMessagePtr create(const ObjectPtr & source, bool active, Format format,
                                      quint64 amount, double rate, bool flush, bool intermediate);
    bool isActive() const;
    Format format() const;
    quint64 amount() const;
    double rate() const;
    bool isFlushingStep() const;
    bool isIntermediateStep() const;
};

/*! \headerfile message.h <QGst/Message>
 * \brief Wrapper class for messages of type QGst::MessageQos
 */
class QTGSTREAMER_EXPORT QosMessage : public Message
{
    QGST_WRAPPER_FAKE_SUBCLASS(Qos, Message)
public:
    static QosMessagePtr create(const ObjectPtr & source, bool live, quint64 runningTime,
                                quint64 streamTime, quint64 timestamp, quint64 duration);

    bool live() const;
    quint64 runningTime() const;
    quint64 streamTime() const;
    quint64 timestamp() const;
    quint64 duration() const;

    qint64 jitter() const;
    double proportion() const;
    int quality() const;
    void setValues(qint64 jitter, double proportion, int quality);

    Format format() const;
    quint64 processed() const;
    quint64 dropped() const;
    void setStats(Format format, quint64 processed, quint64 dropped);
};

/*! \headerfile message.h <QGst/Message>
 * \brief Wrapper class for messages of type QGst::MessageDeviceAdded
 */
class QTGSTREAMER_EXPORT DeviceAddedMessage : public Message
{
    QGST_WRAPPER_FAKE_SUBCLASS(DeviceAdded, Message)
public:
    static DeviceAddedMessagePtr create(const ObjectPtr & source, const DevicePtr& device);

    DevicePtr device() const;
};

/*! \headerfile message.h <QGst/Message>
 * \brief Wrapper class for messages of type QGst::MessageDeviceRemoved
 */
class QTGSTREAMER_EXPORT DeviceRemovedMessage : public Message
{
    QGST_WRAPPER_FAKE_SUBCLASS(DeviceRemoved, Message)
public:
    static DeviceRemovedMessagePtr create(const ObjectPtr & source, const DevicePtr& device);

    DevicePtr device() const;
};

} //namespace QGst

QGST_REGISTER_TYPE(QGst::Message)
QGST_REGISTER_SUBCLASS(Message, Eos)
QGST_REGISTER_SUBCLASS(Message, Error)
QGST_REGISTER_SUBCLASS(Message, Warning)
QGST_REGISTER_SUBCLASS(Message, Info)
QGST_REGISTER_SUBCLASS(Message, Tag)
QGST_REGISTER_SUBCLASS(Message, Buffering)
QGST_REGISTER_SUBCLASS(Message, StateChanged)
QGST_REGISTER_SUBCLASS(Message, StepDone)
QGST_REGISTER_SUBCLASS(Message, StreamStatus)
QGST_REGISTER_SUBCLASS(Message, Application)
QGST_REGISTER_SUBCLASS(Message, Element)
QGST_REGISTER_SUBCLASS(Message, SegmentDone)
QGST_REGISTER_SUBCLASS(Message, DurationChanged)
QGST_REGISTER_SUBCLASS(Message, Latency)
QGST_REGISTER_SUBCLASS(Message, AsyncDone)
QGST_REGISTER_SUBCLASS(Message, RequestState)
QGST_REGISTER_SUBCLASS(Message, StepStart)
QGST_REGISTER_SUBCLASS(Message, Qos)
QGST_REGISTER_SUBCLASS(Message, DeviceAdded)
QGST_REGISTER_SUBCLASS(Message, DeviceRemoved)

#endif
