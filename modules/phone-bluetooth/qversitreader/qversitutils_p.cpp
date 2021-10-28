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

#include "qversitutils_p.h"

#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>

#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonValue>

#include <QTextCodec>


namespace QtVersit {

QTextCodec* VersitUtils::m_previousCodec = 0;
QList<QByteArrayMatcher>* VersitUtils::m_newlineList = 0;
QByteArray VersitUtils::m_encodingMap[256];
QBasicMutex VersitUtils::m_staticLock;

/*!
 * Encode \a ch with \a codec, without adding an byte-order mark
 */
QByteArray VersitUtils::encode(char ch, QTextCodec* codec)
{
    changeCodec(codec);
    return m_encodingMap[(int)ch];
}

/*!
 * Encode \a ba with \a codec, without adding an byte-order mark.  \a ba is interpreted as ASCII
 */
QByteArray VersitUtils::encode(const QByteArray& ba, QTextCodec* codec)
{
    QTextCodec::ConverterState state(QTextCodec::IgnoreHeader);
    return codec->fromUnicode(QString::fromLatin1(ba.data()).data(), ba.length(), &state);
}

/*!
 * Returns the list of DOS, UNIX and Mac newline characters for \a codec.
 */
QList<QByteArrayMatcher>* VersitUtils::newlineList(QTextCodec* codec)
{
    changeCodec(codec);
    return m_newlineList;
}

/*!
 * Update the cached tables of pregenerated encoded text with \a codec.
 */
void VersitUtils::changeCodec(QTextCodec* codec) {
    QMutexLocker readWriterLocker(&VersitUtils::m_staticLock);

    if (VersitUtils::m_newlineList != 0 && codec == VersitUtils::m_previousCodec)
        return;

    // Build m_encodingMap
    QChar qch;
    QTextCodec::ConverterState state(QTextCodec::IgnoreHeader);
    for (int c = 0; c < 256; c++) {
        qch = QLatin1Char(c);
        m_encodingMap[c] = codec->fromUnicode(&qch, 1, &state);
    }

    // Build m_newlineList
    if (m_newlineList != 0)
        delete m_newlineList;
    m_newlineList = new QList<QByteArrayMatcher>;
    m_newlineList->append(QByteArrayMatcher(encode("\r\n", codec)));
    m_newlineList->append(QByteArrayMatcher(encode("\n", codec)));
    m_newlineList->append(QByteArrayMatcher(encode("\r", codec)));

    m_previousCodec = codec;
}

/*!
 * Returns true iff \a bytes is a valid UTF-8 sequence.
 */
bool VersitUtils::isValidUtf8(const QByteArray& bytes) {
    int sequenceLength = 1; // number of bytes in total for a sequence
    int continuation = 0;   // number of bytes left in a continuation
    quint32 codePoint = 0;
    for (int i = 0; i < bytes.size(); i++) {
        quint8 byte = bytes[i];
        if (continuation == 0) {
            if (byte & 0x80) { // 1xxxxxxx
                if (byte & 0x40) { // 11xxxxxx
                    if (byte == 0xc0 || byte == 0xc1) // 1100000x
                        return false; // overlong 2 byte sequence
                    if (byte & 0x20) { // 111xxxxx
                        if (byte & 0x10) { // 1111xxxx
                            if (byte & 0x08) { // 11111xxx
                                // Outside unicode range
                                return false;
                            } else { // 11110xxx
                                sequenceLength = 4;
                                continuation = 3; // three more bytes
                                codePoint = byte & 0x07; // take the last 3 bits
                            }
                        } else { // 1110xxxx
                            sequenceLength = 3;
                            continuation = 2; // two more bytes
                            codePoint = byte & 0x0f; // take last 4 bits
                        }
                    } else { // 110xxxxx
                        sequenceLength = 2;
                        continuation = 1; // one more byte
                        codePoint = byte & 0x1f; // take last 5 bits
                    }
                } else { // 10xxxxxx
                    // unexpected continuation
                    return false;
                }
            } else { // 0xxxxxxx
                sequenceLength = 1;
            }
        } else { // continuation > 0
            if ((byte & 0xc0) != 0x80) // 10xxxxxx
                return false; // expected continuation not found
            codePoint = (codePoint << 6) | (byte & 0x3f); // append last 6 bits
            continuation--;
        }

        if (continuation == 0) {
            // Finished decoding a character - it's not overlong and that it's in range
            switch (sequenceLength) {
            // 1-byte sequence can't be overlong
            // 2-byte sequence has already been checked for overlongness
            case 3:
                if (codePoint < 0x800) // overlong
                    return false;

                // Filter out codepoints outside the Unicode range
                if ((codePoint >= 0xd800 && codePoint <= 0xdfff) // utf-16 surrogate halves
                    || (codePoint >= 0xfffe && codePoint <= 0xffff)) { // reversed utf-16 BOM
                    return false;
                }
                break;
            case 4:
                if (codePoint < 0x10000      // overlong
                    || codePoint > 0x10ffff) // above Unicode range
                    return false;
                break;
            }
            codePoint = 0;
        }
    }
    return continuation == 0;
}

/*!
 * Convert variant \a data to string \a json in JSON format.
 *
 * The data is encoded as an array containing one item
 * to allow the same encoding to be used for both
 * primitive and compound data types.
 *
 * Returns true if the conversion is successful, false otherwise.
 *
 * \sa convertFromJson()
 */
bool VersitUtils::convertToJson(const QVariant &data, QString *json)
{
    const QJsonValue dataAsJsonValue = QJsonValue::fromVariant(data);
    if (data.isValid() && dataAsJsonValue.isNull())
        return false;
    QJsonArray jsonArray;
    jsonArray.append(dataAsJsonValue);
    const QJsonDocument jsonDocument(jsonArray);
    *json = QString::fromUtf8(jsonDocument.toJson());
    return true;
}

/*!
 * Convert string \a json in JSON format to variant \a data.
 *
 * The format of the json string is assumed to be a one-item array.
 *
 * Returns true if the conversion is successful, false otherwise.
 *
 * \sa convertToJson()
 */
bool VersitUtils::convertFromJson(const QString &json, QVariant *data)
{
    const QJsonDocument jsonDoc = QJsonDocument::fromJson(json.toUtf8());
    const QJsonValue jsonValue = jsonDoc.array().at(0);
    if (jsonValue.isUndefined())
        return false;
    *data = jsonValue.toVariant();
    return true;
}

}
