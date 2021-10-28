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

#ifndef QVERSITDOCUMENT_H
#define QVERSITDOCUMENT_H

#include <QtCore/qlist.h>
#include <QtCore/qshareddata.h>

//#include <QtVersit/qversitdocument.h>
#include "qversitproperty.h"

namespace QtVersit {

class QVersitProperty;

class QVersitDocumentPrivate;
class Q_DECL_IMPORT QVersitDocument
{
public:
    enum VersitType {
        InvalidType,
        VCard21Type,    // vCard version 2.1
        VCard30Type,    // vCard version 3.0 (RFC 2426)
        VCard40Type,    // vCard version 4.0
        ICalendar20Type // iCalendar version 2.0 (RFC 2445/5545)
    };

    QVersitDocument();
    QVersitDocument(const QVersitDocument& other);
    QVersitDocument(VersitType type);
    ~QVersitDocument();

    QVersitDocument& operator=(const QVersitDocument& other);
    bool operator==(const QVersitDocument& other) const;
    bool operator!=(const QVersitDocument& other) const;

    // Metadata about the versit document
    // The type determines the format for serialization
    void setType(VersitType type);
    VersitType type() const;

    // The componentType is the value of the BEGIN property
    void setComponentType(QString componentType);
    QString componentType() const;

    // The content
    void addProperty(const QVersitProperty& property);
    void removeProperty(const QVersitProperty& property);
    void removeProperties(const QString& name);
    void setProperties(const QList<QVersitProperty>& properties);
    QList<QVersitProperty> properties() const;

    void addSubDocument(const QVersitDocument& subdocument);
    void removeSubDocument(const QVersitDocument& subdocument);
    void setSubDocuments(const QList<QVersitDocument>& documents);
    QList<QVersitDocument> subDocuments() const;

    bool isEmpty() const;
    void clear();

private:

    QSharedDataPointer<QVersitDocumentPrivate> d;
};

Q_DECL_IMPORT size_t qHash(const QVersitDocument& key);
#ifndef QT_NO_DEBUG_STREAM
Q_DECL_IMPORT QDebug operator<<(QDebug dbg, const QVersitDocument& property);
#endif

}

Q_DECLARE_METATYPE(::QtVersit::QVersitDocument)

#endif // QVERSITDOCUMENT_H
