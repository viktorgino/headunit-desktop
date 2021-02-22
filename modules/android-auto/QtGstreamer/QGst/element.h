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
#ifndef QGST_ELEMENT_H
#define QGST_ELEMENT_H

#include "object.h"
#include "caps.h"
#include "clocktime.h"

#if !QGLIB_HAVE_CXX0X
# include <boost/preprocessor.hpp>
#endif

namespace QGst {

/*! \headerfile element.h <QGst/Element>
 * \brief Wrapper class for GstElement
 */
class QTGSTREAMER_EXPORT Element : public Object
{
    QGST_WRAPPER(Element)
public:
    State currentState() const;
    State pendingState() const;
    StateChangeReturn getState(State *state, State *pending, ClockTime timeout) const;
    StateChangeReturn setState(State state);
    bool syncStateWithParent();

    bool stateIsLocked() const;
    bool setStateLocked(bool locked);

    bool addPad(const PadPtr & pad);
    bool removePad(const PadPtr & pad);

    PadPtr getStaticPad(const char *name);
    PadPtr getRequestPad(const char *name);
    void releaseRequestPad(const PadPtr & pad);

    ClockPtr clock() const;
    bool setClock(const ClockPtr & clock);

    bool link(const char *srcPadName, const ElementPtr & dest,
              const char *sinkPadName, const CapsPtr & filter = CapsPtr());
    bool link(const char *srcPadName, const ElementPtr & dest,
              const CapsPtr & filter = CapsPtr());
    bool link(const ElementPtr & dest, const char *sinkPadName,
              const CapsPtr & filter = CapsPtr());
    bool link(const ElementPtr & dest, const CapsPtr & filter = CapsPtr());

#ifndef DOXYGEN_RUN
    //FIXME-0.11 Remove the default NULL parameter from the first version
    //and remove entirely sinkPadName from the second version
    void unlink(const char *srcPadName, const ElementPtr & dest,
                const char *sinkPadName = NULL);
    void unlink(const ElementPtr & dest, const char *sinkPadName = NULL);
#else
    //documentation should show those functions as they are meant to be used.
    /*! Unlinks the pad named \a srcPadName of this element
     * from the pad named \a sinkPadName of the \a dest element. */
    void unlink(const char *srcPadName, const ElementPtr & dest, const char *sinkPadName);

    /*! Unlinks this element from the \a dest element */
    void unlink(const ElementPtr & dest);
#endif

#ifndef DOXYGEN_RUN
private:
    static inline bool linkMany(const ElementPtr & first, const ElementPtr & second)
    {
        return first->link(second);
    }

    static inline void unlinkMany(const ElementPtr & first, const ElementPtr & second)
    {
        first->unlink(second);
    }
public:
#endif

#if QGLIB_HAVE_CXX0X
    /*! Links together a series of elements in the order that they are provided.
     * \note This function makes use of C++0x features. If your compiler doesn't support
     * this, a different version will be compiled. That version supports up to
     * QGST_ELEMENT_LINK_MANY_MAX_ARGS arguments, which defaults to 10. If you need more,
     * define this to a greater value before including any QtGStreamer headers.
     */
    template <typename First, typename Second, typename Third, typename... Rest>
    static inline bool linkMany(const First & first, const Second & second,
                                const Third & third, const Rest & ... rest)
    {
        if (!first->link(second)) return false;
        return linkMany(second, third, rest...);
    }

    /*! Unlinks a chain of elements.
     * \note This function makes use of C++0x features. If your compiler doesn't support
     * this, a different version will be compiled. That version supports up to
     * QGST_ELEMENT_UNLINK_MANY_MAX_ARGS arguments, which defaults to 10. If you need more,
     * define this to a greater value before including any QtGStreamer headers.
     */
    template <typename First, typename Second, typename Third, typename... Rest>
    static inline void unlinkMany(const First & first, const Second & second,
                                  const Third & third, const Rest & ... rest)
    {
        first->unlink(second);
        unlinkMany(second, third, rest...);
    }
#else //QGLIB_HAVE_CXX0X

# ifndef QGST_ELEMENT_LINK_MANY_MAX_ARGS
#  define QGST_ELEMENT_LINK_MANY_MAX_ARGS 10
# endif

# ifndef QGST_ELEMENT_UNLINK_MANY_MAX_ARGS
#  define QGST_ELEMENT_UNLINK_MANY_MAX_ARGS 10
# endif

# define QGST_ELEMENT_LINK_MANY_DECLARATION(z, n, data) \
    static inline bool linkMany(BOOST_PP_ENUM_PARAMS(n, const ElementPtr & e)) \
    { \
        if (!e0->link(e1)) return false; \
        return linkMany(BOOST_PP_ENUM_SHIFTED_PARAMS(n, e)); \
    };

# define QGST_ELEMENT_UNLINK_MANY_DECLARATION(z, n, data) \
    static inline void unlinkMany(BOOST_PP_ENUM_PARAMS(n, const ElementPtr & e)) \
    { \
        e0->unlink(e1); \
        unlinkMany(BOOST_PP_ENUM_SHIFTED_PARAMS(n, e)); \
    };

    BOOST_PP_REPEAT_FROM_TO(3, BOOST_PP_INC(QGST_ELEMENT_LINK_MANY_MAX_ARGS),
                            QGST_ELEMENT_LINK_MANY_DECLARATION, dummy)
    BOOST_PP_REPEAT_FROM_TO(3, BOOST_PP_INC(QGST_ELEMENT_UNLINK_MANY_MAX_ARGS),
                            QGST_ELEMENT_UNLINK_MANY_DECLARATION, dummy)

# undef QGST_ELEMENT_LINK_MANY_DECLARATION
# undef QGST_ELEMENT_UNLINK_MANY_DECLARATION

#endif //QGLIB_HAVE_CXX0X

    bool query(const QueryPtr & query);
    bool sendEvent(const EventPtr & event);
    bool seek(Format format, SeekFlags flags, quint64 position);
};

}

QGST_REGISTER_TYPE(QGst::Element)

#endif // QGST_ELEMENT_H
