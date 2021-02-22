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
#ifndef QGST_MEMORY_H
#define QGST_MEMORY_H

#include "global.h"
#include "miniobject.h"

namespace QGst {

class QTGSTREAMER_EXPORT MapInfo
{
public:
    MapInfo();
    virtual ~MapInfo();

    MapFlags flags() const;
    quint8 *data() const;
    size_t size() const;
    size_t maxSize() const;

private:
    friend class Buffer;
    friend class Memory;
    Q_DISABLE_COPY(MapInfo);

    void *m_object;
};

/*! \headerfile memory.h <QGst/QGstMemory>
 *  \brief Wrapper class for GstMemory
 *
 * GstMemory is a lightweight refcounted object that wraps a region
 * of memory. They are typically used to manage the data of a
 * GstBuffer.
 */
class QTGSTREAMER_EXPORT Memory : public MiniObject
{
    QGST_WRAPPER(Memory)
public:
    QGst::AllocatorPtr allocator() const;

    size_t size() const;
    size_t offset() const;
    size_t maxSize() const;

    bool isType(const char *type) const;

    bool map(MapInfo &info, MapFlags flags);
    void unmap(MapInfo &info);
};

} // namespace QGst

QGST_REGISTER_TYPE(QGst::Memory)

#endif
