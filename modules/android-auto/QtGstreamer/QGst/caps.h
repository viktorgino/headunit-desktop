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
#ifndef QGST_CAPS_H
#define QGST_CAPS_H

#include "global.h"
#include "miniobject.h"
#include "../QGlib/value.h"
#include "../QGlib/refpointer.h"
#include "../QGlib/type.h"

namespace QGst {

/*! \headerfile caps.h <QGst/Caps>
 * \brief Wrapper class for GstCaps
 */
class QTGSTREAMER_EXPORT Caps : public QGst::MiniObject
{
    QGST_WRAPPER(Caps)
public:
    static CapsPtr createSimple(const char *mediaType);
    static CapsPtr createAny();
    static CapsPtr createEmpty();

    static CapsPtr fromString(const char *string);
    static inline CapsPtr fromString(const QString & string);
    QString toString() const;

    void append(const CapsPtr & caps2);
    CapsPtr merge(CapsPtr & caps2);
    template <typename T>
    inline void setValue(const char *field, const T & value);
    void setValue(const char *field, const QGlib::Value & value);
    bool simplify();
    CapsPtr truncate();

    StructurePtr internalStructure(uint index);

    void appendStructure(const Structure & structure);
    CapsPtr mergeStructure(Structure & structure);
    void removeStructure(uint index);

    uint size() const;
    bool isSimple() const;
    bool isAny() const;
    bool isEmpty() const;
    bool isFixed() const;
    bool equals(const CapsPtr & caps2) const;
    bool isAlwaysCompatibleWith(const CapsPtr & caps2) const;
    bool isSubsetOf(const CapsPtr & superset) const;
    bool canIntersect(const CapsPtr & caps2) const;
    CapsPtr getIntersection(const CapsPtr & caps2) const;
    CapsPtr getNormal();
    CapsPtr subtract(const CapsPtr & subtrahend) const;

    CapsPtr copy() const;
    CapsPtr copyNth(uint index) const;

    inline CapsPtr makeWritable() const;
};

template <typename T>
inline void Caps::setValue(const char *field, const T & value)
{
    setValue(field, QGlib::Value::create(value));
}

//static
inline CapsPtr Caps::fromString(const QString & string)
{
    return fromString(string.toUtf8().constData());
}

inline CapsPtr Caps::makeWritable() const
{
    return MiniObject::makeWritable().staticCast<Caps>();
}

/*! \relates QGst::Caps */
QTGSTREAMER_EXPORT QDebug operator<<(QDebug debug, const CapsPtr & caps);


} //namespace QGst

QGST_REGISTER_TYPE(QGst::Caps)

#endif
