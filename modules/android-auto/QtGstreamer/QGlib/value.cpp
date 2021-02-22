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
#include "value.h"
#include "string.h"
#include <cstring>
#include <boost/type_traits.hpp>
#include <glib-object.h>
#include <QtCore/QDebug>
#include <QtCore/QReadWriteLock>

namespace QGlib {
namespace Private {

class Dispatcher
{
public:
    Dispatcher();

    ValueVTable getVTable(Type t) const;
    void setVTable(Type t, const ValueVTable & vtable);

private:
    mutable QReadWriteLock lock;
    QHash<Type, ValueVTable> dispatchTable;
};

Dispatcher::Dispatcher()
{
#define DECLARE_VTABLE(T, NICK, GTYPE) \
    struct ValueVTable_##NICK \
    { \
        static void get(const Value & value, void *data) \
        { \
            *reinterpret_cast<T*>(data) = g_value_get_##NICK(value); \
        }; \
        \
        static void set(Value & value, const void *data) \
        { \
            g_value_set_##NICK(value, *reinterpret_cast<T const *>(data)); \
        }; \
    }; \
    setVTable(GTYPE, ValueVTable(ValueVTable_##NICK::set, ValueVTable_##NICK::get));

    DECLARE_VTABLE(char, char, Type::Char)
    DECLARE_VTABLE(unsigned char, uchar, Type::Uchar)
    DECLARE_VTABLE(bool, boolean, Type::Boolean)
    DECLARE_VTABLE(int, int, Type::Int)
    DECLARE_VTABLE(unsigned int, uint, Type::Uint)
    DECLARE_VTABLE(long, long, Type::Long)
    DECLARE_VTABLE(unsigned long, ulong, Type::Ulong)
    DECLARE_VTABLE(qint64, int64, Type::Int64)
    DECLARE_VTABLE(quint64, uint64, Type::Uint64)
    DECLARE_VTABLE(int, enum, Type::Enum);
    DECLARE_VTABLE(uint, flags, Type::Flags)
    DECLARE_VTABLE(float, float, Type::Float)
    DECLARE_VTABLE(double, double, Type::Double)
    DECLARE_VTABLE(QByteArray, string, Type::String)
    DECLARE_VTABLE(void*, pointer, Type::Pointer)
    DECLARE_VTABLE(void*, boxed, Type::Boxed)
    DECLARE_VTABLE(GParamSpec*, param, Type::Param)
    DECLARE_VTABLE(void*, object, Type::Object)
    DECLARE_VTABLE(QGlib::Type, gtype, GetType<QGlib::Type>())

#undef DECLARE_VTABLE
}

ValueVTable Dispatcher::getVTable(Type t) const
{
    //if the type is an interface, try to find its
    //instantiatable prerequisite and get the vtable
    //of this instantiatable type instead.
    if (t.isInterface()) {
        QList<Type> prerequisites = t.interfacePrerequisites();
        Q_FOREACH(Type prereq, prerequisites) {
            if (prereq.isInstantiatable()) {
                t = prereq;
            }
        }

        //Check if the prerequisite was found and
        //bail out if not, since such interfaces
        //are not compatible with GValue.
        if (!t.isInstantiatable()) {
            return ValueVTable();
        }
    }

    QReadLocker l(&lock);

    if (dispatchTable.contains(t)) {
        return dispatchTable[t];
    }

    while (t.isDerived()) {
        t = t.parent();
        if (dispatchTable.contains(t)) {
            return dispatchTable[t];
        }
    }

    return ValueVTable();
}

void Dispatcher::setVTable(Type t, const ValueVTable & vtable)
{
    QWriteLocker l(&lock);
    dispatchTable[t] = vtable;
}

} //namespace Private

Q_GLOBAL_STATIC(Private::Dispatcher, s_dispatcher);

#ifndef DOXYGEN_RUN

// -- Value::Data --

struct QTGLIB_NO_EXPORT Value::Data : public QSharedData
{
    Data();
    Data(const Data & other);
    ~Data();

    inline Type type() const { return G_VALUE_TYPE(&m_value); }
    inline GValue *value() { return &m_value; }
    inline const GValue *value() const { return &m_value; }

    GValue m_value;
};

Value::Data::Data()
    : QSharedData()
{
    std::memset(&m_value, 0, sizeof(GValue));
}

Value::Data::Data(const Value::Data & other)
    : QSharedData(other)
{
    std::memset(&m_value, 0, sizeof(GValue));

    if (other.type() != Type::Invalid) {
        g_value_init(value(), other.type());
        g_value_copy(other.value(), value());
    }
}

Value::Data::~Data()
{
    if (type() != Type::Invalid) {
        g_value_unset(value());
    }
}

#endif //DOXYGEN_RUN

// -- Value --

Value::Value()
    : d(new Data)
{
}

Value::Value(const GValue *gvalue)
    : d(new Data)
{
    if (gvalue && G_IS_VALUE(gvalue)) {
        init(G_VALUE_TYPE(gvalue));
        g_value_copy(gvalue, d->value());
    }
}

Value::Value(Type type)
    : d(new Data)
{
    init(type);
}

#define VALUE_CONSTRUCTOR(T) \
    Value::Value(T val) \
        : d(new Data) \
    { \
        init< \
            boost::remove_const< \
                boost::remove_reference<T>::type \
            >::type \
        >(); \
        set(val); \
    }

VALUE_CONSTRUCTOR(bool)
VALUE_CONSTRUCTOR(char)
VALUE_CONSTRUCTOR(uchar)
VALUE_CONSTRUCTOR(int)
VALUE_CONSTRUCTOR(uint)
VALUE_CONSTRUCTOR(long)
VALUE_CONSTRUCTOR(ulong)
VALUE_CONSTRUCTOR(qint64)
VALUE_CONSTRUCTOR(quint64)
VALUE_CONSTRUCTOR(float)
VALUE_CONSTRUCTOR(double)
VALUE_CONSTRUCTOR(const char *)
VALUE_CONSTRUCTOR(const QByteArray &)
VALUE_CONSTRUCTOR(const QString &)

#undef VALUE_CONSTRUCTOR

Value::Value(const Value & other)
    : d(other.d)
{
}

Value & Value::operator=(const Value & other)
{
    d = other.d;
    return *this;
}

Value::~Value()
{
}

void Value::init(Type type)
{
    if (isValid()) {
        g_value_unset(d->value());
    }
    g_value_init(d->value(), type);
}

bool Value::isValid() const
{
    return d->type() != Type::Invalid;
}

Type Value::type() const
{
    return d->type();
}

bool Value::canTransformTo(Type t) const
{
    return isValid() ? g_value_type_transformable(type(), t) : false;
}

Value Value::transformTo(Type t) const
{
    Value dest;
    dest.init(t);
    if (isValid()) {
        g_value_transform(d->value(), dest.d->value());
    }
    return dest;
}

void Value::clear()
{
    if (isValid()) {
        g_value_reset(d->value());
    }
}

Value::operator GValue* ()
{
    return d->value();
}

Value::operator const GValue * () const
{
    return d->value();
}

//static
void Value::registerValueVTable(Type type, const ValueVTable & vtable)
{
    s_dispatcher()->setVTable(type, vtable);
}

static inline std::string toStdStringHelper(const QString & str)
{
#ifndef QT_NO_STL
    return str.toStdString();
#else
    const QByteArray asc = str.toAscii();
    return std::string(asc.constData(), asc.length());
#endif
}

void Value::getData(Type dataType, void *data) const
{
    if (!isValid()) {
        throw Private::InvalidValueException();
    } else if (g_value_type_compatible(type(), dataType)) {
        ValueVTable vtable = s_dispatcher()->getVTable(dataType);
        if (vtable.get != NULL) {
            vtable.get(*this, data);
        } else {
            throw Private::UnregisteredTypeException(toStdStringHelper(dataType.name()));
        }
    } else if (dataType.isValueType() && g_value_type_transformable(type(), dataType)) {
        Value v;
        v.init(dataType);

        if (!g_value_transform(d->value(), v.d->value())) {
            throw Private::TransformationFailedException(toStdStringHelper(type().name()),
                                                         toStdStringHelper(dataType.name()));
        }

        v.getData(dataType, data);
    } else {
        throw Private::InvalidTypeException(toStdStringHelper(dataType.name()),
                                            toStdStringHelper(type().name()));
    }
}

void Value::setData(Type dataType, const void *data)
{
    if (!isValid()) {
        throw Private::InvalidValueException();
    } else if (g_value_type_compatible(dataType, type())) {
        ValueVTable vtable = s_dispatcher()->getVTable(dataType);
        if (vtable.set != NULL) {
            vtable.set(*this, data);
        } else {
            throw Private::UnregisteredTypeException(toStdStringHelper(dataType.name()));
        }
    } else if (dataType.isValueType() && g_value_type_transformable(dataType, type())) {
        Value v;
        v.init(dataType);
        v.setData(dataType, data);

        if (!g_value_transform(v.d->value(), d->value())) {
            throw Private::TransformationFailedException(toStdStringHelper(dataType.name()),
                                                         toStdStringHelper(type().name()));
        }
    } else {
        throw Private::InvalidTypeException(toStdStringHelper(dataType.name()),
                                            toStdStringHelper(type().name()));
    }
}


QDebug operator<<(QDebug debug, const Value & value)
{
    debug.nospace() << "QGlib::Value";
    if(!value.isValid()) {
        debug << "(<invalid>)";
        return debug.space();
    } else {
        QString str = value.toString();
        if (str.isEmpty()) {
            if (g_value_fits_pointer(value)) {
                quintptr ptr = reinterpret_cast<quintptr>(g_value_peek_pointer(value));
                str = QString(QLatin1String("0x%1")).arg(ptr, sizeof(quintptr)*2,
                                                         16, QLatin1Char('0'));
            } else {
                str = QLatin1String("<unknown value>");
            }
        }

        debug << "(" << value.type().name() << ", " << str << ")";
        return debug.space();
    }
}

} //namespace QGlib
