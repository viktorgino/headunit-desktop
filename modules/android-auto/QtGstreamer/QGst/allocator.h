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

#ifndef QGST_ALLOCATOR_H
#define QGST_ALLOCATOR_H

#include "global.h"
#include "object.h"
#include "memory.h"

namespace QGst {

/*! \headerfile allocator.h <QGst/Allocator>
 * \brief Wrapper class for GstAllocationParams
 */
class QTGSTREAMER_EXPORT AllocationParams
{
public:
    AllocationParams();
    AllocationParams(const AllocationParams & other);
    virtual ~AllocationParams();

    AllocationParams & operator=(const AllocationParams & other);

    MemoryFlags flags() const;
    void setFlags(MemoryFlags flags);

    size_t align() const;
    void setAlign(size_t align);

    size_t prefix() const;
    void setPrefix(size_t align);

    size_t padding() const;
    void setPadding(size_t padding);

    operator GstAllocationParams*();
    operator const GstAllocationParams*() const;

private:
    friend class Allocator;
    GstAllocationParams *d;
};

/*! \headerfile allocator.h <QGst/Allocator>
 * \brief Wrapper class for GstAllocator
 */
class QTGSTREAMER_EXPORT Allocator : public Object
{
    QGST_WRAPPER(Allocator)
public:
    /*! search for an already registered allocator. */
    static AllocatorPtr find(const char *name);

    /*! get the default allocator */
    static AllocatorPtr getDefault();
    /*! get the system memory allocator */
    static AllocatorPtr getSystemMemory();

    /*! create a chunk of memory using this allocator */
    MemoryPtr alloc(size_t size, const AllocationParams &params = AllocationParams());
    /*! release memory allocated with alloc()
     * \note this takes a reference to the memory pointer and makes
     * it null after freeing the memory segment */
    void free(MemoryPtr & memory);
};

} //namespace QGst

QGST_REGISTER_TYPE(QGst::Allocator)

#endif /* QGST_ALLOCATOR_H */
