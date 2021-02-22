/*
    Copyright (C) 2010 George Kiagiadakis <kiagiadakis.george@gmail.com>
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
#ifndef QGLIB_CONNECT_H
#define QGLIB_CONNECT_H

#include "global.h"
#include "quark.h"
#include <QtCore/QObject>
#include <QtCore/QSharedPointer>
#include <QtCore/QFlags>
#include <QtCore/QHash>
#include <boost/type_traits.hpp>
#include <boost/utility/enable_if.hpp>

namespace QGlib {

/*! These flags define options that can be
 * passed to connect() to modify its behaviour.
 */
enum ConnectFlag { //codegen: skip=true
    /*! If ConnectAfter is specified, the slot passed to connect()
     * will be invoked after the default signal handler of this signal
     * has been called. See the Glib signals documentation for more
     * details on this parameter.
     */
    ConnectAfter = 1,
    /*! If PassSender is specified, the slot passed to connect()
     * will receive as the first argument a pointer to the sender
     * of the signal. Thus, your slot should be defined like this:
     * \code
     * void mySlot(const QGlib::ObjectPtr & sender, const Foo & firstArgument, ...);
     * \endcode
     */
    PassSender = 2
};
Q_DECLARE_FLAGS(ConnectFlags, ConnectFlag);
Q_DECLARE_OPERATORS_FOR_FLAGS(ConnectFlags)

#if defined(DOXYGEN_RUN)

/*! Connects \a detailedSignal of \a instance to the specified \a slot of \a receiver.
 *
 * This method provides a way of connecting GObject signals to C++ slots, in a Qt-like fashion.
 *
 * \a instance needs to be a pointer to a GLib instantiatable type, such as a GObject. You can
 * pass a RefPointer as an argument here without any problems; it will automatically cast to void*.
 *
 * \a detailedSignal should be the name of the signal that you want to conenct to - with an
 * optional detail, if the signal supports details. The signal must \em not use the Qt SIGNAL()
 * macro; it should use the signal canonical form specified in GLib, i.e. it should be a string
 * containing only lowercase letters and dashes. For example, "my-signal" is a valid signal name.
 * The detail may be specified by appending "::" to the signal name and the detail after that.
 * For example, "my-signal::detail".
 *
 * \a receiver should be the instance of the class on which \a slot will be invoked. Currently,
 * the \a receiver needs to inherit QObject. If you use another class that doesn't inherit
 * QObject, a compilation error will occur.
 *
 * \a slot should be a C++ member function pointer that points to the method that you want
 * to be invoked on the \a receiver once the signal is emitted. Note that it must \em not use
 * the Qt SLOT() macro.
 *
 * Special care must be taken with the arguments that \a slot takes. Consider the following
 * example. The C API documentation mentions a signal that takes these parameters:
 * \code
 * void user_function (GObject *object, GParamSpec *pspec, gpointer user_data);
 * \endcode
 * The first argument is always a pointer to the sender object, the second argument is
 * the real signal argument and the third argument is a user defined pointer that is usually
 * used in C as a substitute for the \a receiver. In general, the first argument is always
 * the sender, the 2nd to Nth-1 arguments are the real signal arguments and the Nth argument
 * is a user defined pointer. Now in C++, the following rules apply:
 * \li You must use the C++ equivalent types of the arguments that the signal sends.
 * So, in the above example, GParamSpec* is translated to QGlib::ParamSpecPtr.
 * \li You must not have the user_data argument at the end.
 * \li You must not have the sender argument at the beginning, unless QGlib::PassSender
 * is used in \a flags.
 * \li You may use const references in the arguments to avoid unecessary copying of objects,
 * as the C++ marshaller that will call your slot will always hold a copy of the converted
 * C++ objects.
 *
 * So, the C++ signature should eventually be:
 * \code
 * void MyClass::mySlot(const QGlib::ParamSpecPtr & pspec);
 * ...
 * //Connected with:
 * QGlib::connect(element, "notify::name", myClassInstance, &MyClass::mySlot);
 * \endcode
 * \em or
 * \code
 * void MyClass::mySlot(const QGlib::ObjectPtr & sender, const QGlib::ParamSpecPtr & pspec);
 * ...
 * //Connected with:
 * QGlib::connect(element, "notify::name", myClassInstance, &MyClass::mySlot, QGlib::PassSender);
 * \endcode
 *
 * Internally, the signal provides GValues that are converted to the C++ types that your
 * \a slot specifies using Value::get(), so all rules that apply there also apply to the
 * \a slot arguments. That means that you may actually use different types of arguments
 * than the ones that the documentation specifies, given that Value is able to transform
 * them (see Value::canTransformTo) to the type that you have requested. Obviously, if you
 * specify non-compatible arguments on the \a slot, the conversion will fail, and in this case
 * a warning is printed at the time of the signal emission and the \a slot is not invoked.
 *
 * \note
 * \li Include <QGlib/Connect> to use this function
 * \li Unlike Qt, in GObject some signals are able to return values back to the sender.
 * In this case, your \a slot should specify a compatible return type instead of void.
 * \li This method makes use of C++0x features (namely, variadic templates and rvalue
 * references). If your compiler does not support them, a hacky implementation using boost's
 * preprocessor, function and bind libraries will be compiled instead. That version has a
 * limit of 9 slot arguments.
 * \li This function is thread-safe.
 *
 * \returns whether the connection was successfully made or not
 * \sa disconnect(), ConnectFlag, \ref connect_design
 */
