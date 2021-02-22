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
#ifndef QGST_OBJECT_H
#define QGST_OBJECT_H

#include "global.h"
#include "../QGlib/object.h"

namespace QGst {

/*! \headerfile QGst/object.h <QGst/Object>
 * \brief Wrapper class for GstObject
 */
class QTGSTREAMER_EXPORT Object : public QGlib::Object
{
    QGST_WRAPPER(Object)
public:
    QString name() const;
    bool setName(const char *name);

    ObjectPtr parent() const;
    bool setParent(const ObjectPtr & parent);
    void unparent();

    bool isAncestorOf(const ObjectPtr & object) const;

    QString pathString() const;

protected:
    virtual void ref(bool increaseRef);
    virtual void unref();
};

}

QGST_REGISTER_TYPE(QGst::Object)

#endif // QGST_OBJECT_H
