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
#include "miniobject.h"
#include "objectstore_p.h"
#include <gst/gst.h>

namespace QGst {

MiniObjectPtr MiniObject::copy() const
{
    return MiniObjectPtr::wrap(gst_mini_object_copy(object<GstMiniObject>()), false);
}

bool MiniObject::isWritable() const
{
    return gst_mini_object_is_writable(object<GstMiniObject>());
}

void MiniObject::ref(bool increaseRef)
{
    if (Private::ObjectStore::put(this)) {
        if (increaseRef) {
            gst_mini_object_ref(GST_MINI_OBJECT(m_object));
        }
    }
}

void MiniObject::unref()
{
    if (Private::ObjectStore::take(this)) {
        gst_mini_object_unref(GST_MINI_OBJECT(m_object));
        delete this;
    }
}

MiniObjectPtr MiniObject::makeWritable() const
{
    /*
     * Calling gst_*_make_writable() below is tempting but wrong.
     * Since MiniObjects and Caps do not share the same C++ instance in various wrappings, calling
     * gst_*_make_writable() on an already writable object and wrapping the result is wrong,
     * since it would just return the same pointer and we would wrap it in a new C++ instance.
     */
    if (!isWritable()) {
        return copy();
    } else {
        return MiniObjectPtr(const_cast<MiniObject*>(this));
    }
}


namespace Private {

QGlib::RefCountedObject *wrapMiniObject(void *miniObject)
{
    return QGlib::constructWrapper(GST_MINI_OBJECT_TYPE(miniObject), miniObject);
}

} //namespace Private
} //namespace QGst
