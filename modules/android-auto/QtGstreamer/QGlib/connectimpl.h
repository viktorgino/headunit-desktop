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
#if !defined(BOOST_PP_IS_ITERATING) || !BOOST_PP_IS_ITERATING

# ifndef IN_QGLIB_CONNECT_H
#  error "This file must not be included directly"
# endif

# include "value.h"
# include "refpointer.h"
# include <QtCore/QList>
# include <stdexcept>
# include <boost/type_traits.hpp>


namespace QGlib {
namespace Private {

//BEGIN ******** CppClosure prototype ********

template <typename Function, typename Signature>
struct CppClosure {};

//END ******** CppClosure prototype ********
//BEGIN ******** invoker ********

template <typename Function, typename R>
struct invoker
{
    static inline void invoke(const Function & f, Value & result) { ValueImpl<R>::set(result, f()); }
};

template <typename Function>
struct invoker<Function, void>
{
    static inline void invoke(const Function & f, Value &) { f(); }
};

//END ******** invoker ********

} //namespace Private
} //namespace QGlib


# if QGLIB_HAVE_CXX0X

namespace QGlib {
namespace Private {

//BEGIN ******** MemberFunction ********

template <typename T, typename R, typename... Args>
class MemberFunction
{
public:
    inline MemberFunction(R (T::*fn)(Args...), T *obj)
        : m_function(fn), m_object(obj) {}

    inline R operator()(Args&&... args) const
    {
        return (m_object->*m_function)(std::forward<Args>(args)...);
    }

private:
    R (T::*m_function)(Args...);
    T *m_object;
};

template <typename T, typename R, typename... Args>
MemberFunction<T, R, Args...> mem_fn(R (T::*fn)(Args...), T *obj)
{
    return MemberFunction<T, R, Args...>(fn, obj);
}

//END ******** MemberFunction ********
//BEGIN ******** BoundArgumentFunction ********

template <typename ParentFunction, typename R, typename Arg1, typename... Args>
class BoundArgumentFunction
{
public:
    inline BoundArgumentFunction(ParentFunction && fn, Arg1 && arg)
        : m_function(std::forward<ParentFunction>(fn)),
          m_firstArg(std::forward<Arg1>(arg)) {}

    inline R operator()(Args&&... args) const
    {
        return m_function(std::forward<Arg1>(m_firstArg), std::forward<Args>(args)...);
    }

private:
    ParentFunction && m_function;
    Arg1 && m_firstArg;
};

template <typename F, typename R, typename Arg1, typename... Args>
inline BoundArgumentFunction<F, R, Arg1, Args...> partial_bind(F && f, Arg1 && a1)
{
    return BoundArgumentFunction<F, R, Arg1, Args...>(std::forward<F>(f), std::forward<Arg1>(a1));
}

//END ******** BoundArgumentFunction ********
//BEGIN ******** unpackAndInvoke ********

template <typename F, typename R>
inline void unpackAndInvoke(F && function, Value & result,
                            QList<Value>::const_iterator &&,
                            QList<Value>::const_iterator &&)
{
    invoker<F, R>::invoke(function, result);
}

template <typename F, typename R, typename Arg1, typename... Args>
inline void unpackAndInvoke(F && function, Value & result,
                            QList<Value>::const_iterator && argsBegin,
                            QList<Value>::const_iterator && argsEnd)
{
    typedef typename boost::remove_const<
                typename boost::remove_reference<Arg1>::type
            >::type CleanArg1;
    typedef BoundArgumentFunction<F, R, Arg1, Args...> F1;

    CleanArg1 && boundArg = ValueImpl<CleanArg1>::get(*argsBegin);
    F1 && f = partial_bind<F, R, Arg1, Args...>(std::forward<F>(function), std::forward<Arg1>(boundArg));

    unpackAndInvoke< F1, R, Args... >(std::forward<F1>(f), result,
                                      std::forward<QList<Value>::const_iterator>(++argsBegin),
                                      std::forward<QList<Value>::const_iterator>(argsEnd));
}

//END ******** unpackAndInvoke ********
//BEGIN ******** CppClosure ********

template <typename F, typename R, typename... Args>
struct CppClosure<F, R (Args...)>
{
    class ClosureData : public ClosureDataBase
    {
    public:
        inline ClosureData(const F & func, bool passSender)
            : ClosureDataBase(passSender), m_function(func) {}

