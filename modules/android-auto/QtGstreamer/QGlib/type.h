/*
    Copyright (C) 2009-2010  George Kiagiadakis <kiagiadakis.george@gmail.com>
    Copyright (C) 2010 Collabora Ltd.
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
#ifndef QGLIB_TYPE_H
#define QGLIB_TYPE_H

#include "global.h"
#include <QtCore/QList>
#include <boost/mpl/if.hpp>

/*
 * This is a re-definition of GType inside the QGlib::Private namespace.
 * It is used in the headers to avoid including <glib-object.h>.
 *
 * According to gtype.h, GType is ulong in C++ for historic reasons,
 * but only if sizeof(size_t) == sizeof(ulong). This template trick
 * here attempts to express the same logic wigh C++ templates.
 */
namespace QGlib {
namespace Private {
    typedef boost::mpl::if_c<
        sizeof(size_t) == sizeof(unsigned long),
        unsigned long,
        QIntegerForSizeof<size_t>::Unsigned
    >::type GType;
} //namespace Private
} //namespace QGlib

namespace QGlib { //closing and re-opening namespace QGlib is required to trick codegen

/*! \headerfile type.h <QGlib/Type>
 * \brief Wrapper class for GType
 *
 * A GType is a numerical value which represents the unique identifier
 * of a registered type. The GType API is the foundation of the GObject system.
 * It provides the facilities for registering and managing all fundamental data types,
 * user-defined object and interface types.
 *
 * As a wrapper, this class provides only methods for querying existing types,
 * not registering new ones. You should never need to use it directly, although
 * it might come in handy in some cases.
 *
 * To retrieve a Type for a certain class, use GetType().
 *
 * \note This class is a thin wrapper around an unsigned long. There is no overhead
 * in copying it around, since it is just an integer.
 */
class QTGLIB_EXPORT Type
{
public:
    enum FundamentalType {
        Invalid = 0,
        None = 1<<2,
        Interface = 2<<2,
        Char = 3<<2,
        Uchar = 4<<2,
        Boolean = 5<<2,
        Int = 6<<2,
        Uint = 7<<2,
        Long = 8<<2,
        Ulong = 9<<2,
        Int64 = 10<<2,
        Uint64 = 11<<2,
        Enum = 12<<2,
        Flags = 13<<2,
        Float = 14<<2,
        Double = 15<<2,
        String = 16<<2,
        Pointer = 17<<2,
        Boxed = 18<<2,
        Param = 19<<2,
        Object = 20<<2
    };

    inline Type() : m_type(0) {}
    inline Type(Private::GType gtype) : m_type(gtype) {}
    inline Type(FundamentalType ftype) : m_type(ftype) {}
    inline Type(const Type & other) : m_type(other.m_type) {}

    inline Type & operator=(Type other);
    inline bool operator==(Type other) const;
    inline operator Private::GType() const { return m_type; }

    static Type fromInstance(void *nativeInstance);
    static Type fromName(const char *name);

    QString name() const;
    Quark nameQuark() const;

    bool isValid() const;
    bool isAbstract() const;
    bool isDerived() const;
    bool isFundamental() const;
    bool isValueType() const;
    bool hasValueTable() const;
    bool isClassed() const;
    bool isInstantiatable() const;
    bool isDerivable() const;
    bool isDeepDerivable() const;
    bool isInterface() const;

    Type fundamental() const;
    Type parent() const;
    uint depth() const;
    Type nextBase(Type rootType) const;
    bool isA(Type is_a_type) const;

    template <typename T>
    inline bool isA() const;

    QList<Type> children() const;
    QList<Type> interfaces() const;
    QList<Type> interfacePrerequisites() const;

