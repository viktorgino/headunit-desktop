/*
    Copyright (C) 2013  Diane Trout <diane@ghic.org>

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
#include "allocator.h"
#include "memory.h"
#include "buffer.h"
#include <gst/gst.h>

namespace QGst {

MapInfo::MapInfo()
{
    m_object = g_slice_new0(GstMapInfo);
}

MapInfo::~MapInfo()
{
    g_slice_free(GstMapInfo, m_object);
}

MapFlags MapInfo::flags() const
{
    return static_cast<MapFlag>(static_cast<GstMapInfo*>(m_object)->flags);
}

quint8 *MapInfo::data() const
{
    return static_cast<GstMapInfo*>(m_object)->data;
}

size_t MapInfo::size() const
{
    return static_cast<GstMapInfo*>(m_object)->size;
}

size_t MapInfo::maxSize() const
{
    return static_cast<GstMapInfo*>(m_object)->maxsize;
}

//-----------------------

AllocatorPtr Memory::allocator() const
{
    return AllocatorPtr::wrap(object<GstMemory>()->allocator);
}

size_t Memory::size() const
{
    return object<GstMemory>()->size;
}

size_t Memory::offset() const
{
    return object<GstMemory>()->offset;
}

size_t Memory::maxSize() const
{
    return object<GstMemory>()->maxsize;
}

bool Memory::isType(const char *type) const
{
    return gst_memory_is_type(object<GstMemory>(), type);
}

bool Memory::map(MapInfo &info, MapFlags flags)
{
    return gst_memory_map(object<GstMemory>(), static_cast<GstMapInfo*>(info.m_object),
                          static_cast<GstMapFlags>(static_cast<int>(flags)));
}

void Memory::unmap(MapInfo &info)
{
    gst_memory_unmap(object<GstMemory>(), static_cast<GstMapInfo*>(info.m_object));
}

} // namespace QGst

