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
#include "element.h"
#include "pad.h"
#include "query.h"
#include "clock.h"
#include "event.h"
#include <gst/gst.h>

namespace QGst {

State Element::currentState() const
{
    State r;
    getState(&r, NULL, 0);
    return r;
}

State Element::pendingState() const
{
    State r;
    getState(NULL, &r, 0);
    return r;
}

StateChangeReturn Element::getState(State *state, State *pending, ClockTime timeout) const
{
    GstState curState, pendingState;
    GstStateChangeReturn result = gst_element_get_state(object<GstElement>(),
                                                        &curState, &pendingState, timeout);
    if (state) {
        *state = static_cast<State>(curState);
    }
    if (pending) {
        *pending = static_cast<State>(pendingState);
    }
    return static_cast<StateChangeReturn>(result);
}

StateChangeReturn Element::setState(State state)
{
    return static_cast<StateChangeReturn>(gst_element_set_state(object<GstElement>(),
                                                                static_cast<GstState>(state)));
}

bool Element::syncStateWithParent()
{
   return gst_element_sync_state_with_parent(object<GstElement>());
}

bool Element::stateIsLocked() const
{
    return gst_element_is_locked_state(object<GstElement>());
}

bool Element::setStateLocked(bool locked)
{
    return gst_element_set_locked_state(object<GstElement>(), locked);
}

bool Element::addPad(const PadPtr & pad)
{
    return gst_element_add_pad(object<GstElement>(), pad);
}

bool Element::removePad(const PadPtr & pad)
{
    return gst_element_remove_pad(object<GstElement>(), pad);
}

PadPtr Element::getStaticPad(const char *name)
{
    GstPad *pad = gst_element_get_static_pad(object<GstElement>(), name);
    return PadPtr::wrap(pad, false);
}

PadPtr Element::getRequestPad(const char *name)
{
    GstPad *pad = gst_element_get_request_pad(object<GstElement>(), name);
    return PadPtr::wrap(pad, false);
}

void Element::releaseRequestPad(const PadPtr & pad)
{
    gst_element_release_request_pad(object<GstElement>(), pad);
}

bool Element::link(const char *srcPadName, const ElementPtr & dest,
                   const char *sinkPadName, const CapsPtr & filter)
{
    return gst_element_link_pads_filtered(object<GstElement>(), srcPadName,
                                          dest, sinkPadName, filter);
}

bool Element::link(const char *srcPadName, const ElementPtr & dest, const CapsPtr & filter)
{
    return link(srcPadName, dest, NULL, filter);
}

bool Element::link(const ElementPtr & dest, const char *sinkPadName, const CapsPtr & filter)
{
    return link(NULL, dest, sinkPadName, filter);
}

bool Element::link(const ElementPtr & dest, const CapsPtr & filter)
{
    return link(NULL, dest, NULL, filter);
}

void Element::unlink(const char *srcPadName, const ElementPtr & dest, const char *sinkPadName)
{
    //FIXME-0.11 This is not entirely correct. Unfortunately I didn't notice
    //that gst_element_unlink_pads requires both pad names when I wrote this
    //function, and it cannot be changed now. For the moment, if the sink
    //pad name is not given, we will assume it is "sink".
    if (!sinkPadName) {
        sinkPadName = "sink";
    }

    gst_element_unlink_pads(object<GstElement>(), srcPadName, dest, sinkPadName);
}

void Element::unlink(const ElementPtr & dest, const char *sinkPadName)
{
    if (sinkPadName) {
        //FIXME-0.11 This is not entirely correct. Unfortunately I didn't notice
        //that gst_element_unlink_pads requires both pad names when I wrote this
        //function, and it cannot be changed now. For the moment, if the source
        //pad name is not given, we will assume it is "src".
        unlink("src", dest, sinkPadName);
    } else {
        gst_element_unlink(object<GstElement>(), dest);
    }
}

bool Element::query(const QueryPtr & query)
{
    return gst_element_query(object<GstElement>(), query);
}

ClockPtr Element::clock() const
{
    return ClockPtr::wrap(gst_element_get_clock(object<GstElement>()), false);
}

bool Element::setClock(const ClockPtr & clock)
{
    return gst_element_set_clock(object<GstElement>(), clock);
}

bool Element::sendEvent(const EventPtr &event)
{
    //Sending an event passes ownership of it, so we need to strong ref() it as we still
    //hold a pointer to the object, and will release it when the wrapper is cleared.
    gst_event_ref(event);
    return gst_element_send_event(object<GstElement>(), event);
}

bool Element::seek(Format format, SeekFlags flags, quint64 position)
{
    return gst_element_seek_simple(object<GstElement>(), static_cast<GstFormat>(format),
                                   static_cast<GstSeekFlags>(static_cast<int>(flags)), position);
}

}
