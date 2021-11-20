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

#ifndef QVERSITPROPERTY_H
#define QVERSITPROPERTY_H

#include <QtCore/qhash.h>
#include <QtCore/qshareddata.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qvariant.h>

namespace QtVersit {

class QVersitPropertyPrivate;
class Q_DECL_IMPORT QVersitProperty
{
public:
    enum ValueType {
        PlainType,
        CompoundType,
        ListType,
        BinaryType,
        VersitDocumentType,
        PreformattedType
    };

    QVersitProperty();
    QVersitProperty(const QVersitProperty& other);
    ~QVersitProperty();

    QVersitProperty& operator=(const QVersitProperty& other);
    bool operator==(const QVersitProperty& other) const;
    bool operator!=(const QVersitProperty& other) const;

    void setGroups(const QStringList& groups);
    QStringList groups() const;

    void setName(const QString& name);
    QString name() const;

    void insertParameter(const QString& name, const QString& value);
    void removeParameter(const QString& name, const QString& value);
    void removeParameters(const QString& name);

    void setParameters(const QMultiHash<QString,QString>& parameters);
    QMultiHash<QString,QString> parameters() const;

    void setValue(const QVariant& value);
    QVariant variantValue() const;
    template <typename T> T value() const
    {
        return variantValue().value<T>();
    }
    QString value() const;

    void setValueType(ValueType type);
    ValueType valueType() const;

    bool isEmpty() const;
    void clear();

private:

    QSharedDataPointer<QVersitPropertyPrivate> d;
};

Q_DECL_IMPORT size_t qHash(const QVersitProperty& key);
#ifndef QT_NO_DEBUG_STREAM
Q_DECL_IMPORT QDebug operator<<(QDebug dbg, const QVersitProperty& property);
#endif

}

#endif // QVERSITPROPERTY_H
