/*
    Copyright (C) 2010  George Kiagiadakis <kiagiadakis.george@gmail.com>
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
#ifndef QGST_ENUMS_H
#define QGST_ENUMS_H

#include "global.h"
#include <QtCore/QFlags>

namespace QGst {
    enum MiniObjectFlag {
        MiniObjectFlagLockable = (1<<0),
        MiniObjectFlagLockReadonly = (1<<1),
        /*padding*/
        MiniObjectFlagLast = (1<<4)
    };
    Q_DECLARE_FLAGS(MiniObjectFlags, MiniObjectFlag);
    Q_DECLARE_OPERATORS_FOR_FLAGS(MiniObjectFlags);
}
QGST_REGISTER_TYPE(QGst::MiniObjectFlags);


namespace QGst {
    enum ObjectFlag {
        /*padding*/
        ObjectFlagLast = (1<<4)
    };
    Q_DECLARE_FLAGS(ObjectFlags, ObjectFlag);
    Q_DECLARE_OPERATORS_FOR_FLAGS(ObjectFlags);
}
QGST_REGISTER_TYPE(QGst::ObjectFlags);


namespace QGst {
    enum State {
        StateVoidPending = 0,
        StateNull = 1,
        StateReady = 2,
        StatePaused = 3,
        StatePlaying = 4
    };
}
QGST_REGISTER_TYPE(QGst::State)

namespace QGst {
    enum StateChange {
        StateChangeNullToReady = (StateNull<<3) | StateReady,
        StateChangeReadyToPaused = (StateReady<<3) | StatePaused,
        StateChangePausedToPlaying = (StatePaused<<3) | StatePlaying,
        StateChangePlayingToPaused = (StatePlaying<<3) | StatePaused,
        StateChangePausedToReady = (StatePaused<<3) | StateReady,
        StateChangeReadyToNull = (StateReady<<3) | StateNull
    };
}
QGST_REGISTER_TYPE(QGst::StateChange)

namespace QGst {
    enum StateChangeReturn {
        StateChangeFailure = 0,
        StateChangeSuccess = 1,
        StateChangeAsync = 2,
        StateChangeNoPreroll = 3
    };
}
QGST_REGISTER_TYPE(QGst::StateChangeReturn)


namespace QGst {
    enum PadDirection {
        PadUnknown,
        PadSrc,
        PadSink
    };
}
QGST_REGISTER_TYPE(QGst::PadDirection)

namespace QGst {
    enum PadFlag {
        PadFlagBlocked = (ObjectFlagLast << 0),
        PadFlagFlushing = (ObjectFlagLast << 1),
        PadFlagEos = (ObjectFlagLast << 2),
        PadFlagBlocking = (ObjectFlagLast << 3),
        PadFlagNeedParent = (ObjectFlagLast << 4),
        PadFlagNeedReconfigure = (ObjectFlagLast << 5),
        PadFlagPendingEvents = (ObjectFlagLast << 6),
        PadFlagFixedCaps = (ObjectFlagLast << 7),
        PadFlagProxyCaps = (ObjectFlagLast << 8),
        PadFlagProxyAllocation = (ObjectFlagLast << 9),
        PadFlagProxyScheduling = (ObjectFlagLast << 10),
        /*padding*/
        PadFlagLast = (ObjectFlagLast << 16)
    };
    Q_DECLARE_FLAGS(PadFlags, PadFlag);
    Q_DECLARE_OPERATORS_FOR_FLAGS(PadFlags);
}
QGST_REGISTER_TYPE(QGst::PadFlags);

namespace QGst {
    enum PadLinkReturn {
        //codegen: PadLinkNoFormat=PAD_LINK_NOFORMAT, PadLinkNoSched=PAD_LINK_NOSCHED
        PadLinkOk = 0,
        PadLinkWrongHierarchy = -1,
        PadLinkWasLinked = -2,
        PadLinkWrongDirection = -3,
        PadLinkNoFormat = -4,
        PadLinkNoSched = -5,
        PadLinkRefused = -6
    };
}
QGST_REGISTER_TYPE(QGst::PadLinkReturn)