    void *quarkData(const Quark & qname) const;
    void setQuarkData(const Quark & qname, void *data);

private:
    Private::GType m_type;
};

inline Type & Type::operator=(Type other)
{
    m_type = other.m_type;
    return *this;
}

inline bool Type::operator==(Type other) const
{
    return m_type == other.m_type;
}

template <class T>
inline Type GetType(); //forward-declaration, defined below

template <typename T>
inline bool Type::isA() const
{
    return isA(GetType<T>());
}

//***************
// -- GetType --
//***************

/* Used to provide the implementation for GetType() */
template <class T>
struct GetTypeImpl
{
//If we have static_assert(), use it to show a more friendly error message to the developer.
//The check is dummy and is expected to evaluate to false. It is just used to trick the
//compiler to delay the evaluation of the expression until the instantiation of this template
//(where T becomes a known type). static_assert(false, "foo"); fails even before instantiation.
#if defined(QGLIB_HAVE_CXX0X_STATIC_ASSERT)
private:
    template <class X> struct FailStruct { static const bool value = false; };
    static_assert(FailStruct<T>::value, "Type T has not been registered with the QGlib type system");
#endif
};

/*! This template function retrieves the QGlib::Type (aka GType) of a given type T.
 * \relates QGlib::Type
 */
template <class T>
inline Type GetType()
{
    return GetTypeImpl<T>();
}

} //namespace QGlib

/* This is to be used to define new Q*_REGISTER_TYPE(T) macros in
 * other bindings libraries that are based on QtGLib.
 */
#define QGLIB_REGISTER_TYPE_WITH_EXPORT_MACRO(T, EXPORT_MACRO) \
    namespace QGlib { \
        template <> \
        struct EXPORT_MACRO GetTypeImpl<T> { operator Type(); }; \
    }

/* This macro is used to register a class with the QtGLib type system. It forward-declares
 * a specialization for struct GetTypeImpl and serves as a keyword for codegen,
 * our code generator, which provides the compiled implementation. This specific macro
 * is meant to be used only inside QtGLib, for other libraries, define a new macro
 * using QGLIB_REGISTER_TYPE_WITH_EXPORT_MACRO.
 * Note: this macro must be used outside of any namespace scope
 */
#define QGLIB_REGISTER_TYPE(T) \
    QGLIB_REGISTER_TYPE_WITH_EXPORT_MACRO(T, QTGLIB_EXPORT)

//**************************
// -- type registrations --
//**************************

#define QGLIB_REGISTER_NATIVE_TYPE(T, GTYPE) \
    namespace QGlib { \
        template <> \
        struct GetTypeImpl<T> { \
            inline operator Type() { return (GTYPE); }; \
        }; \
    }

QGLIB_REGISTER_NATIVE_TYPE(bool, Type::Boolean)
QGLIB_REGISTER_NATIVE_TYPE(char, Type::Char)
QGLIB_REGISTER_NATIVE_TYPE(unsigned char, Type::Uchar)
QGLIB_REGISTER_NATIVE_TYPE(int, Type::Int)
QGLIB_REGISTER_NATIVE_TYPE(unsigned int, Type::Uint)
QGLIB_REGISTER_NATIVE_TYPE(long, Type::Long)
QGLIB_REGISTER_NATIVE_TYPE(unsigned long, Type::Ulong)
QGLIB_REGISTER_NATIVE_TYPE(qint64, Type::Int64)
QGLIB_REGISTER_NATIVE_TYPE(quint64, Type::Uint64)
QGLIB_REGISTER_NATIVE_TYPE(float, Type::Float)
QGLIB_REGISTER_NATIVE_TYPE(double, Type::Double)
QGLIB_REGISTER_NATIVE_TYPE(void*, Type::Pointer)
QGLIB_REGISTER_NATIVE_TYPE(const char*, Type::String)
QGLIB_REGISTER_NATIVE_TYPE(QByteArray, Type::String)
QGLIB_REGISTER_NATIVE_TYPE(QString, Type::String)

//partial specializations for string literals
namespace QGlib {
    template <int N>
    struct GetTypeImpl<const char[N]> { //ISO C++ string literals are const char[]
        inline operator Type() { return Type::String; };
    };
}

namespace QGlib {
    template <int N>
    struct GetTypeImpl<char[N]> { //gcc string literals are char[]
        inline operator Type() { return Type::String; };
    };
}

#undef QGLIB_REGISTER_NATIVE_TYPE

QGLIB_REGISTER_TYPE(QGlib::Type) //codegen: GType=G_TYPE_GTYPE

#endif // QGLIB_TYPE_H
