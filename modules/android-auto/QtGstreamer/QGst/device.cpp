/*
    Copyright (C) 2015  Pavel Bludov <pbludov@gmail.com>

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
#include "device.h"
#include "caps.h"
#include "element.h"
#include "../QGlib/string_p.h"
#include <QtCore/QDebug>
#include <gst/gst.h>

namespace QGst {

ElementPtr Device::createElement(const char* name) const
{
    GstElement *e = gst_device_create_element(object<GstDevice>(), name);
    if (e) {
        gst_object_ref_sink(e);
    }
    return ElementPtr::wrap(e, false);
}

CapsPtr Device::caps() const
{
    return CapsPtr::wrap(gst_device_get_caps(object<GstDevice>()), false);
}

QString Device::deviceClass() const
{
    return QGlib::Private::stringFromGCharPtr(gst_device_get_device_class(object<GstDevice>()));
}

QString Device::displayName() const
{
    return QGlib::Private::stringFromGCharPtr(gst_device_get_display_name(object<GstDevice>()));
}

bool Device::hasClasses(const char *classes) const
{
    return gst_device_has_classes(object<GstDevice>(), classes);
}

bool Device::reconfigureElement(const ElementPtr & element) const
{
    return gst_device_reconfigure_element(object<GstDevice>(), element);
}

} //namespace QGst
