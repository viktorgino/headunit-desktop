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
#include "ghostpad.h"
#include <gst/gst.h>

namespace QGst {

GhostPadPtr GhostPad::create(const PadPtr & target, const char *name)
{
    GstPad *gp = gst_ghost_pad_new(name, target);
    if (gp) {
        gst_object_ref_sink(gp);
    }
    return GhostPadPtr::wrap(GST_GHOST_PAD(gp), false);
}

GhostPadPtr GhostPad::create(PadDirection direction, const char *name)
{
    GstPad *gp = gst_ghost_pad_new_no_target(name, static_cast<GstPadDirection>(direction));
    if (gp) {
        gst_object_ref_sink(gp);
    }
    return GhostPadPtr::wrap(GST_GHOST_PAD(gp), false);
}

PadPtr GhostPad::target() const
{
    return PadPtr::wrap(gst_ghost_pad_get_target(object<GstGhostPad>()), false);
}

bool GhostPad::setTarget(const PadPtr & target)
{
    return gst_ghost_pad_set_target(object<GstGhostPad>(), target);
}

}
