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
#ifndef QGLIB_GLOBAL_H
#define QGLIB_GLOBAL_H

// workaround for https://bugreports.qt-project.org/browse/QTBUG-22829
#if defined(Q_MOC_RUN) && !defined(BOOST_TT_HAS_OPERATOR_HPP_INCLUDED)
#define BOOST_TT_HAS_OPERATOR_HPP_INCLUDED
#endif

#include <QtCore/QtGlobal>
#include <boost/config.hpp>

/* defined by cmake when building this library */
#if defined(QtGLib_EXPORTS) || defined(Qt5GLib_EXPORTS)
# define QTGLIB_EXPORT Q_DECL_EXPORT
#else
# define QTGLIB_EXPORT Q_DECL_IMPORT
#endif

#if !defined(Q_OS_WIN) && !defined(Q_CC_NOKIAX86) && \
    !defined(Q_CC_RVCT) && defined(QT_VISIBILITY_AVAILABLE)
# define QTGLIB_NO_EXPORT __attribute__((visibility("hidden")))
#else
# define QTGLIB_NO_EXPORT
#endif

typedef struct _GValue GValue;
typedef struct _GParamSpec GParamSpec;
typedef struct _GClosure GClosure;
typedef struct _GObject GObject;
typedef struct _GError GError;

namespace QGlib {

class Error;
class Value;
class Quark;
class Type;
class Signal;
class SignalHandler;
template <class T> class RefPointer;
class ParamSpec;
typedef RefPointer<ParamSpec> ParamSpecPtr;
class Object;
typedef RefPointer<Object> ObjectPtr;

} //namespace QGlib


#define QGLIB_WRAPPER_DECLARATION_MACRO(CppClass, CClass, CNamespace, FakeSuperClass) \
    public: \
        typedef CNamespace##CClass CType; \
    protected: \
        CppClass() {} \
        CppClass(const CppClass &); \
        CppClass & operator=(const CppClass &); \
        ~CppClass() {} \
        friend QGlib::RefCountedObject* FakeSuperClass##_new(void*); \
    private:

#define QGLIB_WRAPPER(Class) \
    QGLIB_WRAPPER_DECLARATION_MACRO(Class, Class, G, Class)

#define QGLIB_WRAPPER_DIFFERENT_C_CLASS(Class, CClass) \
    QGLIB_WRAPPER_DECLARATION_MACRO(Class, CClass, G, Class)


#if !defined(BOOST_NO_STATIC_ASSERT) //we have c++0x static_assert
# define QGLIB_STATIC_ASSERT(expr, message) static_assert(expr, message)
# define QGLIB_HAVE_CXX0X_STATIC_ASSERT 1
#else
# include <boost/static_assert.hpp>
# define QGLIB_STATIC_ASSERT(expr, message) BOOST_STATIC_ASSERT(expr)
#endif

//check for the C++0x features that we need
#if !defined(BOOST_NO_VARIADIC_TEMPLATES) && !defined(BOOST_NO_RVALUE_REFERENCES)
# define QGLIB_HAVE_CXX0X 1
#else
# define QGLIB_HAVE_CXX0X 0
#endif

#endif
