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
#include "object.h"
#include "../QGlib/string_p.h"
#include <gst/gst.h>

namespace QGst {

QString Object::name() const
{
    return QGlib::Private::stringFromGCharPtr(gst_object_get_name(object<GstObject>()));
}

bool Object::setName(const char *name)
{
    return gst_object_set_name(object<GstObject>(), name);
}

ObjectPtr Object::parent() const
{
    return ObjectPtr::wrap(gst_object_get_parent(object<GstObject>()), false);
}

bool Object::setParent(const ObjectPtr & parent)
{
    return gst_object_set_parent(object<GstObject>(), parent);
}

void Object::unparent()
{
    gst_object_unparent(object<GstObject>());
}

bool Object::isAncestorOf(const ObjectPtr & obj) const
{
    return gst_object_has_ancestor(obj, object<GstObject>());
}

QString Object::pathString() const
{
    return QGlib::Private::stringFromGCharPtr(gst_object_get_path_string(object<GstObject>()));
}

void Object::ref(bool increaseRef)
{
    if (increaseRef) {
        gst_object_ref(m_object);
    }
}

void Object::unref()
{
    gst_object_unref(m_object);
}

}
