/*
    Copyright (C) 2010-2011 Collabora Ltd.
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
#include "structs.h"
#include "miniobject.h"
#include "structure.h"
#include "../QGlib/value.h"
#include <cmath>
#include <gst/gst.h>

namespace QGlib {

GetTypeImpl<QDate>::operator Type() { return G_TYPE_DATE; }
GetTypeImpl<QDateTime>::operator Type() { return GST_TYPE_DATE_TIME; }

} //namespace QGlib

namespace QGst {
namespace Private {

void registerValueVTables()
{
    struct ValueVTable_Fraction
    {
        static void get(const QGlib::Value & value, void *data)
        {
            reinterpret_cast<Fraction*>(data)->numerator = gst_value_get_fraction_numerator(value);
            reinterpret_cast<Fraction*>(data)->denominator = gst_value_get_fraction_denominator(value);
        };

        static void set(QGlib::Value & value, const void *data)
        {
            gst_value_set_fraction(value, reinterpret_cast<Fraction const *>(data)->numerator,
                                          reinterpret_cast<Fraction const *>(data)->denominator);
        };
    };
    QGlib::Value::registerValueVTable(QGlib::GetType<Fraction>(),
            QGlib::ValueVTable(ValueVTable_Fraction::set, ValueVTable_Fraction::get));


    struct ValueVTable_IntRange
    {
        static void get(const QGlib::Value & value, void *data)
        {
            reinterpret_cast<IntRange*>(data)->start = gst_value_get_int_range_min(value);
            reinterpret_cast<IntRange*>(data)->end = gst_value_get_int_range_max(value);
        };

        static void set(QGlib::Value & value, const void *data)
        {
            gst_value_set_int_range(value, reinterpret_cast<IntRange const *>(data)->start,
                                           reinterpret_cast<IntRange const *>(data)->end);
        };
    };
    QGlib::Value::registerValueVTable(QGlib::GetType<IntRange>(),
            QGlib::ValueVTable(ValueVTable_IntRange::set, ValueVTable_IntRange::get));

    struct ValueVTable_Int64Range
    {
        static void get(const QGlib::Value & value, void *data)
        {
            reinterpret_cast<Int64Range*>(data)->start = gst_value_get_int64_range_min(value);
            reinterpret_cast<Int64Range*>(data)->end = gst_value_get_int64_range_max(value);
        };

        static void set(QGlib::Value & value, const void *data)
        {
            gst_value_set_int64_range(value, reinterpret_cast<Int64Range const *>(data)->start,
                                             reinterpret_cast<Int64Range const *>(data)->end);
        };
    };
    QGlib::Value::registerValueVTable(QGlib::GetType<Int64Range>(),
            QGlib::ValueVTable(ValueVTable_Int64Range::set, ValueVTable_Int64Range::get));


    struct ValueVTable_DoubleRange
    {
        static void get(const QGlib::Value & value, void *data)
        {
            reinterpret_cast<DoubleRange*>(data)->start = gst_value_get_double_range_min(value);
            reinterpret_cast<DoubleRange*>(data)->end = gst_value_get_double_range_max(value);
        };

        static void set(QGlib::Value & value, const void *data)
        {
            gst_value_set_double_range(value, reinterpret_cast<DoubleRange const *>(data)->start,
                                              reinterpret_cast<DoubleRange const *>(data)->end);
        };
    };
    QGlib::Value::registerValueVTable(QGlib::GetType<DoubleRange>(),
            QGlib::ValueVTable(ValueVTable_DoubleRange::set, ValueVTable_DoubleRange::get));


    struct ValueVTable_FractionRange
    {
        static void get(const QGlib::Value & value, void *data)
        {
            reinterpret_cast<FractionRange*>(data)->start.numerator =
                gst_value_get_fraction_numerator(gst_value_get_fraction_range_min(value));
            reinterpret_cast<FractionRange*>(data)->start.denominator =
                gst_value_get_fraction_denominator(gst_value_get_fraction_range_min(value));
            reinterpret_cast<FractionRange*>(data)->end.numerator =
                gst_value_get_fraction_numerator(gst_value_get_fraction_range_max(value));
            reinterpret_cast<FractionRange*>(data)->end.denominator =
                gst_value_get_fraction_denominator(gst_value_get_fraction_range_max(value));
        };

        static void set(QGlib::Value & value, const void *data)
        {
            gst_value_set_fraction_range_full(value,
                    reinterpret_cast<FractionRange const *>(data)->start.numerator,
                    reinterpret_cast<FractionRange const *>(data)->start.denominator,
                    reinterpret_cast<FractionRange const *>(data)->end.numerator,
                    reinterpret_cast<FractionRange const *>(data)->end.denominator);
        };
    };
    QGlib::Value::registerValueVTable(QGlib::GetType<FractionRange>(),
            QGlib::ValueVTable(ValueVTable_FractionRange::set, ValueVTable_FractionRange::get));

    struct ValueVTable_Structure
    {
        static void get(const QGlib::Value & value, void *data)
        {
            *reinterpret_cast<Structure*>(data) = Structure(gst_value_get_structure(value));
        };

        static void set(QGlib::Value & value, const void *data)
        {
            gst_value_set_structure(value, *reinterpret_cast<Structure const *>(data));
        };
    };
    QGlib::Value::registerValueVTable(QGlib::GetType<Structure>(),
            QGlib::ValueVTable(ValueVTable_Structure::set, ValueVTable_Structure::get));

    struct ValueVTable_QDate
    {
        static void get(const QGlib::Value & value, void *data)
        {
            const GDate *gdate = static_cast<const GDate *>(g_value_get_boxed(value));
            *reinterpret_cast<QDate*>(data) = QDate(g_date_get_year(gdate),
                                                    g_date_get_month(gdate),
                                                    g_date_get_day(gdate));
        }

        static void set(QGlib::Value & value, const void *data)
        {
            const QDate *qdate = reinterpret_cast<QDate const *>(data);
            GDate *gdate = g_date_new_dmy(qdate->day(),
                                          static_cast<GDateMonth>(qdate->month()),
                                          qdate->year());
            g_value_set_boxed(value, gdate);
            g_date_free(gdate);
        }
    };
    QGlib::Value::registerValueVTable(QGlib::GetType<QDate>(),
            QGlib::ValueVTable(ValueVTable_QDate::set, ValueVTable_QDate::get));

    struct ValueVTable_QDateTime
    {
        static void get(const QGlib::Value & value, void *data)
        {
            const GstDateTime *gdatetime = static_cast<GstDateTime*>(g_value_get_boxed(value));

            QDate date = QDate(gst_date_time_get_year(gdatetime),
                               gst_date_time_get_month(gdatetime),
                               gst_date_time_get_day(gdatetime));

            /* timezone conversion */
            float tzoffset = gst_date_time_get_time_zone_offset(gdatetime);
            float hourOffset;
            float minutesOffset = std::modf(tzoffset, &hourOffset);

            int hour = gst_date_time_get_hour(gdatetime) - hourOffset;
            int minute = gst_date_time_get_minute(gdatetime) - (minutesOffset * 60);

            /* handle overflow */
            if (minute >= 60) {
                hour++;
                minute -= 60;
            } else if (minute < 0) {
                hour--;
                minute = 60 + minute;
            }

            if (hour >= 24) {
                date = date.addDays(1);
                hour -= 24;
            } else if (hour < 0) {
                date = date.addDays(-1);
                hour = 24 + hour;
            }

            QTime time = QTime(hour, minute,
                               gst_date_time_get_second(gdatetime),
                               gst_date_time_get_microsecond(gdatetime)/1000);

            *reinterpret_cast<QDateTime*>(data) = QDateTime(date, time, Qt::UTC);
        }

        static void set(QGlib::Value & value, const void *data)
        {
            QDateTime qdatetime = reinterpret_cast<QDateTime const *>(data)->toUTC();
            GstDateTime *gdatetime = gst_date_time_new(0.0f,
                qdatetime.date().year(),
                qdatetime.date().month(),
                qdatetime.date().day(),
                qdatetime.time().hour(),
                qdatetime.time().minute(),
                qdatetime.time().second() + (qdatetime.time().msec()/1000.0)
            );

            g_value_take_boxed(value, gdatetime);
        }
    };
    QGlib::Value::registerValueVTable(QGlib::GetType<QDateTime>(),
            QGlib::ValueVTable(ValueVTable_QDateTime::set, ValueVTable_QDateTime::get));
}

} //namespace Private
} //namespace QGst
