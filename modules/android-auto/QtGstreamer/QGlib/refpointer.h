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
#ifndef QGLIB_REFPOINTER_H
#define QGLIB_REFPOINTER_H

#include "global.h"
#include "type.h"
#include "wrap.h"
#include <cstddef>
#include <boost/type_traits.hpp>
#include <boost/utility/enable_if.hpp>
#include <QtCore/QHash>

namespace QGlib {

//forward declarations
class Object;
class Interface;


namespace Private {

template <class T, class X>
struct RefPointerEqualityCheck {};

template <class T, class X>
struct RefPointerEqualityCheck<T, RefPointer<X> >
{
    static inline bool check(const RefPointer<T> & self, const RefPointer<X> & other)
    {
        if (self.m_class && other.m_class) {
            return self.m_class->m_object == other.m_class->m_object;
        } else {
            return self.isNull() && other.isNull();
        }
    }
};

template <class T, class X>
struct RefPointerEqualityCheck<T, X*>
{
    static inline bool check(const RefPointer<T> & self, X* const & other)
    {
        return self.m_class ? self.m_class->m_object == other : !other;
    }
};

} //namespace Private


/*! \headerfile refpointer.h <QGlib/RefPointer>
 * \brief Smart pointer class for working with wrapper classes that support reference counting
 *
 * Nearly all GObject and GStreamer classes are designed to work with reference counting.
 * This class provides a smart pointer for instances of those classes which takes care of
 * increasing and decreasing the reference count automatically when a new pointer is
 * constructed and destructed, respectively.
 *
 * All wrapper classes that wrap reference-counted objects must be used with RefPointer.
 * For convenience, this library provides typedefs for all the reference-counted wrappers,
 * which are in the form:
 * \code
 * typedef RefPointer<Foo> FooPtr;
 * \endcode
 * So, for example, if you want to use an instance of a QGst::Element, you should declare
 * a pointer to it like that:
 * \code
 * QGst::ElementPtr element;
 * \endcode
 */
template <class T>
class RefPointer
{
public:
    inline RefPointer();
    inline ~RefPointer();

    /*! \internal */
    explicit inline RefPointer(T *cppClass);

    template <class X>
    inline RefPointer(const RefPointer<X> & other);
    inline RefPointer(const RefPointer<T> & other);

    template <class X>
    inline RefPointer<T> & operator=(const RefPointer<X> & other);
    inline RefPointer<T> & operator=(const RefPointer<T> & other);

    /*! This operator allows you to compare a RefPointer to either
     * another RefPointer or to a pointer of a C object.
     * For example:
     * \code
     * GObject *object = g_object_new(...);
     * QGlib::ObjectPtr objectPtr = QGlib::ObjectPtr::wrap(object);
     * if (objectPtr == object) {
     *    //this code will be executed, the comparison returns true
     * }
     * \endcode
     * \note Although this is a template, you cannot compare to
     * anything else other than a RefPointer or a C instance pointer.
     */
    template <class X>
    bool operator==(const X & other) const;
    template <class X>
    bool operator!=(const X & other) const; ///< \see operator==()

    /*! Sets this RefPointer to NULL and drops the reference
     * to the object that it was previously pointing at. */
    void clear();

    inline bool isNull() const;
    inline bool operator!() const;
    inline T *operator->() const;

    /*! Cast operator that implicitly casts the smart pointer to the pointer type
     * of the underlying C instance. For example, RefPointer<QGst::Element> will cast
     * to GstElement*. This is provided as a helper tool for working with native C
     * methods if needed.
     * \note the returned pointer does not have an increased reference count
     */
    inline operator typename T::CType*() const;

    /*! Makes a RefPointer out of a pointer to a native C instance. If \a increaseRef
     * is specified as false, the reference count is not increased on construction
     * (but it is decreased on destruction!).
     */
    static RefPointer<T> wrap(typename T::CType *nativePtr, bool increaseRef = true);

    /*! Statically casts this RefPointer to a RefPointer of another class. */
    template <class X>
    RefPointer<X> staticCast() const;

    /*! Dynamically casts this RefPointer to a RefPointer of another class.
     * This is equivalent to the built-in dynamic_cast, but it additionally allows you
     * to cast objects to interfaces that they implement and vice-versa by using the
     * Glib type system to determine if the cast is allowed or not. If the cast fails,
     * it returns a null RefPointer.
     *
     * For example, you can do:
     * \code
     * QGst::ElementPtr filesrc = QGst::ElementFactory::make("filesrc");
     * QGst::UriHandlerPtr uriHandler = filesrc.dynamicCast<QGst::UriHandler>();
     * \endcode
     * because a "filesrc" element implements the GstUriHandler interface.
     */
    template <class X>
    RefPointer<X> dynamicCast() const;

private:
    template <class X> friend class RefPointer;
    template <class X, class Y> friend struct Private::RefPointerEqualityCheck;