        virtual void marshaller(Value & result, const QList<Value> & params)
        {
            if (static_cast<size_t>(params.size()) < sizeof...(Args)) {
                throw std::logic_error("The signal provides less arguments than what the closure expects");
            }

            unpackAndInvoke<F, R, Args...>(std::forward<F>(m_function), result,
                                           params.constBegin(), params.constEnd());
        }

    private:
        F m_function;
    };

    static inline ClosureDataBase *create(const F & function, bool passSender)
    {
        return new ClosureData(function, passSender);
    }
};

//END ******** CppClosure ********

} //namespace Private

//BEGIN ******** QGlib::connect ********

template <typename T, typename R, typename... Args>
bool connect(void *instance, const char *detailedSignal,
             T *receiver, R (T::*slot)(Args...), ConnectFlags flags = 0)
{
    typedef Private::MemberFunction<T, R, Args...> F;

    F && f = Private::mem_fn(slot, receiver);
    Private::ClosureDataBase* && closure
        = Private::CppClosure<F, R (Args...)>::create(f, flags & PassSender);

    return Private::connect(instance, detailedSignal, Quark(),
                            receiver, Private::GetDestroyNotifier<T>(),
                            Private::hashMfp(slot), closure, flags);
}

//END ******** QGlib::connect ********

} //namespace QGlib

# else //QGLIB_HAVE_CXX0X

#  include <boost/function.hpp>
#  include <boost/preprocessor.hpp>
#  include <boost/bind.hpp>

// include the second part of this file as many times as QGLIB_CONNECT_MAX_ARGS specifies
#  define BOOST_PP_ITERATION_PARAMS_1 (3,(0, QGLIB_CONNECT_MAX_ARGS, "QGlib/connectimpl.h"))
#  include BOOST_PP_ITERATE()

#  undef BOOST_PP_ITERATION_PARAMS_1
#  undef QGLIB_CONNECT_MAX_ARGS

# endif //QGLIB_HAVE_CXX0X


#else // !defined(BOOST_PP_IS_ITERATING) || !BOOST_PP_IS_ITERATING

/*
    This part is included from BOOST_PP_ITERATE(). It defines a CppClosureN class
    (where N is the number of template arguments it takes) and a specialization for class
    CppClosure, so that the CppClosure<R (Args...), F> syntax is supported. This part is
    included multiple times (QGLIB_CONNECT_MAX_ARGS defines how many), and each time
    it defines those classes with different number of arguments.
    The concept is based on the implementation of boost::function.
*/

# define QGLIB_CONNECT_IMPL_NUM_ARGS \
    BOOST_PP_ITERATION()

# define QGLIB_CONNECT_IMPL_TRAILING_TEMPLATE_PARAMS \
    BOOST_PP_ENUM_TRAILING_PARAMS(QGLIB_CONNECT_IMPL_NUM_ARGS, typename A)

# define QGLIB_CONNECT_IMPL_TRAILING_TEMPLATE_ARGS \
    BOOST_PP_ENUM_TRAILING_PARAMS(QGLIB_CONNECT_IMPL_NUM_ARGS, A)

# define QGLIB_CONNECT_IMPL_TEMPLATE_ARGS \
    BOOST_PP_ENUM_PARAMS(QGLIB_CONNECT_IMPL_NUM_ARGS, A)

namespace QGlib {
namespace Private {

//BEGIN ******** boostpp CppClosure ********

# define QGLIB_CONNECT_IMPL_CPPCLOSUREN \
    BOOST_PP_CAT(CppClosure, QGLIB_CONNECT_IMPL_NUM_ARGS)

# define QGLIB_CONNECT_IMPL_UNPACK_ARGS_STEP(z, n, list) \
    ,ValueImpl< \
        typename boost::remove_const< \
            typename boost::remove_reference<A ##n>::type \
        >::type \
    >::get(list.at(n))

# define QGLIB_CONNECT_IMPL_UNPACK_ARGS(list) \
    BOOST_PP_REPEAT(QGLIB_CONNECT_IMPL_NUM_ARGS, QGLIB_CONNECT_IMPL_UNPACK_ARGS_STEP, list)

template <typename F, typename R QGLIB_CONNECT_IMPL_TRAILING_TEMPLATE_PARAMS>
struct QGLIB_CONNECT_IMPL_CPPCLOSUREN
{
    class ClosureData : public ClosureDataBase
    {
    public:
        inline ClosureData(const F & func, bool passSender)
            : ClosureDataBase(passSender), m_function(func) {}

