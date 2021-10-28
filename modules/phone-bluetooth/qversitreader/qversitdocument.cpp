/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtVersit module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qversitdocument.h"
#include "qversitdocument_p.h"

#ifndef QT_NO_DEBUG_STREAM
#include <QtCore/qdebug.h>
#endif

namespace QtVersit {

/*!
  \class QVersitDocument
  \brief The QVersitDocument class is a container for a list of versit properties.
  \ingroup versit
  \inmodule QtVersit

  A vCard is represented in abstract form as a QVersitDocument that consists of a number of
  properties such as a name (N), a telephone number (TEL) and an email address (EMAIL), for
  instance.  Each of these properties is stored as an instance of a QVersitProperty in a
  QVersitDocument.

  In addition to the list of properties, QVersitDocument also records the type of the Versit
  document in two ways.  The VersitType enum describes the format in which the document is to be
  serialized by QVersitWriter (or the format from which it was read by QVersitReader), and should
  not be used to infer any semantics about the document data.  The componentType field is a string
  corresponding directly to the value of the BEGIN line in a document.  For example, for a vCard,
  this will always be the string "VCARD"; for an iCalendar, it could be "VCALENDAR", "VEVENT",
  "VTODO", "VJOURNAL", "VALARM" or "VTIMEZONE".

  As well as properties, a QVersitDocument can hold other documents.  For iCalendar, this is how
  a single VCALENDAR document can compose documents of type VEVENT, VTODO, etc.

  For example, for the following iCalendar:
  \code
  BEGIN:VCALENDAR
  VERSION:2.0
  BEGIN:VEVENT
  SUMMARY:Christmas
  DTSTART:20001225
  END:VEVENT
  END:VCALENDAR
  \endcode

  This can be represented as a QVersitDocument of with componentType VCALENDAR and versitType
  ICalendar20Type.  It contains no properties (note: the VERSION property is not stored explicitly
  as a property) and one sub-document.  The sub-document has componentType VEVENT and versitType
  ICalendar20Type, and contains two properties.

  QVersitDocument supports implicit sharing.

  \sa QVersitProperty
 */

/*!
  \enum QVersitDocument::VersitType
  This enum describes a Versit document serialization format and version.
  \value InvalidType No type specified or a document with an invalid type was parsed
  \value VCard21Type vCard version 2.1
  \value VCard30Type vCard version 3.0
  \value VCard40Type vCard version 4.0
  \value ICalendar20Type iCalendar version 2.0
 */

/*! Constructs a new empty document */
QVersitDocument::QVersitDocument() : d(new QVersitDocumentPrivate())
{
}

/*! Constructs a new empty document with the type set to \a type */
QVersitDocument::QVersitDocument(VersitType type) : d(new QVersitDocumentPrivate())
{
    d->mVersitType = type;
}


/*! Constructs a document that is a copy of \a other */
QVersitDocument::QVersitDocument(const QVersitDocument& other) : d(other.d)
{
}

/*! Frees the memory used by the document */
QVersitDocument::~QVersitDocument()
{
}

/*! Assigns this document to \a other */
QVersitDocument& QVersitDocument::operator=(const QVersitDocument& other)
{
    if (this != &other)
        d = other.d;
    return *this;
}

/*! Returns true if this is equal to \a other; false otherwise. */
bool QVersitDocument::operator==(const QVersitDocument& other) const
{
    return d->mVersitType == other.d->mVersitType &&
            d->mProperties == other.d->mProperties &&
            d->mSubDocuments == other.d->mSubDocuments &&
            d->mComponentType == other.d->mComponentType;
}

/*! Returns true if this is not equal to \a other; false otherwise. */
bool QVersitDocument::operator!=(const QVersitDocument& other) const
{
    return !(*this == other);
}

/*! Returns the hash value for \a key. */
size_t qHash(const QVersitDocument &key)
{
    size_t hash = QT_PREPEND_NAMESPACE(qHash)(key.type());
    hash += QT_PREPEND_NAMESPACE(qHash)(key.componentType());
    hash += key.properties().length() + key.subDocuments().length();
    foreach (const QVersitProperty& property, key.properties()) {
        hash += qHash(property);
    }
    foreach (const QVersitDocument& nested, key.subDocuments()) {
        hash += qHash(nested);
    }
    return hash;
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const QVersitDocument& document)
{
    dbg.nospace() << "QVersitDocument(" << document.type() << ", " << document.componentType() << ')';
    foreach (const QVersitProperty& property, document.properties()) {
        dbg.space() << '\n' << property;
    }
    foreach (const QVersitDocument& nested, document.subDocuments()) {
        dbg.space() << '\n' << nested;
    }
    return dbg.maybeSpace();
}
#endif

/*!
 * Sets the versit document type to \a type.  This determines the format in which the document is
 * to be serialized.
 */
void QVersitDocument::setType(VersitType type)
{
    d->mVersitType = type;
}

/*!
 * Gets the versit document type.
 */
QVersitDocument::VersitType QVersitDocument::type() const
{
    return d->mVersitType;
}

/*!
 * Sets the versit component type to \a componentType (eg. VCARD, VCALENDAR, VEVENT, etc.)
 */
void QVersitDocument::setComponentType(QString componentType)
{
    d->mComponentType = componentType;
}

/*!
 * Gets the versit component type
 */
QString QVersitDocument::componentType() const
{
    return d->mComponentType;
}

/*!
 * Add \a property to the list of contained versit properties.
 * The property is appended as the last property of the list.
 */
void QVersitDocument::addProperty(const QVersitProperty& property)
{
    d->mProperties.append(property);
}

/*!
 * Removes the property \a property from the versit document.
 */
void QVersitDocument::removeProperty(const QVersitProperty& property)
{
    d->mProperties.removeAll(property);
}

/*!
 * Removes all the properties with the given \a name from the versit document.
 */
void QVersitDocument::removeProperties(const QString& name)
{
    for (int i=d->mProperties.count()-1; i >=0; i--) {
        if (d->mProperties[i].name() == name) {
            d->mProperties.removeAt(i);
        }
    }
}

/*!
 * Sets the list of properties to \a properties.  Logically, all of the existing properties are
 * removed and all of the supplied \a properties are added.
 */
void QVersitDocument::setProperties(const QList<QVersitProperty>& properties)
{
    d->mProperties = properties;
}

/*!
 * Gets the list of the contained versit properties.
 * Note that the actual properties cannot be modified using the copy.
 */
QList<QVersitProperty> QVersitDocument::properties() const
{
    return d->mProperties;
}

/*!
 * Adds \a subdocument to the Versit document.
 */
void QVersitDocument::addSubDocument(const QVersitDocument& subdocument)
{
    d->mSubDocuments.append(subdocument);
}

/*!
 * Removes the \a subdocument from the versit document.
 */
void QVersitDocument::removeSubDocument(const QVersitDocument& subdocument)
{
    d->mSubDocuments.removeAll(subdocument);
}

/*!
 * Sets the list of subdocuments to \a documents.
 */
void QVersitDocument::setSubDocuments(const QList<QVersitDocument>& documents)
{
    d->mSubDocuments = documents;
}

/*!
 * Returns the list of subdocuments contained within this Versit document.
 */
QList<QVersitDocument> QVersitDocument::subDocuments() const
{
    return d->mSubDocuments;
}

/*!
 * Returns true if the document is empty.
 */
bool QVersitDocument::isEmpty() const
{
    return d->mProperties.isEmpty()
        && d->mSubDocuments.isEmpty()
        && d->mVersitType == QVersitDocument::InvalidType;
}

/*!
 * Clears the document, removing all properties, sub-documents and metadata.
 */
void QVersitDocument::clear()
{
    d->mProperties.clear();
    d->mSubDocuments.clear();
    d->mVersitType = QVersitDocument::InvalidType;
    d->mComponentType.clear();
}

}
