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
#include "parse.h"
#include "element.h"
#include "../QGlib/error.h"
#include <gst/gst.h>

namespace QGst {
namespace Parse {

ElementPtr launch(const char* description)
{
    GError *error = NULL;
    GstElement *e = gst_parse_launch_full(description, NULL, GST_PARSE_FLAG_FATAL_ERRORS, &error);
    if (error) {
        throw QGlib::Error(error);
    }
    if (e) {
        gst_object_ref_sink(e);
    }
    return ElementPtr::wrap(e, false);
}

ElementPtr launch(const char *argv[])
{
    GError *error = NULL;
    GstElement *e = gst_parse_launchv_full(argv, NULL, GST_PARSE_FLAG_FATAL_ERRORS, &error);
    if (error) {
        throw QGlib::Error(error);
    }
    if (e) {
        gst_object_ref_sink(e);
    }
    return ElementPtr::wrap(e, false);
}

} //namespace Parse
} //namespace QGst
