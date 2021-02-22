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
#ifndef QGST_STRUCTS_H
#define QGST_STRUCTS_H

#include "global.h"
#include <QtCore/QDebug>

namespace QGst {

    /*! \headerfile structs.h <QGst/Fourcc>
     * \brief Helper structure for accessing Fourcc values
     */
    struct QTGSTREAMER_EXPORT Fourcc
    {
        inline Fourcc() { value.as_integer = 0; }

        inline Fourcc (char first, char second, char third, char fourth)
        {
            value.as_integer = first | second << 8 | third << 16 | fourth << 24;
        }

        inline Fourcc(const char str[4])
        {
            value.as_integer = str[0] | str[1] << 8 | str[2] << 16 | str[3] << 24;
        }

        inline Fourcc(quint32 fourcc)
        {
            value.as_integer = fourcc;
        }

        union {
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
            struct {
                char first;
                char second;
                char third;
                char fourth;
            } as_bytes;
#else
            struct {
                char fourth;
                char third;
                char second;
                char first;
            } as_bytes;
#endif
            quint32 as_integer;
        } value;
    };
}
QGST_REGISTER_TYPE(QGst::Fourcc) //codegen: GType=G_TYPE_UINT

namespace QGst {
    /*! \headerfile structs.h <QGst/Fraction>
     * \brief Helper structure for accessing Fraction values
     */
    struct QTGSTREAMER_EXPORT Fraction
    {
        inline Fraction() {}
        inline Fraction(int numerator, int denominator)
            : numerator(numerator), denominator(denominator) {}

        inline bool operator==(const Fraction & other) const
        { return numerator == other.numerator && denominator == other.denominator; }
        inline bool operator!=(const Fraction & other) const
        { return !operator==(other); }

        int numerator;
        int denominator;
    };

    inline QDebug operator<<(QDebug debug, const Fraction &f)
    {
        return (debug.nospace() << f.numerator << "/" << f.denominator).maybeSpace();
    }
}
QGST_REGISTER_TYPE(QGst::Fraction)

namespace QGst {
    namespace Private {
        /*! \brief Common template for IntRange, Int64Range, DoubleRange and FractionRange
         * \note This structure is private and should not be used directly. You should use
         * QGst::IntRange, QGst::Int64Range, QGst::DoubleRange and QGst::FractionRange instead.
         */
        template <typename T>
        struct Range
        {
            inline Range()
                : start(T()), end(T()) {}
            inline Range(const T & start, const T & end)
                : start(start), end(end) {}

            T start;
            T end;
        };
    }

    /*! \headerfile structs.h <QGst/IntRange>
     * \brief Helper structure for accessing int ranges
     */
    typedef Private::Range<int> IntRange;

    /*! \headerfile structs.h <QGst/Int64Range>
     * \brief Helper structure for accessing qint64 ranges
     */
    typedef Private::Range<qint64> Int64Range;

    /*! \headerfile structs.h <QGst/DoubleRange>
     * \brief Helper structure for accessing double ranges
     */
    typedef Private::Range<double> DoubleRange;

    /*! \headerfile structs.h <QGst/FractionRange>
     * \brief Helper structure for accessing fraction ranges
     */
    typedef Private::Range<Fraction> FractionRange;
}
QGST_REGISTER_TYPE(QGst::IntRange)
QGST_REGISTER_TYPE(QGst::Int64Range)
QGST_REGISTER_TYPE(QGst::DoubleRange)
QGST_REGISTER_TYPE(QGst::FractionRange)

#endif // QGST_STRUCTS_H
