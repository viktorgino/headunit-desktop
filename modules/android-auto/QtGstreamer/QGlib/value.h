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
#ifndef QGLIB_VALUE_H
#define QGLIB_VALUE_H

#include "global.h"
#include "type.h"
#include "refpointer.h"
#include "error.h"
#include <boost/mpl/if.hpp>
#include <boost/type_traits.hpp>
#include <stdexcept>
#include <QtCore/QString>
#include <QtCore/QDebug>
#include <QtCore/QSharedData>

namespace QGlib {

/*! This structure holds the set and get methods that are used internally
 * by Value to handle data of a specific type. If you want to provide
 * support for a custom type, you need to write two such methods, create
 * a new ValueVTable instance that holds pointers to them and register it
 * using Value::registerValueVTable().
 * \sa \ref value_design
 */
struct QTGLIB_EXPORT ValueVTable
{
    typedef void (*SetFunction)(Value & value, const void *data);
    typedef void (*GetFunction)(const Value & value, void *data);

    inline ValueVTable() : set(NULL), get(NULL) {}
    inline ValueVTable(SetFunction s, GetFunction g) : set(s), get(g) {}

    SetFunction set;
    GetFunction get;
};


/*! \headerfile value.h <QGlib/Value>
 * \brief Wrapper class for GValue
 *
 * This class serves as a wrapper for GValue. GValue is a data type that can hold different
 * types of values inside, like a QVariant.
 *
 * A Value object holds a single value of a single type() at a time. To create a new Value,
 * use the static create() method or one of the constructors. To get the held value, use
 * the template get() method or one of the toT() functions (ex. toInt()).
 *
 * To set a value to an invalid Value (on which isValid() returns false), you must first
 * initialize this Value using one of the init() methods (preferably the template one) in order
 * to tell it what kind of value it is going to hold. Once initialized to hold a specific type,
 * you can use the set() method to set a value. To change the type that this value holds, you
 * can call the init() method again at any time. In this case, though, any previously held value
 * will be lost.
 *
 * \note This class is implicitly shared.
 */
class QTGLIB_EXPORT Value
{
public:
    /*! Creates a new invalid Value \sa isValid() */
    Value();

    /*! Creates a new Value that holds a copy of the given \a gvalue */
    explicit Value(const GValue *gvalue);

    /*! Creates a new Value and initializes it to hold values of the given \a type.
     * This is equivalent to:
     * \code
     * Value v;
     * v.init(type);
     * \endcode
     */
    explicit Value(Type type);

    Value(bool val); ///< Creates a new Value of Type::Bool and sets it to hold \a val
    Value(char val); ///< Creates a new Value of Type::Char and sets it to hold \a val
    Value(uchar val); ///< Creates a new Value of Type::Uchar and sets it to hold \a val
    Value(int val); ///< Creates a new Value of Type::Int and sets it to hold \a val
    Value(uint val); ///< Creates a new Value of Type::Uint and sets it to hold \a val
    Value(long val); ///< Creates a new Value of Type::Long and sets it to hold \a val
    Value(ulong val); ///< Creates a new Value of Type::Ulong and sets it to hold \a val
    Value(qint64 val); ///< Creates a new Value of Type::Int64 and sets it to hold \a val
    Value(quint64 val); ///< Creates a new Value of Type::Uint64 and sets it to hold \a val
    Value(float val); ///< Creates a new Value of Type::Float and sets it to hold \a val
    Value(double val); ///< Creates a new Value of Type::Double and sets it to hold \a val
    Value(const char *val); ///< Creates a new Value of Type::String and sets it to hold \a val
    Value(const QByteArray & val); ///< Creates a new Value of Type::String and sets it to hold \a val
    Value(const QString & val); ///< Creates a new Value of Type::String and sets it to hold \a val

    Value(const Value & other);
    Value & operator=(const Value & other);

    virtual ~Value();


    /*! Creates a new Value that is intialized to hold data
     * of type T and sets it to hold \a data.
     * \note T must be a type registered with the QtGLib type system. Attempting
     * to use a type that is not properly registered will fail to compile.
     */
    template <typename T>
    static inline Value create(const T & data);


