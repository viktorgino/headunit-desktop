/*
    Copyright (C) 2011 Collabora Ltd. <info@collabora.co.uk>
      @author George Kiagiadakis <george.kiagiadakis@collabora.co.uk>

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
#include "bufferlist.h"
#include <gst/gst.h>

namespace QGst {

BufferListPtr BufferList::create()
{
    return BufferListPtr::wrap(gst_buffer_list_new(), false);
}

uint BufferList::length() const
{
    return gst_buffer_list_length(object<GstBufferList>());
}

BufferPtr BufferList::bufferAt(uint index) const
{
    return BufferPtr::wrap(gst_buffer_list_get(object<GstBufferList>(), index));
}

} //namespace QGst

