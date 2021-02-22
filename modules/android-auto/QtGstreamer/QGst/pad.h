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
#ifndef QGST_PAD_H
#define QGST_PAD_H

#include "object.h"

namespace QGst {

/*! \headerfile pad.h <QGst/Pad>
 * \brief Wrapper class for GstPad
 */
class QTGSTREAMER_EXPORT Pad : public Object
{
    QGST_WRAPPER(Pad)
public:
    static PadPtr create(PadDirection direction, const char *name = NULL);

    PadDirection direction() const;

    /*! This method returns the pad's parent casted to an Element.
     * It is equivalent to pad->parent().dynamicCast<QGst::Element>();
     * If no parent is set, it will return a null RefPointer.
     */
    ElementPtr parentElement() const;
    PadPtr peer() const;

    bool isLinked() const;
    bool canLink(const PadPtr & sink) const;
    PadLinkReturn link(const PadPtr & sink);
    bool unlink(const PadPtr & sink);

    CapsPtr currentCaps() const;
    CapsPtr allowedCaps() const;
    CapsPtr padTemplateCaps() const;

    bool isActive() const;
    bool setActive(bool active);

    bool isBlocked() const;
    bool isBlocking() const;

    bool query(const QueryPtr & query);
    bool sendEvent(const EventPtr & event);
};

}

QGST_REGISTER_TYPE(QGst::Pad)

#endif