    /*! Initializes or re-initializes this Value to hold data of the given \a type.
     * If this Value was previously holding any data, this data will be freed.
     */
    void init(Type type);

    /*! Initializes or re-initializes this Value to hold data of type T.
     * If this Value was previously holding any data, this data will be freed.
     * \note T must be a type registered with the QtGLib type system. Attempting
     * to use a type that is not properly registered will fail to compile.
     */
    template <typename T>
    inline void init();


    /*! \returns whether this Value instance is initialized to hold a certain type
     * \sa init() */
    bool isValid() const;

    /*! \returns the type that this Value instance has been initialized to hold */
    Type type() const;

    /*! \returns whether it is possible to transform this Value to a Value of another type */
    bool canTransformTo(Type type) const;

    /*! Transforms the current value into a value of the specified \a type, if possible.
     * Possible transformations are, for example, int to float, int to string, etc...
     * The original Value remains unaffected and the transformed Value is returned.
    */
    Value transformTo(Type type) const;

    /*! Clears the current value in this Value instance and resets it to the
     * default value (as if the Value had just been initialized). */
    void clear();


    /*! Retrieves the value stored in this Value instance, as the specified type T.
     * The bindings take care of calling the appropriate g_value_get_* method depending
     * on the type T. Note though that this is only meant to be used with the types of
     * the bindings, not their C types. This means that if for example the Value has
     * been initialized to hold a GstObject pointer, you can use:
     * \code
     * QGst::ObjectPtr object = value.get<QGst::ObjectPtr>();
     * \endcode
     * but \em not this:
     * \code
     * GstObject *object = value.get<GstObject*>(); //will cause compilation error
     * \endcode
     *
     * If the underlying stored value is not of the type T, this method will attempt
     * to convert it to type T. If this is not possible, a default constructed value
     * will be returned.
     *
     * If \a ok has been specified, it is set to true if the value was retrieved
     * successfully or false if there was an error (probably conversion error) and
     * a default constructed value has been returned.
     */
    template <typename T> T get(bool *ok = NULL) const;

    /*! Sets this Value instance to hold the specified \a data.
     * As with get(), the bindings take care of calling the appropriate g_value_set_*
     * method depending on the type T, but note that this is only meant to be used
     * with the types of the bindings.
     *
     * If this Value instance has been initialized to hold a different type of data
     * than T, a conversion to the correct type() will be attempted. If the conversion
     * fails, the Value will remain unaffected and a warning will be printed.
     * \sa get()
     */
    template <typename T> void set(const T & data);


    /*! Returns the held value as a bool. Equivalent to get<bool>(ok); \sa get() */
    inline bool toBool(bool *ok = NULL) const { return get<bool>(ok); }

    /*! Returns the held value as a char. Equivalent to get<char>(ok); \sa get() */
    inline char toChar(bool *ok = NULL) const { return get<char>(ok); }

    /*! Returns the held value as a uchar. Equivalent to get<uchar>(ok); \sa get() */
    inline uchar toUChar(bool *ok = NULL) const { return get<uchar>(ok); }

    /*! Returns the held value as an int. Equivalent to get<int>(ok); \sa get() */
    inline int toInt(bool *ok = NULL) const { return get<int>(ok); }

    /*! Returns the held value as a uint. Equivalent to get<uint>(ok); \sa get() */
    inline uint toUInt(bool *ok = NULL) const { return get<uint>(ok); }

    /*! Returns the held value as a long. Equivalent to get<long>(ok); \sa get() */
    inline long toLong(bool *ok = NULL) const { return get<long>(ok); }

    /*! Returns the held value as a ulong. Equivalent to get<ulong>(ok); \sa get() */
    inline ulong toULong(bool *ok = NULL) const { return get<ulong>(ok); }

    /*! Returns the held value as a qint64. Equivalent to get<qint64>(ok); \sa get() */
    inline qint64 toInt64(bool *ok = NULL) const { return get<qint64>(ok); }

    /*! Returns the held value as a quint64. Equivalent to get<quint64>(ok); \sa get() */
    inline quint64 toUInt64(bool *ok = NULL) const { return get<quint64>(ok); }

