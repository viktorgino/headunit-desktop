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
#ifndef QGST_MINIOBJECT_H
#define QGST_MINIOBJECT_H

#include "global.h"
#include "../QGlib/refpointer.h"
#include "../QGlib/type.h"

namespace QGst {

/*! \headerfile miniobject.h <QGst/MiniObject>
 * \brief Wrapper class for GstMiniObject
 */
class QTGSTREAMER_EXPORT MiniObject : public QGlib::RefCountedObject
{
// expanded QGST_WRAPPER(MiniObject) but without MiniObject_new()
// this is to avoid codegen picking it up, since MiniObject has no
// GType and cannot be handled properly
public:
    typedef GstMiniObject CType;
protected:
    MiniObject() {}
    MiniObject(const MiniObject &);
    MiniObject & operator=(const MiniObject &);
    ~MiniObject() {}

public:
    MiniObjectPtr copy() const;
    bool isWritable() const;
    MiniObjectPtr makeWritable() const;

protected:
    virtual void ref(bool increaseRef);
    virtual void unref();
};


namespace Private {

QTGSTREAMER_EXPORT QGlib::RefCountedObject *wrapMiniObject(void *miniObject);

} //namespace Private
} //namespace QGst

QGLIB_REGISTER_WRAPIMPL_FOR_SUBCLASSES_OF(QGst::MiniObject, QGst::Private::wrapMiniObject)

#endif