namespace QGst {
    enum FlowReturn {
        //codegen: FlowCustomSuccess2=FLOW_CUSTOM_SUCCESS_2, FlowCustomSuccess1=FLOW_CUSTOM_SUCCESS_1, FlowCustomError1=FLOW_CUSTOM_ERROR_1, FlowCustomError2=FLOW_CUSTOM_ERROR_2
        FlowCustomSuccess2 = 102,
        FlowCustomSuccess1 = 101,
        FlowCustomSuccess = 100,
        FlowOk = 0,
        FlowNotLinked = -1,
        FlowFlushing = -2,
        FlowEos = -3,
        FlowNotNegotiated = -4,
        FlowError = -5,
        FlowNotSupported = -6,
        FlowCustomError = -100,
        FlowCustomError1 = -101,
        FlowCustomError2 = -102
    };
}
QGST_REGISTER_TYPE(QGst::FlowReturn)

namespace QGst {
    enum PadMode {
        PadModeNone,
        PadModePush,
        PadModePull
    };
}
QGST_REGISTER_TYPE(QGst::PadMode)


namespace QGst {
    enum Rank {
        RankNone = 0,
        RankMarginal = 64,
        RankSecondary = 128,
        RankPrimary = 256
    };
}
QGST_REGISTER_TYPE(QGst::Rank)


namespace QGst {
    enum MessageType {
        MessageUnknown         = 0,
        MessageEos             = (1 << 0),
        MessageError           = (1 << 1),
        MessageWarning         = (1 << 2),
        MessageInfo            = (1 << 3),
        MessageTag             = (1 << 4),
        MessageBuffering       = (1 << 5),
        MessageStateChanged    = (1 << 6),
        MessageStateDirty      = (1 << 7),
        MessageStepDone        = (1 << 8),
        MessageClockProvide    = (1 << 9),
        MessageClockLost       = (1 << 10),
        MessageNewClock        = (1 << 11),
        MessageStructureChange = (1 << 12),
        MessageStreamStatus    = (1 << 13),
        MessageApplication     = (1 << 14),
        MessageElement         = (1 << 15),
        MessageSegmentStart    = (1 << 16),
        MessageSegmentDone     = (1 << 17),
        MessageDurationChanged = (1 << 18),
        MessageLatency         = (1 << 19),
        MessageAsyncStart      = (1 << 20),
        MessageAsyncDone       = (1 << 21),
        MessageRequestState    = (1 << 22),
        MessageStepStart       = (1 << 23),
        MessageQos             = (1 << 24),
        MessageProgress        = (1 << 25),
        MessageToc             = (1 << 26),
        MessageResetTime       = (1 << 27),
        MessageStreamStart     = (1 << 28),
        MessageNeedContext     = (1 << 29),
        MessageHaveContext     = (1 << 30),
        MessageExtended        = (1 << 31),
        MessageDeviceAdded   /*= MessageExtended + 1*/,
        MessageDeviceRemoved /*= MessageExtended + 2*/,
        MessageAny             = ~0
    };
}
QGST_REGISTER_TYPE(QGst::MessageType)


namespace QGst {
    enum ParseError {
        ParseErrorSyntax,
        ParseErrorNoSuchElement,
        ParseErrorNoSuchProperty,
        ParseErrorLink,
        ParseErrorCouldNotSetProperty,
        ParseErrorEmptyBin,
        ParseErrorEmpty
    };
}
QGST_REGISTER_TYPE(QGst::ParseError)


namespace QGst {
    enum UriType {
        UriUnknown,
        UriSink,
        UriSrc
    };
}
QGST_REGISTER_TYPE(QGst::UriType)

namespace QGst {
    enum BufferingMode {
        BufferingStream,
        BufferingDownload,
        BufferingTimeshift,
        BufferingLive
    };
}
QGST_REGISTER_TYPE(QGst::BufferingMode)

namespace QGst {
    enum Format {
        FormatUndefined,
        FormatDefault,
        FormatBytes,
        FormatTime,
        FormatBuffers,
        FormatPercent
    };
}
QGST_REGISTER_TYPE(QGst::Format)

