/*
    Copyright (C) 2010  Collabora Multimedia.
      @author Mauricio Piacentini <mauricio.piacentini@collabora.co.uk>

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
#ifndef QGST_QUERY_H
#define QGST_QUERY_H

#include "miniobject.h"
#include "structure.h"
#include "clocktime.h"

class QUrl;

namespace QGst {

    /*! \headerfile query.h <QGst/Query>
     * \brief Wrapper class for GstQuery
     *
     * Queries are lightweight objects that are sent from the application directly to elements
     * or pads using the query() methods. A typical usage is to find out the duration or current
     * position of media, or to display the status of a buffering operation.
     *
     * Queries are implemented as a subclass of MiniObject with a generic GstStructure as the
     * content. This allows for writing custom queries without requiring an API change while
     * allowing a wide range of different types of queries.
     *
     * QGst::Query is the common base class for all query types.
     *
     * In these bindings, for convenience, each query type has its own Query subclass. The create()
     * method in the subclasses should be used to create a query object. This does not reflect 1-1
     * the native C API, where there is only one Query class with tens of 'new_foo' and 'parse_foo'
     * methods.
     *
     * Note that the Query subclasses \em cannot be used with Value::get(), since a GValue
     * will actually contain a GstQuery (the subclasses do not exist in C) and Value::get()
     * is not able to do dynamic casts. As a result of that, Query subclasses also \em cannot be
     * used as arguments in slots connected to GObject signals, even though you may know that your
     * slot will only be called with that type of queries.
     */
class QTGSTREAMER_EXPORT Query : public MiniObject
{
    QGST_WRAPPER(Query)
public:
    QString typeName() const;
    QueryType type() const;

    StructureConstPtr internalStructure();
};

/*! \headerfile query.h <QGst/Query>
 * \brief Wrapper class for queries of type QGst::PositionQuery
 */
class QTGSTREAMER_EXPORT PositionQuery : public Query
{
    QGST_WRAPPER_FAKE_SUBCLASS(Position, Query)
public:
    static PositionQueryPtr create(Format format);

    Format format() const;
    qint64 position() const;
    void setValues(Format format, qint64 position);
};

/*! \headerfile query.h <QGst/Query>
 * \brief Wrapper class for queries of type QGst::DurationQuery
 */
class QTGSTREAMER_EXPORT DurationQuery : public Query
{
    QGST_WRAPPER_FAKE_SUBCLASS(Duration, Query)
public:
    static DurationQueryPtr create(Format format);

    Format format() const;
    qint64 duration() const;
    void setValues(Format format, qint64 duration);
};

/*! \headerfile query.h <QGst/Query>
 * \brief Wrapper class for queries of type QGst::LatencyQuery
 */
class QTGSTREAMER_EXPORT LatencyQuery : public Query
{
    QGST_WRAPPER_FAKE_SUBCLASS(Latency, Query)
public:
    static LatencyQueryPtr create();

    bool hasLive() const;
    ClockTime minimumLatency() const;
    ClockTime maximumLatency() const;

    void setValues(bool live, ClockTime minimumLatency, ClockTime maximumLatency);
};

/*! \headerfile query.h <QGst/Query>
 * \brief Wrapper class for queries of type QGst::SeekingQuery
 */
class QTGSTREAMER_EXPORT SeekingQuery : public Query
{
    QGST_WRAPPER_FAKE_SUBCLASS(Seeking, Query)
public:
    static SeekingQueryPtr create(Format format);

    Format format() const;
    bool seekable() const;
    qint64 segmentStart() const;
    qint64 segmentEnd() const;

    void setValues(Format format, bool seekable, qint64 segmentStart, qint64 segmentEnd);
};

/*! \headerfile query.h <QGst/Query>
 * \brief Wrapper class for queries of type QGst::SegmentQuery
 */
class QTGSTREAMER_EXPORT SegmentQuery : public Query
{
    QGST_WRAPPER_FAKE_SUBCLASS(Segment, Query)
public:
    static SegmentQueryPtr create(Format format);

    Format format() const;
    double rate() const;
    qint64 startValue() const;
    qint64 stopValue() const;

    void setValues(Format format, double rate, qint64 startValue, qint64 stopValue);
};

/*! \headerfile query.h <QGst/Query>
 * \brief Wrapper class for queries of type QGst::ConvertQuery
 */
class QTGSTREAMER_EXPORT ConvertQuery : public Query
{
    QGST_WRAPPER_FAKE_SUBCLASS(Convert, Query)
public:
    static ConvertQueryPtr create(Format sourceFormat, qint64 value, Format destinationFormat);

    Format sourceFormat() const;
    qint64 sourceValue() const;
    Format destinationFormat() const;
    qint64 destinationValue() const;

    void setValues(Format sourceFormat, qint64 sourcevalue, Format destinationFormat,
                    qint64 destinationValue);
};

/*! \headerfile query.h <QGst/Query>
 * \brief Wrapper class for queries of type QGst::FormatsQuery
 */
class QTGSTREAMER_EXPORT FormatsQuery : public Query
{
    QGST_WRAPPER_FAKE_SUBCLASS(Formats, Query)
public:
    static FormatsQueryPtr create();

    QList<Format> formats() const;
    void setFormats(const QList<Format> & formats);
};

/*! \headerfile query.h <QGst/Query>
 * \brief Wrapper class for queries of type QGst::BufferingQuery
 */
class QTGSTREAMER_EXPORT BufferingQuery : public Query
{
    QGST_WRAPPER_FAKE_SUBCLASS(Buffering, Query)
public:
    static BufferingQueryPtr create(Format format);

    bool isBusy() const;
    int percent() const;

    void setBufferingPercent(bool busy, int percent);

    BufferingMode mode() const;
    int averageIn() const;
    int averageOut() const;
    qint64 bufferingLeft() const;

    void setBufferingStats(BufferingMode mode, int averageIn,
                           int averageOut, qint64 bufferingLeft);

    Format rangeFormat() const;
    qint64 rangeStart() const;
    qint64 rangeStop() const;
    qint64 estimatedTotal() const;

    void setBufferingRange(Format rangeFormat, qint64 rangeStart,
                           qint64 rangeStop, qint64 estimatedTotal);
};

/*! \headerfile query.h <QGst/Query>
 * \brief Wrapper class for queries of type QGst::UriQuery
 */
class QTGSTREAMER_EXPORT UriQuery : public Query
{
    QGST_WRAPPER_FAKE_SUBCLASS(Uri, Query)
public:
    static UriQueryPtr create();

    QUrl uri() const;
    void setUri(const QUrl & uri);
};

} //namespace QGst

QGST_REGISTER_TYPE(QGst::Query)
QGST_REGISTER_SUBCLASS(Query, Position)
QGST_REGISTER_SUBCLASS(Query, Duration)
QGST_REGISTER_SUBCLASS(Query, Latency)
QGST_REGISTER_SUBCLASS(Query, Seeking)
QGST_REGISTER_SUBCLASS(Query, Segment)
QGST_REGISTER_SUBCLASS(Query, Convert)
QGST_REGISTER_SUBCLASS(Query, Formats)
QGST_REGISTER_SUBCLASS(Query, Buffering)
QGST_REGISTER_SUBCLASS(Query, Uri)

#endif
