/*
    Copyright (C) 2010  George Kiagiadakis <kiagiadakis.george@gmail.com>

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
#ifndef QGST_GLOBAL_H
#define QGST_GLOBAL_H

// workaround for https://bugreports.qt-project.org/browse/QTBUG-22829
#if defined(Q_MOC_RUN) && !defined(BOOST_TT_HAS_OPERATOR_HPP_INCLUDED)
#define BOOST_TT_HAS_OPERATOR_HPP_INCLUDED
#endif

#include "../QGlib/type.h"
#include <QtCore/QtGlobal>
#include <QtCore/QDate>
#include <QtCore/QSharedPointer>

/* defined by cmake when building this library */
#if defined(QtGStreamer_EXPORTS) || defined(Qt5GStreamer_EXPORTS)
# define QTGSTREAMER_EXPORT Q_DECL_EXPORT
#else
# define QTGSTREAMER_EXPORT Q_DECL_IMPORT
#endif

#if !defined(Q_OS_WIN) && !defined(Q_CC_NOKIAX86) && \
    !defined(Q_CC_RVCT) && defined(QT_VISIBILITY_AVAILABLE)
# define QTGSTREAMER_NO_EXPORT __attribute__((visibility("hidden")))
#else
# define QTGSTREAMER_NO_EXPORT
#endif

#define QGST_REGISTER_TYPE(T) \
    QGLIB_REGISTER_TYPE_WITH_EXPORT_MACRO(T, QTGSTREAMER_EXPORT)

//cyclic dependency, must include after defining the above
#include "enums.h"

//registered in value.cpp
QGST_REGISTER_TYPE(QDate) //codegen: skip=true
QGST_REGISTER_TYPE(QDateTime) //codegen: skip=true


#define QGST_WRAPPER_GSTCLASS_DECLARATION(Class) \
    typedef struct _Gst##Class Gst##Class;

#define QGST_WRAPPER_DIFFERENT_GSTCLASS_DECLARATION(Class, CClass) \
    typedef struct _Gst##CClass Gst##Class;

#define QGST_WRAPPER_REFPOINTER_DECLARATION(Class) \
    namespace QGst { \
        class Class; \
        typedef QGlib::RefPointer<Class> Class##Ptr; \
    }

#define QGST_WRAPPER_DECLARATION(Class) \
    QGST_WRAPPER_GSTCLASS_DECLARATION(Class) \
    QGST_WRAPPER_REFPOINTER_DECLARATION(Class)

