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

#ifndef QVERSITUTILS_P_H
#define QVERSITUTILS_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/qbytearray.h>
#include <QtCore/qbytearraymatcher.h>
#include <QtCore/qlist.h>
#include <QtCore/QMutex>

#include "qversitproperty.h"

QT_FORWARD_DECLARE_CLASS(QString)
QT_FORWARD_DECLARE_CLASS(QTextCodec)
QT_FORWARD_DECLARE_CLASS(QVariant)

namespace QtVersit {

class QVersitDocument;

class Q_DECL_IMPORT VersitUtils
{
public:
    static QByteArray encode(const QByteArray& ba, QTextCodec* codec);
    static QByteArray encode(char ch, QTextCodec* codec);
    static QList<QByteArrayMatcher>* newlineList(QTextCodec* codec);
    static void changeCodec(QTextCodec* codec);

    static bool isValidUtf8(const QByteArray& bytes);
    static bool convertToJson(const QVariant &data, QString *json);
    static bool convertFromJson(const QString &json, QVariant *data);

private:
    // These are caches for performance:
    // The previous codec that encode(char, QTextCodec) was called with
    static QTextCodec* m_previousCodec;
    // The QByteArray corresponding to each char from 0-255, encoded with m_previousCodec
    static QByteArray m_encodingMap[256];
    // List of different newline delimeters, encoded with m_previousCodec
    static QList<QByteArrayMatcher>* m_newlineList;
    static QBasicMutex m_staticLock;
};

}

#endif // QVERSITUTILS_P_H