namespace QGst {
    enum StreamStatusType {
        StreamStatusTypeCreate = 0,
        StreamStatusTypeEnter = 1,
        StreamStatusTypeLeave = 2,
        StreamStatusTypeDestroy = 3,
        StreamStatusTypeStart = 8,
        StreamStatusTypePause = 9,
        StreamStatusTypeStop = 10
    };
}
QGST_REGISTER_TYPE(QGst::StreamStatusType)

namespace QGst {
    enum StreamVolumeFormat {
        StreamVolumeFormatLinear,
        StreamVolumeFormatCubic,
        StreamVolumeFormatDb
    };
}
//QGST_REGISTER_TYPE(QGst::StreamVolumeFormat)

namespace QGst {
    enum ColorBalanceType {
        ColorBalanceHardware,
        ColorBalanceSoftware
    };
}
QGST_REGISTER_TYPE(QGst::ColorBalanceType)

namespace QGst {
    enum QueryTypeFlag {
        QueryTypeUpstream = 1 << 0,
        QueryTypeDownstream = 1 << 1,
        QueryTypeSerialized = 1 << 2,
        QueryTypeBoth = (QueryTypeUpstream | QueryTypeDownstream)
    };
    Q_DECLARE_FLAGS(QueryTypeFlags, QueryTypeFlag);
    Q_DECLARE_OPERATORS_FOR_FLAGS(QueryTypeFlags)
}
QGST_REGISTER_TYPE(QGst::QueryTypeFlags)

namespace QGst {
    enum QueryType {
        QueryUnknown = 0,
        QueryPosition = (10 << 8) | QueryTypeBoth,
        QueryDuration = (20 << 8) | QueryTypeBoth,
        QueryLatency = (30 << 8) | QueryTypeBoth,
        QueryJitter = (40 << 8) | QueryTypeBoth,
        QueryRate = (50 << 8) | QueryTypeBoth,
        QuerySeeking = (60 << 8) | QueryTypeBoth,
        QuerySegment = (70 << 8) | QueryTypeBoth,
        QueryConvert = (80 << 8) | QueryTypeBoth,
        QueryFormats = (90 << 8) | QueryTypeBoth,
        QueryBuffering = (110 << 8) | QueryTypeBoth,
        QueryCustom = (120 << 8) | QueryTypeBoth,
        QueryUri = (130 << 8) | QueryTypeBoth,
        QueryAllocation = (140 << 8) | QueryTypeDownstream | QueryTypeSerialized,
        QueryScheduling = (150 << 8) | QueryTypeUpstream,
        QueryAcceptCaps = (160 << 8) | QueryTypeBoth,
        QueryCaps = (170 << 8) | QueryTypeBoth,
        QueryDrain = (180 << 8) | QueryTypeDownstream | QueryTypeSerialized,
        QueryContext = (190 << 8) | QueryTypeBoth
    };
}
QGST_REGISTER_TYPE(QGst::QueryType)

namespace QGst {
    enum BufferFlag {
        BufferFlagLive = (MiniObjectFlagLast << 0),
        BufferFlagDecodeOnly  = (MiniObjectFlagLast << 1),
        BufferFlagDiscont = (MiniObjectFlagLast << 2),
        BufferFlagResync = (MiniObjectFlagLast << 3),
        BufferFlagCorrupted = (MiniObjectFlagLast << 4),
        BufferFlagMarker = (MiniObjectFlagLast << 5),
        BufferFlagHeader = (MiniObjectFlagLast << 6),
        BufferFlagGap = (MiniObjectFlagLast << 7),
        BufferFlagDroppable = (MiniObjectFlagLast << 8),
        BufferFlagDeltaUnit = (MiniObjectFlagLast << 9),
        BufferFlagLast = (MiniObjectFlagLast << 16)
    };
    Q_DECLARE_FLAGS(BufferFlags, BufferFlag);
    Q_DECLARE_OPERATORS_FOR_FLAGS(BufferFlags)
}
QGST_REGISTER_TYPE(QGst::BufferFlags)


