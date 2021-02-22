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
#ifndef QGST_CHILDPROXY_H
#define QGST_CHILDPROXY_H

#include "object.h"

namespace QGst {

/*! \interface ChildProxy childproxy.h <QGst/ChildProxy>
 * \brief Wrapper class for GstChildProxy
 */
class QTGSTREAMER_EXPORT ChildProxy : public QGlib::Interface
{
    QGST_WRAPPER(ChildProxy)
public:
    uint childrenCount() const;
    QGlib::ObjectPtr childByName(const char *name) const;
    QGlib::ObjectPtr childByIndex(uint index) const;

    bool findChildProperty(const char *name, QGlib::ObjectPtr *object,
                           QGlib::ParamSpecPtr *paramSpec) const;
    QGlib::Value childProperty(const char *name) const;
    template <typename T> void setChildProperty(const char *name, const T & value);
};

template <typename T>
void ChildProxy::setChildProperty(const char *name, const T & value)
{
    QGlib::ParamSpecPtr param;
    QGlib::ObjectPtr object;
    if (findChildProperty(name, &object, &param)) {
        QGlib::Value v;
        v.init(param->valueType());
        v.set<T>(value);
        object->setProperty(param->name(), value);
    }
}

}

QGST_REGISTER_TYPE(QGst::ChildProxy)
QGLIB_REGISTER_INTERFACE(QGst::ChildProxy)

#endif // QGST_CHILDPROXY_H