    /*! Returns the held value as a QByteAray. Equivalent to get<QByteArray>(ok); \sa get() */
    inline QByteArray toByteArray(bool *ok = NULL) const { return get<QByteArray>(ok); }

    /*! Returns the held value as a QString. Equivalent to get<QString>(ok); \sa get() */
    inline QString toString(bool *ok = NULL) const { return get<QString>(ok); }

    /*! Returns the held value as a QGlib::Error. Equivalent to get<QGlib::Error>(ok); \sa get() */
    inline Error toError(bool *ok = NULL) const { return get<Error>(ok); }

    /*! This is a cast operator that gives access to the underlying GValue instance.
     * It is provided for convenience, to be able to pass this Value as argument to C
     * functions that expect a GValue pointer. Use this feature with care. Do not store
     * the pointer, as it may go away at any time. This Value instance keeps control of
     * this pointer. If you need to store it, use g_value_copy() to copy it first.
     */
    operator GValue*();
    operator const GValue*() const; ///< \overload


    /*! Registers the given ValueVTable \a vtable for the given \a type.
     * This is provided to let you add support for a custom type, if necessary.
     * You should normally not need to do that, as most types are handled
     * by the handlers of their parent types.
     * \sa \ref value_design
     */
    static void registerValueVTable(Type type, const ValueVTable & vtable);

private:
    template <typename T>
    friend struct ValueImpl;

    /*! Retrieves the data from this Value and places it into the memory position
     * pointed to by \a data. \a dataType indicates the actual data type of \a data
     * and is used, among others, to cast \a data back to the actual C++ type that
     * it points to and assign it.
     * \note This method should only be accessed from ValueImpl.
     * \sa \ref value_design
     */
    void getData(Type dataType, void *data) const;

    /*! Sets the data of this Value to be the data pointed to by \a data.
     * \a dataType indicates the actual data type of \a data and is used,
     * among others, to cast \a data back to the actual C++ type that
     * it points to and retrieve its value.
     * \note This method should only be accessed from ValueImpl.
     * \sa \ref value_design
     */
    void setData(Type dataType, const void *data);

    struct Data;
    QSharedDataPointer<Data> d;
};


/*! This struct provides the implementation for the Value::get() and Value::set() methods.
 * If you want to provide support for a custom type, you may want to provide a template
 * specialization of this class to handle your type in a different way than the default
 * implementation. You should normally not need to be concerned at all with this.
 * \note this struct is declared as friend in Value and as a result it has access to
 * Value::setData() and Value::getData()
 * \sa \ref value_design
 */
template <typename T>
struct ValueImpl
{
    static inline T get(const Value & value);
    static inline void set(Value & value, const T & data);
};

// -- template implementations --

//static
template <typename T>
inline Value Value::create(const T & data)
{
    Value v;
    v.init<T>();
    v.set(data);
    return v;
}

template <typename T>
inline void Value::init()
{
    init(GetType<T>());
}

template <typename T>
T Value::get(bool *ok) const
{
    if (ok) {
        *ok = true;
    }

    try {
        return ValueImpl<T>::get(*this);
    } catch (const std::exception &) {
        if (ok) {
            *ok = false;
        }
        return T();
    }
}

template <typename T>
void Value::set(const T & data)
{
    try {
        ValueImpl<T>::set(*this, data);
    } catch (const std::exception & e) {
        qWarning() << "QGlib::Value::set:" << e.what();
    }
}

// -- default ValueImpl implementation --

template <typename T>
inline T ValueImpl<T>::get(const Value & value)
{
    //Use int for enums, T for everything else
    typename boost::mpl::if_<
        boost::is_enum<T>,
        int, T
    >::type result;

    value.getData(GetType<T>(), &result);
    return static_cast<T>(result);
}

template <typename T>
inline void ValueImpl<T>::set(Value & value, const T & data)
{
    //Use const int for enums, const T for everything else
    typename boost::mpl::if_<
        boost::is_enum<T>,
        const int, const T &
    >::type dataRef = data;

    value.setData(GetType<T>(), &dataRef);
}

// -- ValueImpl specialization for QFlags --

template <class T>
struct ValueImpl< QFlags<T> >
{
    static inline QFlags<T> get(const Value & value)
    {
        uint flags;
        value.getData(GetType< QFlags<T> >(), &flags);
        return QFlags<T>(QFlag(flags));
    }

