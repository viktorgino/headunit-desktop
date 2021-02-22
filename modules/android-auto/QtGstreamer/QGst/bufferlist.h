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
#ifndef QGST_BUFFERLIST_H
#define QGST_BUFFERLIST_H

#include "buffer.h"

namespace QGst {

/*! \headerfile bufferlist.h <QGst/BufferList>
 * \brief Wrapper class for GstBufferList
 *
 * Buffer lists are units of grouped scatter/gather data transfer in GStreamer.
 *
 * \sa BufferListIterator
 */
class QTGSTREAMER_EXPORT BufferList : public MiniObject
{
    QGST_WRAPPER(BufferList)
public:
    static BufferListPtr create();

    uint length() const;
    BufferPtr bufferAt(uint index) const;

    inline BufferListPtr copy() const;
    inline BufferListPtr makeWritable() const;
};

inline BufferListPtr BufferList::copy() const
{
    return MiniObject::copy().staticCast<BufferList>();
}

inline BufferListPtr BufferList::makeWritable() const
{
    return MiniObject::makeWritable().staticCast<BufferList>();
}

} //namespace QGst

QGST_REGISTER_TYPE(QGst::BufferList)

#endif // QGST_BUFFERLIST_H
