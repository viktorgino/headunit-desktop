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
#include "bus.h"
#include "message.h"
#include "../QGlib/Signal"
#include <gst/gst.h>
#include <QtCore/QObject>
#include <QtCore/QTimerEvent>
#include <QtCore/QHash>
#include <QtCore/QBasicTimer>

namespace QGst {
namespace Private {

class BusWatch : public QObject
{
public:
    BusWatch(GstBus *bus)
        : QObject(), m_bus(bus)
    {
        m_timer.start(50, this);
    }

    void stop()
    {
        m_timer.stop();
    }

private:
    virtual void timerEvent(QTimerEvent *event)
    {
        if (event->timerId() == m_timer.timerId()) {
            dispatch();
        } else {
            QObject::timerEvent(event);
        }
    }

    void dispatch()
    {
        GstMessage *message;
        gst_object_ref(m_bus);
        while((message = gst_bus_pop(m_bus)) != NULL) {
            MessagePtr msg = MessagePtr::wrap(message, false);
            QGlib::Quark detail = gst_message_type_to_quark(static_cast<GstMessageType>(msg->type()));
            QGlib::emitWithDetail<void>(m_bus, "message", detail, msg);
        }
        gst_object_unref(m_bus);
    }

    GstBus *m_bus;
    QBasicTimer m_timer;
};

class BusWatchManager
{
public:
    void addWatch(GstBus *bus)
    {
        if (m_watches.contains(bus)) {
            m_watches[bus].second++; //reference count
        } else {
            m_watches.insert(bus, qMakePair(new BusWatch(bus), uint(1)));
            g_object_weak_ref(G_OBJECT(bus), &BusWatchManager::onBusDestroyed, this);
        }
    }

    void removeWatch(GstBus *bus)
    {
        if (m_watches.contains(bus) && --m_watches[bus].second == 0) {
            m_watches[bus].first->stop();
            m_watches[bus].first->deleteLater();
            m_watches.remove(bus);
            g_object_weak_unref(G_OBJECT(bus), &BusWatchManager::onBusDestroyed, this);
        }
    }

private:
    static void onBusDestroyed(gpointer selfPtr, GObject *busPtr)
    {
        BusWatchManager *self = static_cast<BusWatchManager*>(selfPtr);
        GstBus *bus = reinterpret_cast<GstBus*>(busPtr);

        //we cannot call removeWatch() here because g_object_weak_unref will complain
        self->m_watches[bus].first->stop();
        self->m_watches[bus].first->deleteLater();
        self->m_watches.remove(bus);
    }

    QHash< GstBus*, QPair<BusWatch*, uint> > m_watches;
};

Q_GLOBAL_STATIC(Private::BusWatchManager, s_watchManager)

} //namespace Private


//static
BusPtr Bus::create()
{
    GstBus *bus = gst_bus_new();
    if (bus) {
        gst_object_ref_sink(bus);
    }
    return BusPtr::wrap(bus, false);
}

bool Bus::hasPendingMessages() const
{
    return gst_bus_have_pending(object<GstBus>());
}

MessagePtr Bus::peek() const
{
    return MessagePtr::wrap(gst_bus_peek(object<GstBus>()), false);
}

MessagePtr Bus::pop(ClockTime timeout)
{
    return MessagePtr::wrap(gst_bus_timed_pop(object<GstBus>(), timeout), false);
}

MessagePtr Bus::pop(MessageType type, ClockTime timeout)
{
    return MessagePtr::wrap(gst_bus_timed_pop_filtered(object<GstBus>(), timeout,
                                                       static_cast<GstMessageType>(type)), false);
}

bool Bus::post(const MessagePtr & message)
{
    return gst_bus_post(object<GstBus>(), gst_message_copy(message));
}

void Bus::setFlushing(bool flush)
{
    gst_bus_set_flushing(object<GstBus>(), flush);
}

void Bus::addSignalWatch()
{
    Private::s_watchManager()->addWatch(object<GstBus>());
}

void Bus::removeSignalWatch()
{
    Private::s_watchManager()->removeWatch(object<GstBus>());
}

void Bus::enableSyncMessageEmission()
{
    gst_bus_enable_sync_message_emission(object<GstBus>());
}

void Bus::disableSyncMessageEmission()
{
    gst_bus_disable_sync_message_emission(object<GstBus>());
}

} //namespace QGst