QGST_WRAPPER_DECLARATION(Bin)
QGST_WRAPPER_DECLARATION(Bus)
QGST_WRAPPER_DECLARATION(Caps)
QGST_WRAPPER_DECLARATION(ChildProxy)
QGST_WRAPPER_DECLARATION(Clock)
QGST_WRAPPER_DECLARATION(ColorBalanceChannel)
QGST_WRAPPER_DECLARATION(ColorBalance)
QGST_WRAPPER_DECLARATION(Device)
QGST_WRAPPER_DECLARATION(DeviceMonitor)
QGST_WRAPPER_DECLARATION(Discoverer)
QGST_WRAPPER_DECLARATION(DiscovererInfo)
QGST_WRAPPER_DECLARATION(DiscovererStreamInfo)
QGST_WRAPPER_DECLARATION(DiscovererContainerInfo)
QGST_WRAPPER_DECLARATION(DiscovererAudioInfo)
QGST_WRAPPER_DECLARATION(DiscovererVideoInfo)
QGST_WRAPPER_DECLARATION(DiscovererSubtitleInfo)
QGST_WRAPPER_DECLARATION(Element)
QGST_WRAPPER_DECLARATION(ElementFactory)
QGST_WRAPPER_DECLARATION(GhostPad)
QGST_WRAPPER_DECLARATION(Message)
QGST_WRAPPER_REFPOINTER_DECLARATION(EosMessage)
QGST_WRAPPER_REFPOINTER_DECLARATION(ErrorMessage)
QGST_WRAPPER_REFPOINTER_DECLARATION(WarningMessage)
QGST_WRAPPER_REFPOINTER_DECLARATION(InfoMessage)
QGST_WRAPPER_REFPOINTER_DECLARATION(TagMessage)
QGST_WRAPPER_REFPOINTER_DECLARATION(BufferingMessage)
QGST_WRAPPER_REFPOINTER_DECLARATION(StateChangedMessage)
QGST_WRAPPER_REFPOINTER_DECLARATION(StepDoneMessage)
QGST_WRAPPER_REFPOINTER_DECLARATION(StreamStatusMessage)
QGST_WRAPPER_REFPOINTER_DECLARATION(ApplicationMessage)
QGST_WRAPPER_REFPOINTER_DECLARATION(ElementMessage)
QGST_WRAPPER_REFPOINTER_DECLARATION(SegmentDoneMessage)
QGST_WRAPPER_REFPOINTER_DECLARATION(DurationChangedMessage)
QGST_WRAPPER_REFPOINTER_DECLARATION(LatencyMessage)
QGST_WRAPPER_REFPOINTER_DECLARATION(AsyncDoneMessage)
QGST_WRAPPER_REFPOINTER_DECLARATION(RequestStateMessage)
QGST_WRAPPER_REFPOINTER_DECLARATION(StepStartMessage)
QGST_WRAPPER_REFPOINTER_DECLARATION(QosMessage)
QGST_WRAPPER_REFPOINTER_DECLARATION(DeviceAddedMessage)
QGST_WRAPPER_REFPOINTER_DECLARATION(DeviceRemovedMessage)
QGST_WRAPPER_DECLARATION(MiniObject)
QGST_WRAPPER_DECLARATION(Object)
QGST_WRAPPER_DECLARATION(Pad)
QGST_WRAPPER_DECLARATION(Pipeline)
QGST_WRAPPER_DECLARATION(PluginFeature)
QGST_WRAPPER_DECLARATION(Query)
QGST_WRAPPER_REFPOINTER_DECLARATION(PositionQuery)
QGST_WRAPPER_REFPOINTER_DECLARATION(DurationQuery)
QGST_WRAPPER_REFPOINTER_DECLARATION(LatencyQuery)
QGST_WRAPPER_REFPOINTER_DECLARATION(SeekingQuery)
QGST_WRAPPER_REFPOINTER_DECLARATION(SegmentQuery)
QGST_WRAPPER_REFPOINTER_DECLARATION(ConvertQuery)
QGST_WRAPPER_REFPOINTER_DECLARATION(FormatsQuery)
QGST_WRAPPER_REFPOINTER_DECLARATION(BufferingQuery)
QGST_WRAPPER_REFPOINTER_DECLARATION(UriQuery)
QGST_WRAPPER_DECLARATION(Buffer)
QGST_WRAPPER_DECLARATION(Allocator)
QGST_WRAPPER_DECLARATION(Memory)
QGST_WRAPPER_DECLARATION(BufferList)
QGST_WRAPPER_DECLARATION(Event)
QGST_WRAPPER_REFPOINTER_DECLARATION(FlushStartEvent)
QGST_WRAPPER_REFPOINTER_DECLARATION(FlushStopEvent)
QGST_WRAPPER_REFPOINTER_DECLARATION(CapsEvent)
QGST_WRAPPER_REFPOINTER_DECLARATION(EosEvent)
QGST_WRAPPER_REFPOINTER_DECLARATION(SegmentEvent)
QGST_WRAPPER_REFPOINTER_DECLARATION(TagEvent)
QGST_WRAPPER_REFPOINTER_DECLARATION(BufferSizeEvent)
QGST_WRAPPER_REFPOINTER_DECLARATION(SinkMessageEvent)
QGST_WRAPPER_REFPOINTER_DECLARATION(QosEvent)
QGST_WRAPPER_REFPOINTER_DECLARATION(SeekEvent)
QGST_WRAPPER_REFPOINTER_DECLARATION(NavigationEvent)
QGST_WRAPPER_REFPOINTER_DECLARATION(LatencyEvent)
QGST_WRAPPER_REFPOINTER_DECLARATION(StepEvent)
QGST_WRAPPER_DECLARATION(StreamVolume)
QGST_WRAPPER_DECLARATION(Sample)
QGST_WRAPPER_GSTCLASS_DECLARATION(Structure)
QGST_WRAPPER_GSTCLASS_DECLARATION(TagList)
QGST_WRAPPER_GSTCLASS_DECLARATION(Segment)
QGST_WRAPPER_GSTCLASS_DECLARATION(AllocationParams)
namespace QGst {
    class Structure;
    class SharedStructure;
    typedef QSharedPointer<SharedStructure> StructurePtr;
    typedef QSharedPointer<const SharedStructure> StructureConstPtr;
    class AllocationParams;
    class MapInfo;
    class Segment;
}
QGST_WRAPPER_GSTCLASS_DECLARATION(URIHandler)
QGST_WRAPPER_REFPOINTER_DECLARATION(UriHandler)
QGST_WRAPPER_DECLARATION(VideoOrientation)
QGST_WRAPPER_DECLARATION(VideoOverlay)

#undef QGST_WRAPPER_DECLARATION
#undef QGST_WRAPPER_REFPOINTER_DECLARATION
#undef QGST_WRAPPER_GSTCLASS_DECLARATION


#define QGST_WRAPPER(Class) \
    QGLIB_WRAPPER_DECLARATION_MACRO(Class, Class, Gst, Class)

#define QGST_WRAPPER_DIFFERENT_C_CLASS(Class, CClass) \
    QGLIB_WRAPPER_DECLARATION_MACRO(Class, CClass, Gst, Class)

#define QGST_WRAPPER_FAKE_SUBCLASS(Sub, Class) \
    QGLIB_WRAPPER_DECLARATION_MACRO(Sub##Class, Class, Gst, Class)

#define QGST_REGISTER_SUBCLASS(BASECLASS, TYPE) \
    namespace QGlib { \
        template <> \
        struct GetTypeImpl<QGst::TYPE##BASECLASS> { \
            inline operator Type() { return GetType<QGst::BASECLASS>(); }; \
        }; \
    }

#endif