    template <class X>
    void assign(const RefPointer<X> & other);

    T *m_class;
};

/*! \headerfile refpointer.h <QGlib/RefPointer>
 * \brief Base class for all the reference-counted object wrappers.
 * \internal
 */
class QTGLIB_EXPORT RefCountedObject
{
public:
    virtual ~RefCountedObject() {}

protected:
    template <class T> friend class RefPointer;
    template <class T, class X> friend struct Private::RefPointerEqualityCheck;

    virtual void ref(bool increaseRef) = 0;
    virtual void unref() = 0;

    template <class T>
    inline T* object() const;

    void *m_object;
};

template <class T>
inline T* RefCountedObject::object() const
{
    return static_cast<T* const>(m_object);
}


template <class T>
inline RefPointer<T>::RefPointer()
    : m_class(NULL)
{
}

template <class T>
inline RefPointer<T>::~RefPointer()
{
    clear();
}

template <class T>
inline RefPointer<T>::RefPointer(T *cppClass)
    : m_class(cppClass)
{
    static_cast<RefCountedObject*>(m_class)->ref(true);
}

template <class T>
template <class X>
inline RefPointer<T>::RefPointer(const RefPointer<X> & other)
    : m_class(NULL)
{
    assign(other);
}

template <class T>
inline RefPointer<T>::RefPointer(const RefPointer<T> & other)
    : m_class(NULL)
{
    assign(other);
}

template <class T>
template <class X>
inline RefPointer<T> & RefPointer<T>::operator=(const RefPointer<X> & other)
{
    clear();
    assign(other);
    return *this;
}

template <class T>
inline RefPointer<T> & RefPointer<T>::operator=(const RefPointer<T> & other)
{
    clear();
    assign(other);
    return *this;
}

template <class T>
template <class X>
void RefPointer<T>::assign(const RefPointer<X> & other)
{
    //T should be a base class of X
    QGLIB_STATIC_ASSERT((boost::is_base_of<T, X>::value),
                        "Cannot implicitly cast a RefPointer down the hierarchy");

    if (!other.isNull()) {
        m_class = static_cast<T*>(other.m_class);
        static_cast<RefCountedObject*>(m_class)->ref(true);
    }
}

template <class T>
template <class X>
bool RefPointer<T>::operator==(const X & other) const
{
    return Private::RefPointerEqualityCheck<T, X>::check(*this, other);
}

template <class T>
template <class X>
bool RefPointer<T>::operator!=(const X & other) const
{
    return !Private::RefPointerEqualityCheck<T, X>::check(*this, other);
}

/*! \see RefPointer::operator==()
 * \relates QGlib::RefPointer
 */
template <class T, class X>
//use this function only if X is a pointer and is NOT the same as T::CType*, otherwise
//it is ambiguous with RefPointer::operator==() and the built-in operator== for pointers.
typename boost::enable_if_c<
    boost::is_pointer<X>::value &&
    !boost::is_same<X, typename boost::add_pointer<typename T::CType>::type>::value,
    bool
>::type
operator==(const X & other, const RefPointer<T> & self)
{
    return Private::RefPointerEqualityCheck<T, X>::check(self, other);
}

/*! \see RefPointer::operator==()
 * \relates QGlib::RefPointer
 */
template <class T, class X>
//use this function only if X is a pointer and is NOT the same as T::CType*, otherwise
//it is ambiguous with RefPointer::operator!=() and the built-in operator!= for pointers.
typename boost::enable_if_c<
    boost::is_pointer<X>::value &&
    !boost::is_same<X, typename boost::add_pointer<typename T::CType>::type>::value,
    bool
>::type
operator!=(const X & other, const RefPointer<T> & self)
{
    return !Private::RefPointerEqualityCheck<T, X>::check(self, other);
}

template <class T>
void RefPointer<T>::clear()
{
    if (!isNull()) {
        static_cast<RefCountedObject*>(m_class)->unref(); //this may delete m_class at this point
        m_class = NULL;
    }
}

//static
template <class T>
RefPointer<T> RefPointer<T>::wrap(typename T::CType *nativePtr, bool increaseRef)
{
    RefPointer<T> ptr;
    if (nativePtr != NULL) {
        RefCountedObject *cppObj = WrapImpl<T>::wrap(nativePtr);
        cppObj->ref(increaseRef);
        ptr.m_class = dynamic_cast<T*>(cppObj);
        Q_ASSERT(ptr.m_class);
    }
    return ptr;
}

template <class T>
inline bool RefPointer<T>::isNull() const
{
    return m_class == NULL;
}

template <class T>
inline bool RefPointer<T>::operator!() const
{
    return m_class == NULL;
}

template <class T>
inline T *RefPointer<T>::operator->() const
{
    Q_ASSERT_X(!isNull(), "RefPointer::operator->() const",
               "Attempted to dereference a null pointer");
    return m_class;
}

template <class T>
inline RefPointer<T>::operator typename T::CType*() const
{
    return m_class ? static_cast<RefCountedObject*>(m_class)->object<typename T::CType>() : NULL;
}

template <class T>
template <class X>
RefPointer<X> RefPointer<T>::staticCast() const
{
    RefPointer<X> result;
    if (m_class) {
        static_cast<RefCountedObject*>(m_class)->ref(true);
        result.m_class = static_cast<X*>(m_class);
    }
    return result;
}


namespace Private {

template <typename T, typename X, typename Enable = void>
struct IfaceDynamicCastImpl
{
    static inline X *doCast(typename X::CType *obj)
    {
        Q_UNUSED(obj);
        return NULL;
    }
};

//this version is compiled if X is an interface and T is an object,
//i.e. we are dynamically casting from an object to an interface.
template <typename T, typename X>
struct IfaceDynamicCastImpl<T, X,
        typename boost::enable_if_c<
            //to check if something is an interface, we need to also verify that it does
            //not inherit Object, since derived object classes may also derive from interfaces.
            (boost::is_base_of<Interface, X>::value &&
             !boost::is_base_of<Object, X>::value &&
             boost::is_base_of<Object, T>::value)
        >::type
    >
{
    static inline X *doCast(typename X::CType *obj)
    {
        X *targetClass = NULL;

        //Check that instanceType implements (isA) the interface
        //and if it does, return a wrapper for that interface.
        if (Type::fromInstance(obj).isA(GetType<X>()))
        {
            targetClass = dynamic_cast<X*>(Private::wrapInterface(GetType<X>(), obj));
            Q_ASSERT(targetClass);
        }

        return targetClass;
    }
};

//this version is compiled if T is an interface,
//i.e. we are dynamically casting from an interface to either an object or another interface.
template <typename T, typename X>
struct IfaceDynamicCastImpl<T, X,
        typename boost::enable_if_c<
            //to check if something is an interface, we need to also verify that it does
            //not inherit Object, since derived object classes may also derive from interfaces.
            (boost::is_base_of<Interface, T>::value &&
             !boost::is_base_of<Object, T>::value)
        >::type
    >
{
    static inline X *doCast(typename X::CType *obj)
    {
        //get the instance type and try to create (or rather fetch from the GObject qdata)
        //the C++ wrapper class for this type of object.
        RefCountedObject *cppClass = Private::wrapObject(obj);

        //attempt to cast it to X
        X *targetClass = dynamic_cast<X*>(cppClass);

        if (!targetClass) {
            //Cast failed. This either means that X is something that our instance is not
            //or that X is another interface that is not inherited by the wrapper class
            //for this instance type, but it is possible that our instance actually
            //implements it, so let's check it.
            if (boost::is_base_of<Interface, X>::value &&
                !boost::is_base_of<Object, X>::value &&
                Type::fromInstance(obj).isA(GetType<X>()))
            {
                targetClass = dynamic_cast<X*>(Private::wrapInterface(GetType<X>(), obj));
                Q_ASSERT(targetClass);
            }
        }

        return targetClass;
    }
};

} //namespace Private


template <class T>
template <class X>
RefPointer<X> RefPointer<T>::dynamicCast() const
{
    RefPointer<X> result;
    if (m_class) {
        X *targetClass = dynamic_cast<X*>(m_class);
        if (!targetClass) {
            //in case either X or T is an interface, we need to do some extra checks.
            //this is a template to optimize the compiled code depending on what X and T are.
            typename X::CType *obj = static_cast<RefCountedObject*>(m_class)->object<typename X::CType>();
            targetClass = Private::IfaceDynamicCastImpl<T, X>::doCast(obj);
        }

        if (targetClass) {
            static_cast<RefCountedObject*>(targetClass)->ref(true);
            result.m_class = targetClass;
        }
    }

    return result;
}

// trick GetType to return the same type for GetType<T>() and GetType< RefPointer<T> >()
template <class T>
struct GetTypeImpl< RefPointer<T> >
{
    inline operator Type() { return GetType<T>(); }
};

/*! \relates QGlib::RefPointer */
template <typename T>
inline uint qHash(const RefPointer<T> & ptr)
{
    return qHash(static_cast<typename T::CType*>(ptr));
}

} //namespace QGlib

#endif