namespace QGst {
    enum EventTypeFlag {
        EventTypeUpstream = 1 << 0,
        EventTypeDownstream = 1 << 1,
        EventTypeSerialized = 1 << 2,
        EventTypeSticky = 1 << 3,
        EventTypeStickyMulti = 1 << 4,
        EventTypeBoth = (EventTypeUpstream | EventTypeDownstream)
    };
    Q_DECLARE_FLAGS(EventTypeFlags, EventTypeFlag);
    Q_DECLARE_OPERATORS_FOR_FLAGS(EventTypeFlags)
}
QGST_REGISTER_TYPE(QGst::EventTypeFlags)

namespace QGst {
    enum EventType {
        //codegen: EventBufferSize=EVENT_BUFFERSIZE
        EventUnknown = (0 << 8),
        EventFlushStart = (10 << 8) | EventTypeBoth,
        EventFlushStop = (20 << 8) | EventTypeBoth | EventTypeSerialized,

        /* downstream */
        EventStreamStart = (40 << 8) | EventTypeDownstream | EventTypeSerialized | EventTypeSticky,
        EventCaps = (50 << 8) | EventTypeDownstream | EventTypeSerialized | EventTypeSticky,
        EventSegment = (70 << 8) | EventTypeDownstream | EventTypeSerialized | EventTypeSticky,
        EventTag = (80 << 8) | EventTypeDownstream | EventTypeSerialized | EventTypeSticky | EventTypeStickyMulti,
        EventBufferSize = (90 << 8) | EventTypeDownstream | EventTypeSerialized | EventTypeSticky,
        EventSinkMessage = (100 << 8) | EventTypeDownstream | EventTypeSerialized | EventTypeSticky | EventTypeStickyMulti,
        EventEos = (110 << 8) | EventTypeDownstream | EventTypeSerialized | EventTypeSticky,
        EventToc = (120 << 8) | EventTypeDownstream | EventTypeSerialized | EventTypeSticky | EventTypeStickyMulti,

        /* non sticky downstream */
        EventSegmentDone = (150 << 8) | EventTypeDownstream | EventTypeSerialized,
        EventGap = (160 << 8) | EventTypeDownstream | EventTypeSerialized,

        /* upstream events */
        EventQos = (190 << 8) | EventTypeUpstream,
        EventSeek = (200 << 8) | EventTypeUpstream,
        EventNavigation = (210 << 8) | EventTypeUpstream,
        EventLatency = (220 << 8) | EventTypeUpstream,
        EventStep = (230 << 8) | EventTypeUpstream,
        EventReconfigure = (240 << 8 ) | EventTypeUpstream,
        EventTocSelect = (250 << 8) | EventTypeUpstream,

        /* custom events */
        EventCustomUpstream = (270 << 8) | EventTypeUpstream,
        EventCustomDownstream = (280 << 8) | EventTypeDownstream | EventTypeSerialized,
        EventCustomDownstreamOob = (290 << 8) | EventTypeDownstream,
        EventCustomDownstreamSticky = (300 << 8) | EventTypeDownstream | EventTypeSerialized | EventTypeSticky | EventTypeStickyMulti,
        EventCustomBoth = (310 << 8) | EventTypeBoth | EventTypeSerialized,
        EventCustomBothOob = (320 << 8) | EventTypeBoth
    };
}
QGST_REGISTER_TYPE(QGst::EventType)

namespace QGst {
    enum SeekFlag {
        SeekFlagNone = 0,
        SeekFlagFlush = (1 << 0),
        SeekFlagAccurate = (1 << 1),
        SeekFlagKeyUnit = (1 << 2),
        SeekFlagSegment = (1 << 3),
        SeekFlagSkip = (1 << 4)
    };
    Q_DECLARE_FLAGS(SeekFlags, SeekFlag);
    Q_DECLARE_OPERATORS_FOR_FLAGS(SeekFlags)
}
QGST_REGISTER_TYPE(QGst::SeekFlags)

