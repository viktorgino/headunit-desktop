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
#include "structure.h"
#include "miniobject.h"
#include "caps.h"
#include "../QGlib/string_p.h"
#include <gst/gst.h>
#include <QtCore/QDebug>

namespace QGst {

#ifndef DOXYGEN_RUN

struct QTGSTREAMER_NO_EXPORT Structure::Data : public QSharedData
{
    Data() : QSharedData(), structure(NULL) {}
    Data(const Data & other);
    virtual ~Data();

    GstStructure *structure;
};

Structure::Data::Data(const Structure::Data & other)
    : QSharedData(other), structure(NULL)
{
    if (other.structure) {
        structure = gst_structure_copy(other.structure);
    }
}

Structure::Data::~Data()
{
    if (structure) {
        gst_structure_free(structure);
    }
}

#endif //DOXYGEN_RUN

Structure::Structure()
    : d(new Data)
{
}

Structure::Structure(Data* data)
    : d(data)
{
}

Structure::Structure(const char *name)
    : d(new Data)
{
    d->structure = gst_structure_new_empty(name);
}

Structure::Structure(const GstStructure* structure)
    : d(new Data)
{
    d->structure = gst_structure_copy(structure);
}

Structure::Structure(const Structure & other)
    : d(other.d)
{
}

Structure::~Structure()
{
}

Structure & Structure::operator=(const Structure & other)
{
    d = other.d;
    return *this;
}

bool Structure::isValid() const
{
    return d->structure != NULL;
}

QString Structure::name() const
{
    if (d->structure) {
        return QString::fromUtf8(gst_structure_get_name(d->structure));
    } else {
        return QString();
    }
}

void Structure::setName(const char *name)
{
    if (!d->structure) {
        //lazy construction
        d->structure = gst_structure_new_empty(name);
    } else {
        gst_structure_set_name(d->structure, name);
    }
}

QGlib::Value Structure::value(const char *fieldName) const
{
    if (d->structure) {
        return QGlib::Value(gst_structure_get_value(d->structure, fieldName));
    } else {
        return QGlib::Value();
    }
}

void Structure::setValue(const char *fieldName, const QGlib::Value & value)
{
    Q_ASSERT(isValid());
    gst_structure_set_value(d->structure, fieldName, value);
}

unsigned int Structure::numberOfFields() const
{
    return d->structure ? gst_structure_n_fields(d->structure) : 0;
}

QString Structure::fieldName(unsigned int fieldNumber) const
{
    if (fieldNumber < numberOfFields()) {
        return QString::fromUtf8(gst_structure_nth_field_name(d->structure, fieldNumber));
    } else {
        return QString();
    }
}

QGlib::Type Structure::fieldType(const char *fieldName) const
{
    if (d->structure) {
        return gst_structure_get_field_type(d->structure, fieldName);
    } else {
        return QGlib::Type::Invalid;
    }
}

bool Structure::hasField(const char *fieldName) const
{
    return d->structure ? gst_structure_has_field(d->structure, fieldName) : false;
}

bool Structure::hasFieldTyped(const char *fieldName, QGlib::Type type) const
{
    return d->structure ? gst_structure_has_field_typed(d->structure, fieldName, type) : false;
}

void Structure::removeField(const char *fieldName)
{
    if (d->structure) {
        gst_structure_remove_field(d->structure, fieldName);
    }
}

void Structure::removeAllFields()
{
    if (d->structure) {
        gst_structure_remove_all_fields(d->structure);
    }
}

QString Structure::toString() const
{
    if (d->structure) {
        return QGlib::Private::stringFromGCharPtr(gst_structure_to_string(d->structure));
    } else {
        return QString();
    }
}

Structure Structure::fromString(const char *str)
{
    //we don't use the Structure(const GstStructure*) constructor to avoid copying
    Structure s;
    s.d->structure = gst_structure_from_string(str, NULL);
    return s;
}

Structure::operator GstStructure*()
{
    return d->structure;
}

Structure::operator const GstStructure*() const
{
    return d->structure;
}

//END Structure

//BEGIN SharedStructure

#ifndef DOXYGEN_RUN

struct QTGSTREAMER_NO_EXPORT SharedStructure::Data : public Structure::Data
{
    Data() : Structure::Data() {}
    Data(const Data & other) : Structure::Data(other) {}

    MiniObjectPtr miniobject;
    CapsPtr caps;
};

#endif

Structure SharedStructure::copy() const
{
    return Structure(d->structure);
}

SharedStructure::SharedStructure(SharedStructure::Data* data)
    : Structure(data)
{
}

StructurePtr SharedStructure::fromMiniObject(GstStructure *structure, const MiniObjectPtr & parent)
{
    SharedStructure::Data *d = new SharedStructure::Data;
    d->structure = structure;
    d->miniobject = parent;
    return StructurePtr(new SharedStructure(d));
}

StructurePtr SharedStructure::fromCaps(GstStructure* structure, const CapsPtr & parent)
{
    SharedStructure::Data *d = new SharedStructure::Data;
    d->structure = structure;
    d->caps = parent;
    return StructurePtr(new SharedStructure(d));
}

SharedStructure::~SharedStructure()
{
    d->structure = NULL;
}

//END SharedStructure

QDebug operator<<(QDebug debug, const Structure & structure)
{
    debug.nospace() << "QGst::Structure";
    if (structure.isValid()) {
        debug.nospace() << "(" << structure.toString() << ")";
    } else {
        debug.nospace() << "(<invalid>)";
    }
    return debug.space();
}

} //namespace QGst
