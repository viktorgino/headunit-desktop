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
#ifndef QGLIB_QUARK_H
#define QGLIB_QUARK_H

#include "global.h"
#include <QtCore/QString>

namespace QGlib {

/*! \headerfile quark.h <QGlib/Quark>
 * \brief Wrapper class for GQuark
 *
 * Quarks are associations between strings and integer identifiers.
 * Given either the string or the Quark identifier it is possible to retrieve the other.
 *
 * To create a new quark from a string, use fromString().
 *
 * To find the string corresponding to a given Quark, use toString().
 *
 * To find the Quark corresponding to a given string, use tryString().
 *
 * \note This class is a thin wrapper around a quint32. There is no overhead in copying
 * it around, since it is just an integer.
 */
class QTGLIB_EXPORT Quark
{
public:
    inline Quark(quint32 gquark = 0) : m_quark(gquark) {}

    /*! Creates a new Quark given a string \a str. */
    static Quark fromString(const char *str);
    static inline Quark fromString(const QString & str); ///< \overload

    /*! Finds an existing Quark that corresponds to the given string \a str.
     * If the Quark is not found, an invalid quark (equal to 0) is returned. */
    static Quark tryString(const char *str);
    static inline Quark tryString(const QString & str); ///< \overload

    /*! Retrieves the string that corresponds to this Quark. */
    QString toString() const;

    inline operator quint32() const { return m_quark; }

private:
    quint32 m_quark;
};

//static
inline Quark Quark::fromString(const QString & str)
{
    return fromString(str.toUtf8().constData());
}

//static
inline Quark Quark::tryString(const QString & str)
{
    return tryString(str.toUtf8().constData());
}

} //namespace QGlib

#endif // QGLIB_QUARK_H