    static inline void set(Value & value, const QFlags<T> & data)
    {
        uint flags = data;
        value.setData(GetType< QFlags<T> >(), &flags);
    }
};

// -- ValueImpl specialization for RefPointer --

template <class T>
struct ValueImpl< RefPointer<T> >
{
    static inline RefPointer<T> get(const Value & value)
    {
        typename T::CType *gobj;
        value.getData(GetType<T>(), &gobj);
        return RefPointer<T>::wrap(gobj);
    }

    static inline void set(Value & value, const RefPointer<T> & data)
    {
        typename T::CType *gobj = static_cast<typename T::CType*>(data);
        value.setData(GetType<T>(), &gobj);
    }
};

// -- ValueImpl specializations for string literals --

template <int N>
struct ValueImpl<const char[N]> //ISO C++ string literals are const char[]
{
    //No get method, obviously.

    static inline void set(Value & value, const char (&data)[N])
    {
        QByteArray str = QByteArray::fromRawData(data, N);
        value.setData(Type::String, &str);
    }
};

template <int N>
struct ValueImpl<char[N]> //gcc string literals are char[]
{
    //No get method, obviously.

    static inline void set(Value & value, const char (&data)[N])
    {
        QByteArray str = QByteArray::fromRawData(data, N);
        value.setData(Type::String, &str);
    }
};

// -- ValueImpl specialization for const char* --

template <>
struct ValueImpl<const char*>
{
    //No get method, obviously.

    static inline void set(Value & value, const char *data)
    {
        QByteArray str = QByteArray::fromRawData(data, qstrlen(data));
        value.setData(Type::String, &str);
    }
};

// -- ValueImpl specialization for QString --

template <>
struct ValueImpl<QString>
{
    static inline QString get(const Value & value)
    {
        QByteArray str;
        value.getData(Type::String, &str);
        return QString::fromUtf8(str);
    }

    static inline void set(Value & value, const QString & data)
    {
        QByteArray str = data.toUtf8();
        value.setData(Type::String, &str);
    }
};

// -- ValueImpl specialization for Value --

template <>
struct ValueImpl<Value>
{
    static inline Value get(const Value & value)
    {
        return value;
    }

    static inline void set(Value & value, const Value & data)
    {
        value = data;
    }
};

// -- ValueImpl specialization for Error --

template <>
struct ValueImpl<Error>
{
    static inline Error get(const Value & value)
    {
        GError *error = 0;
        value.getData(GetType<Error>(), &error);
        return Error::copy(error);
    }

    static inline void set(Value & value, const Error & data)
    {
        value.setData(GetType<Error>(), static_cast<const GError *>(data));
    }
};

// -- Exceptions thrown from getData/setData --

namespace Private {

class QTGLIB_EXPORT InvalidValueException : public std::logic_error
{
public:
    inline InvalidValueException()
        : std::logic_error("This Value instance has not been initialized") {}
};

class QTGLIB_EXPORT InvalidTypeException : public std::logic_error
{
public:
    inline InvalidTypeException(const std::string & dataType, const std::string & valueType)
        : std::logic_error("Unable to handle value type \"" + dataType +
                           "\". This Value instance has been initialized to hold values of type \""
                           + valueType + "\" and no conversion is possible") {}
};

class QTGLIB_EXPORT UnregisteredTypeException : public std::logic_error
{
public:
    inline UnregisteredTypeException(const std::string & typeName)
        : std::logic_error("Unable to handle unregistered type \"" + typeName + "\"") {}
};

class QTGLIB_EXPORT TransformationFailedException : public std::runtime_error
{
public:
    inline TransformationFailedException(const std::string & srcTypeName,
                                         const std::string & destTypeName)
        : std::runtime_error("Failed to transform value from type \""
                             + srcTypeName + "\" to type \"" + destTypeName + "\"") {}
};

} //namespace Private

// -- QDebug operator --

/*! \relates QGlib::Value */
QTGLIB_EXPORT QDebug operator<<(QDebug debug, const Value & value);

} //namespace QGlib

QGLIB_REGISTER_TYPE(QGlib::Value)

#endif
