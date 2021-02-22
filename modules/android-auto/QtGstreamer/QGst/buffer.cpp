/*
    Copyright (C) 2010  Collabora Multimedia.
      @author Mauricio Piacentini <mauricio.piacentini@collabora.co.uk>

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
#include "buffer.h"
#include "caps.h"
#include <QtCore/QDebug>
#include <gst/gst.h>

namespace QGst {
class MapInfo;

BufferPtr Buffer::create(uint size)
{
    return BufferPtr::wrap(gst_buffer_new_allocate(NULL, size, NULL), false);
}

quint32 Buffer::size() const
{
    return gst_buffer_get_size(object<GstBuffer>());
}

ClockTime Buffer::decodingTimeStamp() const
{
    return GST_BUFFER_DTS(object<GstBuffer>());
}

ClockTime Buffer::presentationTimeStamp() const
{
    return GST_BUFFER_PTS(object<GstBuffer>());
}

ClockTime Buffer::duration() const
{
    return GST_BUFFER_DURATION(object<GstBuffer>());
}

quint64 Buffer::offset() const
{
    return GST_BUFFER_OFFSET(object<GstBuffer>());
}

quint64 Buffer::offsetEnd() const
{
    return GST_BUFFER_OFFSET_END(object<GstBuffer>());
}

BufferFlags Buffer::flags() const
{
    return BufferFlags(GST_BUFFER_FLAGS(object<GstBuffer>()));
}

void Buffer::setFlags(const BufferFlags flags)
{
    GST_BUFFER_FLAGS(object<GstBuffer>()) = flags;
}

BufferPtr Buffer::copy() const
{
    return BufferPtr::wrap(gst_buffer_copy(object<GstBuffer>()), false);
}

void Buffer::setSize(uint size)
{
    gst_buffer_set_size(object<GstBuffer>(), size);
}

uint Buffer::extract(uint offset, void *dest, uint size)
{
    return gst_buffer_extract(object<GstBuffer>(), offset, dest, size);
}

uint Buffer::memoryCount() const
{
    return gst_buffer_n_memory (object<GstBuffer>());
}

MemoryPtr Buffer::getMemory(uint index) const
{
    return MemoryPtr::wrap(gst_buffer_get_memory(object<GstBuffer>(), index), false);
}

bool Buffer::map(MapInfo &info, MapFlags flags)
{
    if (!gst_buffer_map(object<GstBuffer>(), static_cast<GstMapInfo *>(info.m_object),
                        static_cast<GstMapFlags>(static_cast<int>(flags)))) {
        return false;
    }
    return true;
}

void Buffer::unmap(MapInfo &info)
{
    gst_buffer_unmap(object<GstBuffer>(), static_cast<GstMapInfo *>(info.m_object));
}

} //namespace QGst
