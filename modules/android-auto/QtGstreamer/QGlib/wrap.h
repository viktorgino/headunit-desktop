/*
    Copyright (C) 2010 Collabora Ltd. <info@collabora.co.uk>
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
#ifndef QGLIB_WRAP_H
#define QGLIB_WRAP_H

#include "global.h"

namespace QGlib {

class RefCountedObject;
class Type;

/*! This function constructs a RefCountedObject that wraps the given \a instance,
 * which is of type \a instanceType. It returns a C++ wrapper class that inherits
 * RefCountedObject and wraps \a instanceType objects in the best possible way.
 *
 * This function is provided for implementing bindings that use QtGLib. It is only
 * needed if those bindings have some reference counted instance type that is not
 * an Object or Interface or ParamSpec (like QGst::MiniObject and QGst::Caps).
 * You should \em not otherwise call this function directly.
 */
QTGLIB_EXPORT RefCountedObject *constructWrapper(Type instanceType, void *instance);

template <typename T, typename Enable = void>
struct WrapImpl {};

#define QGLIB_REGISTER_WRAPIMPL_FOR_SUBCLASSES_OF(BaseClass, WrapFunc) \
    namespace QGlib { \
        template <typename T> \
        struct WrapImpl<T, typename boost::enable_if< boost::is_base_of<BaseClass, T> >::type > \
        { \
            static inline RefCountedObject *wrap(typename T::CType *object) \
            { \
                return WrapFunc(object); \
            } \
        }; \
    } //namespace QGlib

#define QGLIB_REGISTER_INTERFACE(IfaceClass) \
    namespace QGlib { \
        template <> \
        struct WrapImpl<IfaceClass, void> \
        { \
            static inline RefCountedObject *wrap(IfaceClass::CType *object) \
            { \
                return Private::wrapInterface(GetType<IfaceClass>(), object); \
            } \
        }; \
    } //namespace QGlib

namespace Private {

QTGLIB_EXPORT RefCountedObject *wrapObject(void *gobject);
QTGLIB_EXPORT RefCountedObject *wrapParamSpec(void *param);
QTGLIB_EXPORT RefCountedObject *wrapInterface(Type interfaceType, void *gobject);

} //namespace Private
} //namespace QGlib


#endif // QGLIB_WRAP_H
