/*
    Copyright (C) 2014  Diane Trout <diane@ghic.org>

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
#include <gst/gst.h>

namespace QGst {

AllocationParams::AllocationParams()
  : d(g_slice_new0(GstAllocationParams))
{
    gst_allocation_params_init(d);
}

AllocationParams::AllocationParams(const AllocationParams & other)
  : d(gst_allocation_params_copy(other.d))
{
}

AllocationParams::~AllocationParams()
{
    gst_allocation_params_free(d);
}

AllocationParams & AllocationParams::operator=(const AllocationParams & other)
{
    gst_allocation_params_free(d);
    d = gst_allocation_params_copy(other.d);
    return *this;
}

MemoryFlags AllocationParams::flags() const
{
    return static_cast<QGst::MemoryFlags>(static_cast<unsigned int>(d->flags));
}

void AllocationParams::setFlags(MemoryFlags flags)
{
    d->flags = static_cast<GstMemoryFlags>(static_cast<unsigned int>(flags));
}

size_t AllocationParams::align() const
{
    return d->align;
}

void AllocationParams::setAlign(size_t align)
{
    d->align = align;
}

size_t AllocationParams::prefix() const
{
    return d->prefix;
}

void AllocationParams::setPrefix(size_t align)
{
    d->prefix = align;
}

size_t AllocationParams::padding() const
{
    return d->padding;
}

void AllocationParams::setPadding(size_t padding)
{
    d->padding = padding;
}

AllocationParams::operator const GstAllocationParams*() const
{
    return d;
}

AllocationParams::operator GstAllocationParams*()
{
    return d;
}

//static
AllocatorPtr Allocator::find(const char *name)
{
    return AllocatorPtr::wrap(gst_allocator_find(name), false);
}

//static
AllocatorPtr Allocator::getDefault()
{
    return find(NULL);
}

//static
AllocatorPtr Allocator::getSystemMemory()
{
    return find(GST_ALLOCATOR_SYSMEM);
}

MemoryPtr Allocator::alloc(size_t size, const AllocationParams & params)
{
    return MemoryPtr::wrap(gst_allocator_alloc(object<GstAllocator>(), size,
            const_cast<GstAllocationParams *>(static_cast<const GstAllocationParams *>(params))), false);
}

void Allocator::free(MemoryPtr & memory)
{
    GstMemory *mem = memory;
    gst_memory_ref(mem);
    memory.clear();
    gst_allocator_free(object<GstAllocator>(), mem);
}

} /* QGst */
