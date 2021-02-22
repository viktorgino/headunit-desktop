/*
    Copyright (C) 2009  George Kiagiadakis <kiagiadakis.george@gmail.com>

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
#include "pad.h"
#include "caps.h"
#include "element.h"
#include "query.h"
#include "event.h"
#include <QtCore/QDebug>
#include <gst/gst.h>

namespace QGst {

//static
PadPtr Pad::create(PadDirection direction, const char *name)
{
    GstPad *pad = gst_pad_new(name, static_cast<GstPadDirection>(direction));
    if (pad) {
        gst_object_ref_sink(pad);
    }
    return PadPtr::wrap(pad, false);
}

PadDirection Pad::direction() const
{
    return static_cast<PadDirection>(gst_pad_get_direction(object<GstPad>()));
}

ElementPtr Pad::parentElement() const
{
    return ElementPtr::wrap(gst_pad_get_parent_element(object<GstPad>()), false);
}

PadPtr Pad::peer() const
{
    return PadPtr::wrap(gst_pad_get_peer(object<GstPad>()), false);
}

bool Pad::isLinked() const
{
    return gst_pad_is_linked(object<GstPad>());
}

bool Pad::canLink(const PadPtr & sink) const
{
    return gst_pad_can_link(object<GstPad>(), sink);
}

PadLinkReturn Pad::link(const PadPtr & sink)
{
    return static_cast<PadLinkReturn>(gst_pad_link(object<GstPad>(), sink));
}

bool Pad::unlink(const PadPtr & sink)
{
    return gst_pad_unlink(object<GstPad>(), sink);
}

CapsPtr Pad::currentCaps() const
{
    return CapsPtr::wrap(gst_pad_get_current_caps(object<GstPad>()), false);
}

CapsPtr Pad::allowedCaps() const
{
    return CapsPtr::wrap(gst_pad_get_allowed_caps(object<GstPad>()), false);
}

CapsPtr Pad::padTemplateCaps() const
{
    return CapsPtr::wrap(gst_pad_get_pad_template_caps(object<GstPad>()), false);
}

bool Pad::isActive() const
{
    return gst_pad_is_active(object<GstPad>());
}

bool Pad::setActive(bool active)
{
    return gst_pad_set_active(object<GstPad>(), active);
}

bool Pad::isBlocked() const
{
    return gst_pad_is_blocked(object<GstPad>());
}

bool Pad::isBlocking() const
{
    return gst_pad_is_blocking(object<GstPad>());
}

bool Pad::query(const QueryPtr & query)
{
    return gst_pad_query(object<GstPad>(), query);
}

bool Pad::sendEvent(const EventPtr &event)
{
    //Sending an event passes ownership of it, so we need to strong ref() it as we still
    //hold a pointer to the object, and will release it when the wrapper is cleared.
    gst_event_ref(event);
    return gst_pad_send_event(object<GstPad>(), event);
}

}