namespace QGst {
    enum SeekType {
        SeekTypeNone = 0,
        SeekTypeSet = 1,
        SeekTypeEnd = 2
    };
}
QGST_REGISTER_TYPE(QGst::SeekType)

namespace QGst {
    enum SegmentFlag {
        SegmentFlagNone = SeekFlagNone,
        SegmentFlagReset = SeekFlagFlush,
        SegmentFlagSkip = SeekFlagSkip,
        SegmentFlagSegment = SeekFlagSegment
    };
    Q_DECLARE_FLAGS(SegmentFlags, SegmentFlag);
    Q_DECLARE_OPERATORS_FOR_FLAGS(SegmentFlags);
}
QGST_REGISTER_TYPE(QGst::SegmentFlags);

namespace QGst {
    enum QosType {
        QosTypeOverflow = 0,
        QosTypeUnderflow = 1,
        QosTypeThrottle = 2
    };
}
QGST_REGISTER_TYPE(QGst::QosType);

namespace QGst {
    enum TagMergeMode {
        TagMergeUndefined,
        TagMergeReplaceAll,
        TagMergeReplace,
        TagMergeAppend,
        TagMergePrepend,
        TagMergeKeep,
        TagMergeKeepAll,
        // add more
        TagMergeCount
    };
}
QGST_REGISTER_TYPE(QGst::TagMergeMode)

namespace QGst {
    enum TagFlag {
        TagFlagUndefined,
        TagFlagMeta,
        TagFlagEncoded,
        TagFlagDecoded,
        TagFlagCount
    };
    Q_DECLARE_FLAGS(TagFlags, TagFlag);
}
Q_DECLARE_OPERATORS_FOR_FLAGS(QGst::TagFlags)
QGST_REGISTER_TYPE(QGst::TagFlags) //codegen: GType=GST_TYPE_TAG_FLAG

namespace QGst {
    enum AppStreamType {
        /*! No seeking is supported in the stream, such as a live stream. */
        AppStreamTypeStream,
        /*! The stream is seekable but seeking might not be very
         * fast, such as data from a webserver. */
        AppStreamTypeSeekable,
        /*! The stream is seekable and seeking is fast, such as in a local file. */
        AppStreamTypeRandomAccess
    };
}
//FIXME: GST_TYPE_APP_STREAM_TYPE is not exported from libgstapp,
//so we have to reside in this method...
namespace QGlib {
    template <>
    struct GetTypeImpl<QGst::AppStreamType>
    {
        inline operator Type() { return Type::fromName("GstAppStreamType"); }
    };
}

namespace QGst {
    enum DiscovererResult {
        DiscovererOk,
        DiscovererUriInvalid,
        DiscovererError,
        DiscovererTimeout,
        DiscovererBusy,
        DiscovererMissingPlugins
    };
}
QGST_REGISTER_TYPE(QGst::DiscovererResult)

namespace QGst {
    enum MapFlag {
        MapRead = (1 << 0),
        MapWrite = (1 << 1),
        MapFlagLast = (1 << 16)
    };
    Q_DECLARE_FLAGS(MapFlags, MapFlag);
}
Q_DECLARE_OPERATORS_FOR_FLAGS(QGst::MapFlags)
QGST_REGISTER_TYPE(QGst::MapFlags)

namespace QGst {
    enum MemoryFlag {
        MemoryFlagReadonly = MiniObjectFlagLockReadonly,
        MemoryFlagNoShare = (MiniObjectFlagLast << 0),
        MemoryFlagZeroPrefixed = (MiniObjectFlagLast << 1),
        MemoryFlagZeroPadded = (MiniObjectFlagLast << 2),
        MemoryFlagPhysicallyContiguous = (MiniObjectFlagLast << 3),
        MemoryFlagNotMappable = (MiniObjectFlagLast << 4),
        MemoryFlagLast = (MiniObjectFlagLast << 16)
    };
    Q_DECLARE_FLAGS(MemoryFlags, MemoryFlag);
}
Q_DECLARE_OPERATORS_FOR_FLAGS(QGst::MemoryFlags)
QGST_REGISTER_TYPE(QGst::MemoryFlags)
#endif