        virtual void marshaller(Value & result, const QList<Value> & params)
        {
            if (params.size() < QGLIB_CONNECT_IMPL_NUM_ARGS) {
                throw std::logic_error("The signal provides less arguments than what the closure expects");
            }

# if QGLIB_CONNECT_IMPL_NUM_ARGS > 0
            boost::function<R ()> callback = boost::bind<R>(m_function
                                                            QGLIB_CONNECT_IMPL_UNPACK_ARGS(params));
            invoker< boost::function<R ()>, R >::invoke(callback, result);
# else
            invoker< F, R >::invoke(m_function, result);
# endif
        }

    private:
        F m_function;
    };

    static ClosureDataBase *create(const F & function, bool passSender)
    {
        return new ClosureData(function, passSender);
    }
};

//partial specialization of struct CppClosure to support the CppClosure<F, R (Args...)> syntax
template <typename F, typename R  QGLIB_CONNECT_IMPL_TRAILING_TEMPLATE_PARAMS>
struct CppClosure<F, R (QGLIB_CONNECT_IMPL_TEMPLATE_ARGS)>
    : public QGLIB_CONNECT_IMPL_CPPCLOSUREN< F, R QGLIB_CONNECT_IMPL_TRAILING_TEMPLATE_ARGS >
{
};

# undef QGLIB_CONNECT_IMPL_UNPACK_ARGS
# undef QGLIB_CONNECT_IMPL_UNPACK_ARGS_STEP
# undef QGLIB_CONNECT_IMPL_CPPCLOSUREN

//END ******** boostpp CppClosure ********

} //namespace Private

//BEGIN ******** bostpp QGlib::connect ********

# define QGLIB_CONNECT_IMPL_BIND_ARGS \
    BOOST_PP_COMMA_IF(QGLIB_CONNECT_IMPL_NUM_ARGS) \
    BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_PP_INC(QGLIB_CONNECT_IMPL_NUM_ARGS), _)

template <typename T, typename R QGLIB_CONNECT_IMPL_TRAILING_TEMPLATE_PARAMS>
bool connect(void *instance, const char *detailedSignal,
             T *receiver, R (T::*slot)(QGLIB_CONNECT_IMPL_TEMPLATE_ARGS), ConnectFlags flags = 0)
{
    boost::function<R (QGLIB_CONNECT_IMPL_TEMPLATE_ARGS)> f
        = boost::bind(slot, receiver QGLIB_CONNECT_IMPL_BIND_ARGS);

    Private::ClosureDataBase *closure = Private::CppClosure<
            boost::function<R (QGLIB_CONNECT_IMPL_TEMPLATE_ARGS)>,
            R (QGLIB_CONNECT_IMPL_TEMPLATE_ARGS)
        >::create(f, flags & PassSender);

    return Private::connect(instance, detailedSignal, Quark(),
                            receiver, Private::GetDestroyNotifier<T>(),
                            Private::hashMfp(slot), closure, flags);
}

# undef QGLIB_CONNECT_IMPL_BIND_ARGS

//END ******** bostpp QGlib::connect ********

} //namespace QGlib

# undef QGLIB_CONNECT_IMPL_TEMPLATE_ARGS
# undef QGLIB_CONNECT_IMPL_TRAILING_TEMPLATE_ARGS
# undef QGLIB_CONNECT_IMPL_TRAILING_TEMPLATE_PARAMS
# undef QGLIB_CONNECT_IMPL_NUM_ARGS

#endif // !defined(BOOST_PP_IS_ITERATING) || !BOOST_PP_IS_ITERATING