template <typename T, typename R, typename... Args>
bool connect(void *instance, const char *detailedSignal,
             T *receiver, R (T::*slot)(Args...), ConnectFlags flags = 0);

//Fake disconnect() declaration.
//Doxygen should document a version with optional arguments. In reality we have to use
//two versions to avoid having to type the template parameters in case the user wants
//to use NULL for the receiver and slot arguments. Also, a version that takes void*
//for everything is not possible since member function pointers do not cast to void*.

/*! Disconnects \a detailedSignal of \a instance from the \a slot of \a receiver.
 *
 * A signal-slot connection typically is broken when either of the objects involved
 * are destroyed. However, there are cases in which you might want to break that
 * connection manually using this method.
 *
 * disconnect() can be used in the following ways:
 * \li Disconnect everything connected to an object's signals:
 * \code
 * disconnect(myGObject);
 * \endcode
 * \li Disconnect everything connected to a specific signal:
 * \code
 * disconnect(myGObject, "some-signal");
 * \endcode
 * \li Disconnect a specific receiver:
 * \code
 * disconnect(myGObject, 0, myReceiver);
 * \endcode
 * \li Disconnect a specific slot (note that the receiver must also be specified):
 * \code
 * disconnect(myGObject, 0, myReceiver, &MyReceiver::mySlot);
 * \endcode
 *
 * 0 can be used as a wildcard, meaning "any signal", "any receiver" or "any slot".
 * \a instance can never be 0.
 *
 * If \a detailedSignal is 0, it disconnects \a receiver and \a slot from any signal.
 * Otherwise, only \a detailedSignal is disconnected. A signal detail may or may not
 * be specified. If it is not specified, it acts as a wildcard for all details, meaning
 * it disconnects all the connections that have been made with a specific detail.
 *
 * If \a receiver is 0, it disconnects anything connected to \a detailedSignal. If not,
 * only slots in \a receiver are disconnected.
 *
 * If \a slot is 0, it disconnects anything that is connected to \a receiver. If not,
 * only the specified \a slot will be disconnected. Note that \a slot must be 0 if
 * \a receiver is left out.
 *
 * \note
 * \li Include <QGlib/Connect> to use this function
 * \li This function is thread-safe.
 *
 * \returns true if the connection was successfully broken, or false otherwise
 * \sa connect()
 */
template <typename T, typename R, typename... Args>
bool disconnect(void *instance, const char *detailedSignal = 0,
                T *receiver = 0, R (T::*slot)(Args...) = 0);

#else //DOXYGEN_RUN

namespace Private {

//BEGIN ******** ClosureDataBase ********

class QTGLIB_EXPORT ClosureDataBase
{
public:
    inline virtual ~ClosureDataBase() {}
    virtual void marshaller(Value &, const QList<Value> &) = 0;

