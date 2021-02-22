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
#include "bin.h"
#include "pad.h"
#include "../QGlib/error.h"
#include <gst/gst.h>

namespace QGst {

//static
BinPtr Bin::create(const char *name)
{
    GstElement *bin = gst_bin_new(name);
    if (bin) {
        gst_object_ref_sink(bin);
    }
    return BinPtr::wrap(GST_BIN(bin), false);
}

//static
BinPtr Bin::fromDescription(const char *description, BinFromDescriptionOption ghostUnlinkedPads)
{
    GError *error = NULL;
    GstElement *e = gst_parse_bin_from_description_full(description, ghostUnlinkedPads,
                                                        NULL, GST_PARSE_FLAG_FATAL_ERRORS, &error);
    if (error) {
        throw QGlib::Error(error);
    }
    if (e) {
        gst_object_ref_sink(e);
    }
    return BinPtr::wrap(GST_BIN(e), false);
}

bool Bin::add(const ElementPtr & element)
{
    return gst_bin_add(object<GstBin>(), element);
}

bool Bin::remove(const ElementPtr & element)
{
    return gst_bin_remove(object<GstBin>(), element);
}

ElementPtr Bin::getElementByName(const char *name, RecursionType r) const
{
    GstElement *e = NULL;
    switch(r) {
    case RecurseDown:
        e = gst_bin_get_by_name(object<GstBin>(), name);
        break;
    case RecurseUp:
        e = gst_bin_get_by_name_recurse_up(object<GstBin>(), name);
        break;
    default:
        Q_ASSERT_X(false, "QGst::Bin::getElementByName", "Invalid RecursionType");
    }
    return ElementPtr::wrap(e, false);
}

ElementPtr Bin::getElementByInterface(QGlib::Type interfaceType) const
{
    return ElementPtr::wrap(gst_bin_get_by_interface(object<GstBin>(), interfaceType), false);
}

PadPtr Bin::findUnlinkedPad(PadDirection direction) const
{
    return PadPtr::wrap(gst_bin_find_unlinked_pad(object<GstBin>(),
                                                  static_cast<GstPadDirection>(direction)), false);
}

bool Bin::recalculateLatency()
{
    return gst_bin_recalculate_latency(object<GstBin>());
}

}
