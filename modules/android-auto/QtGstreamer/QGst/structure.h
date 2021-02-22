/*
    Copyright (C) 2009-2010  George Kiagiadakis <kiagiadakis.george@gmail.com>
    Copyright (C) 2010  Collabora Multimedia.
      @author Mauricio Piacentini <mauricio.piacentini@collabora.co.uk>
    Copyright (C) 2011  Collabora Ltd.
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
#ifndef QGST_STRUCTURE_H
#define QGST_STRUCTURE_H

#include "global.h"
#include "../QGlib/type.h"
#include "../QGlib/value.h"
#include <QtCore/QString>

namespace QGst {

/*! \headerfile structure.h <QGst/Structure>
 * \brief Wrapper for GstStructure
 *
 * A Structure is a collection of key/value pairs. In addition to the key/value pairs,
 * a Structure also has a name. The name starts with a letter and can be folled by letters,
 * numbers and any of "/-_.:". Structure is used by various GStreamer subsystems to store
 * information in a flexible and extensible way.
 *
 * To use a Structure, you must first give it a name, either on the constructor or using
 * the setName() method. Afterwards, you can set values with setValue() and retrieve them
 * with value().
 *
 * Structure is also serializable. You can use toString() to serialize it into a string
 * and fromString() to deserialize it.
 *
 * \note This class is implicitly shared.
 * \sa SharedStructure
 */
class QTGSTREAMER_EXPORT Structure
{
public:
    Structure();
    explicit Structure(const char *name);
    explicit Structure(const GstStructure *structure);
    Structure(const Structure & other);
    virtual ~Structure();

    Structure & operator=(const Structure & other);

    bool isValid() const;

    QString name() const;
    void setName(const char *name);

    QGlib::Value value(const char *fieldName) const;
    template <typename T>
    inline void setValue(const char *fieldName, const T & value);
    void setValue(const char *fieldName, const QGlib::Value & value);

    unsigned int numberOfFields() const;
    QString fieldName(unsigned int fieldNumber) const;
    QGlib::Type fieldType(const char *fieldName) const;
    bool hasField(const char *fieldName) const;
    bool hasFieldTyped(const char *fieldName, QGlib::Type type) const;

    void removeField(const char *fieldName);
    void removeAllFields();

    QString toString() const;
    static Structure fromString(const char *str);
    static inline Structure fromString(const QString & str);

    operator GstStructure*();
    operator const GstStructure*() const;

private:
    friend class SharedStructure;

    struct Data;

    QTGSTREAMER_NO_EXPORT
    Structure(Data *data);

    QSharedDataPointer<Data> d;
};

template <typename T>
inline void Structure::setValue(const char *fieldName, const T & value)
{
    setValue(fieldName, QGlib::Value::create(value));
}

//static
inline Structure Structure::fromString(const QString & str)
{
    return fromString(str.toUtf8().constData());
}


/*! \headerfile structure.h <QGst/Structure>
 * \brief Helper for shared GstStructure instances
 *
 * This class serves as a helper for wrapping shared GstStructure instances. Some functions in
 * the GStreamer API return a pointer to some internal GstStructure and expect you to change this
 * internal instance, not copy it and re-set it using some setter function (like all normal
 * object-oriented APIs do), so it is necessary to have way of accessing those instances.
 *
 * This class wraps a GstStructure without copying it and without freeing it from the destructor,
 * unlike Structure, which always keeps a GstStructure instance for itself.
 * In addition to that, this class also holds a reference to the GstStructure's parent object,
 * so that it doesn't accidentally get deleted because its smart pointer has gone out of scope.
 *
 * Since this class is not copy-able (to prevent misuse), to ease using it, there is a StructurePtr
 * typedef for QSharedPointer<SharedStructure> available. You should only use this class through
 * StructurePtr and not directly.
 *
 * \sa Structure, Caps::internalStructure
 */
class QTGSTREAMER_EXPORT SharedStructure : public Structure
{
public:
    virtual ~SharedStructure();

    /*! \returns a deep copy of the structure */
    Structure copy() const;

private:
    //Only these classes can call our constructor, others should use a StructurePtr passed to them
    friend class Caps;
    friend class Message;
    friend class Event;
    friend class Query;

    struct Data;

    QTGSTREAMER_NO_EXPORT
    SharedStructure(Data *data);
    QTGSTREAMER_NO_EXPORT
    static StructurePtr fromMiniObject(GstStructure *structure, const MiniObjectPtr & parent);
    QTGSTREAMER_NO_EXPORT
    static StructurePtr fromCaps(GstStructure *structure, const CapsPtr & parent);

    Q_DISABLE_COPY(SharedStructure);
};

/*! \relates QGst::Structure */
QTGSTREAMER_EXPORT QDebug operator<<(QDebug debug, const Structure & structure);

} //namespace QGst

QGST_REGISTER_TYPE(QGst::Structure)

#endif