    bool passSender; //whether to pass the sender instance as the first slot argument

protected:
    inline ClosureDataBase(bool passSender)
        : passSender(passSender) {}
};

//END ******** ClosureDataBase ********


/* This interface specifies the methods that will be used to connect/disconnect a
 * signal receiver to/from a slot that should be called when the receiver is destroyed.
 * This notification is used to disconnect the signal automatically.
 */
class QTGLIB_EXPORT DestroyNotifierIface
{
public:
    virtual ~DestroyNotifierIface() {}
    virtual bool connect(void *receiver, QObject *notificationReceiver, const char *slot) = 0;
    virtual bool disconnect(void *receiver, QObject *notificationReceiver) = 0;
};

typedef QSharedPointer<DestroyNotifierIface> DestroyNotifierIfacePtr;

/* This is DestroyNotifierIface that works for signal receivers that inherit QObject. */
class QTGLIB_EXPORT QObjectDestroyNotifier : public DestroyNotifierIface
{
public:
    static DestroyNotifierIfacePtr instance();

    virtual bool connect(void *receiver, QObject *notificationReceiver, const char *slot);
    virtual bool disconnect(void *receiver, QObject *notificationReceiver);
};

/* This is provided for future expansion.
 * It should implement operator DestroyNotifierIfacePtr() and return
 * the appropriate DestroyNotifierIface for the given type T
 * (i.e. the signal receiver is of type T)
 */
template <typename T, typename Enable = void>
struct GetDestroyNotifier
{
};

/* Partial specialization for QObjects (T inherits QObject) */
template <typename T>
struct GetDestroyNotifier<T, typename boost::enable_if< boost::is_base_of<QObject, T> >::type>
{
    inline operator DestroyNotifierIfacePtr() { return QObjectDestroyNotifier::instance(); }
};


/* This method is used internally from QGlib::connect(). */
QTGLIB_EXPORT ulong connect(void *instance, const char *signal, Quark detail,
                            void *receiver, const DestroyNotifierIfacePtr & notifier,
                            uint slotHash, ClosureDataBase *closureData, ConnectFlags flags);

/* This method is used internally from QGlib::disconnect(). */
QTGLIB_EXPORT bool disconnect(void *instance, const char *signal, Quark detail,
                              void *receiver, uint slotHash, ulong handlerId);


/* This is a helper that returns a hash value for a member function pointer.
 * Because of the nature of member function pointers, it is not possible to cast
 * them to void* or any integral type and as a result we need to create a hash value
 * of their data to be able to store them in the connections store. This value is
 * only used for disconnection, so storing the real pointer is not necessary.
 */
template <typename T>
inline typename boost::enable_if< boost::is_member_function_pointer<T>, uint >::type
hashMfp(const T & mfp)
{
    const char *data = reinterpret_cast<const char*>(&mfp);
    return qHash(QByteArray::fromRawData(data, sizeof(T)));
}

template <typename T>
inline typename boost::enable_if< boost::is_integral<T>, uint >::type
hashMfp(const T & mfp)
{
    Q_ASSERT(mfp == 0);
    return 0;
}

} //namespace Private


//The real QGlib::disconnect

inline bool disconnect(void *instance, const char *detailedSignal = 0, void *receiver = 0)
{
    return Private::disconnect(instance, detailedSignal, Quark(), receiver, 0, 0);
}

template <typename T>
inline bool disconnect(void *instance, const char *detailedSignal, void *receiver, T slot)
{
    return Private::disconnect(instance, detailedSignal, Quark(), receiver, Private::hashMfp(slot), 0);
}

#endif //DOXYGEN_RUN

} //namespace QGlib

#if !QGLIB_HAVE_CXX0X
//boost::bind restricts us to 9 arguments. if you need more,
//consider using a modern compiler with variadic template support ;)
# define QGLIB_CONNECT_MAX_ARGS 9
#endif

#define IN_QGLIB_CONNECT_H
# include "connectimpl.h"
#undef IN_QGLIB_CONNECT_H

#if defined(QGLIB_CONNECT_MAX_ARGS)
# undef QGLIB_CONNECT_MAX_ARGS
#endif

#endif //QGLIB_CONNECT_H
