
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

#ifndef QVERSITREADER_P_H
#define QVERSITREADER_P_H

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
#include <QtCore/qhash.h>
#include <QtCore/qlist.h>
#include <QtCore/qmutex.h>
#include <QtCore/qpair.h>
#include <QtCore/qpointer.h>
#include <QtCore/qscopedpointer.h>
#include <QtCore/qstack.h>
#include <QtCore/qthread.h>

#include "qversitreader.h"
#include "qversitdocument.h"
#include "qversitproperty.h"

QT_FORWARD_DECLARE_CLASS(QBuffer)
QT_FORWARD_DECLARE_CLASS(QIODevice)
QT_FORWARD_DECLARE_CLASS(QTextCodec)

namespace QtVersit {

// The maximum number of bytes allowed to stay in memory after being read.  The smaller this is,
// the more time spent moving bytes around.  The larger it is, the more memory is wasted.
static const int MAX_OLD_BYTES_TO_KEEP = 8192;

/*
 * An LByteArray has a subset of QByteArray's interface, plus an efficient chopLeft function
 *
 * It stores a QByteArray internally, plus a marker of where it starts and where it ends.
 */
class LByteArray
{
public:
    LByteArray() : mStart(0), mEnd(0) {}
    explicit LByteArray(const QByteArray& d) :mData(d), mStart(0), mEnd(d.size()) {}
    LByteArray(const QByteArray& d, int start, int end) :mData(d), mStart(start), mEnd(end) {}
    bool isEmpty() const {
        return mEnd <= mStart;
    }
    char at(int i) const {
        return mData.at(mStart + i);
    }
    QByteArray toByteArray() const {
        return mData.mid(mStart, mEnd-mStart);
    }
    /* Removes \a n bytes from the start of the QByteArray. */
    void chopLeft(int n) {
        Q_ASSERT(size() >= n && n >= 0);
        mStart += n;
    }
    QByteArray left(int n) {
        Q_ASSERT(size() >= n && n >= 0);
        return mData.mid(mStart, n);
    }
    int indexOf(const QByteArray& needle) {
        int index = mData.indexOf(needle, mStart) - mStart;
        if (index < size())
            return index;
        return -1;
    }
    int size() const {
        return mEnd - mStart;
    }
    const char* constData() const {
        return mData.constData() + mStart;
    }
    bool contains(const QByteArray& ba) const {
        int i = mData.indexOf(ba, mStart);
        return i > 0 && i <= mEnd - ba.length();
    }
    bool endsWith(const QByteArray& ba) const {
        // Loop backwards from ba and from mData (starting from index mEnd)
        if (ba.size() > size())
            return false;
        return memcmp(mData.constData()+mEnd-ba.size(), ba.constData(), ba.size()) == 0;
    }
    LByteArray& operator=(const QByteArray& ba) {
        mData = ba;
        mStart = 0;
        mEnd = mData.size();
        return *this;
    }
    bool operator==(const QByteArray& ba) {
        return toByteArray() == ba;
    }
    bool operator!=(const QByteArray& ba) {
        return toByteArray() != ba;
    }

private:
    /* Clears the memory of bytes before the start marker */
    void dropOldData() {
        if (mStart > MAX_OLD_BYTES_TO_KEEP && mEnd >= mStart) {
            mData.remove(0, mStart);
            mEnd -= mStart;
            mStart = 0;
        }
    }
    void setBounds(int start, int end) {
        mStart = start;
        mEnd = end;
    }
    QByteArray mData;
    int mStart;
    int mEnd;
    friend class LineReader;
};

class LineReader
{
public:
    LineReader(QIODevice* device, QTextCodec* codec);
    LineReader(QIODevice* device);
    LineReader(QIODevice* device, QTextCodec* codec, int chunkSize);
    void init();
    void pushLine(const QByteArray& line);
    int odometer() const;
    bool atEnd() const;
    QTextCodec* codec() const;
    bool isCodecCertain() const;
    bool isCodecUtf8Compatible() const;
    void setCodecUtf8Incompatible();
    LByteArray readLine();

private:
    void readOneLine(LByteArray* cursor);
    bool tryReadLine(LByteArray* cursor, bool atEnd);

    QIODevice* const mDevice;
    QTextCodec* mCodec;
    bool mIsCodecCertain;
    bool mIsCodecUtf8Compatible;
    int mChunkSize; // How many bytes to read in one go.
    QStack<QByteArray> mPushedLines; // Stores a lines that has been "pushed" in front by pushLine
    LByteArray mBuffer;
    int mOdometer;
    int mSearchFrom;
};

class QVersitReaderPrivate : public QThread
{
    Q_OBJECT

public: // Constructors and destructor
    QVersitReaderPrivate();
    ~QVersitReaderPrivate();

    static QHash<QPair<QVersitDocument::VersitType,QString>, QVersitProperty::ValueType>*
    valueTypeMap();
    void init(QVersitReader* reader);

signals:
    void stateChanged(QVersitReader::State state);
    void resultsAvailable();

protected: // From QThread
    void run() override;

public: // New functions
    void read();

    // mutexed getters and setters.
    void setState(QVersitReader::State);
    QVersitReader::State state() const;
    void setError(QVersitReader::Error);
    QVersitReader::Error error() const;
    void setCanceling(bool cancelling);
    bool isCanceling();

    bool parseVersitDocument(LineReader* lineReader, QVersitDocument* document);
    bool parseVersitDocumentBody(LineReader* lineReader, QVersitDocument* document);

    QVersitProperty parseNextVersitProperty(
        QVersitDocument::VersitType versitType,
        LineReader* lineReader);

    void parseVCard21Property(
        LByteArray* text,
        QVersitProperty* property,
        LineReader* lineReader);

    void parseVCard30Property(
        QVersitDocument::VersitType versitType,
        LByteArray* text,
        QVersitProperty* property,
        LineReader* lineReader);

    bool setVersionFromProperty(
        QVersitDocument* document,
        const QVersitProperty& property) const;

    bool unencode(
        QByteArray* value,
        QVersitProperty* property,
        LineReader* lineReader) const;

    QString decodeCharset(
        const QByteArray& value,
        QVersitProperty* property,
        LineReader* lineReader,
        QTextCodec** codec) const;

    void decodeQuotedPrintable(QByteArray* text) const;


    /* These functions operate on a cursor describing a single line */
    QPair<QStringList,QString> extractPropertyGroupsAndName(LByteArray* line, QTextCodec* codec)
        const;
    QMultiHash<QString,QString> extractVCard21PropertyParams(LByteArray* line, QTextCodec* codec)
        const;
    QMultiHash<QString,QString> extractVCard30PropertyParams(LByteArray* line, QTextCodec* codec)
        const;

    // "Private" functions
    QList<QByteArray> extractParams(LByteArray* line, QTextCodec *codec) const;
    QList<QByteArray> extractParts(const QByteArray& text, const QByteArray& separator,
                                   QTextCodec *codec) const;
    QByteArray extractPart(const QByteArray& text, int startPosition, int length=-1) const;
    QString paramName(const QByteArray& parameter, QTextCodec* codec) const;
    QString paramValue(const QByteArray& parameter, QTextCodec* codec) const;
    template <class T> static bool containsAt(const T& text, const QByteArray& ba, int index);
    bool splitStructuredValue(QVersitProperty* property,
                              bool hasEscapedBackslashes) const;
    static QStringList splitValue(const QString& string,
                                  const QChar& sep,
                                  QString::SplitBehavior behavior,
                                  bool hasEscapedBackslashes);
    static void removeBackSlashEscaping(QString* text);

    // Data
public:
    QPointer<QIODevice> mIoDevice;
    QScopedPointer<QBuffer> mInputBytes; // Holds the data set by setData()
    QList<QVersitDocument> mVersitDocuments;
    int mDocumentNestingLevel; // Depth in parsing nested Versit documents
    QTextCodec* mDefaultCodec;
    QVersitReader::State mState;
    QVersitReader::Error mError;
    bool mIsCanceling;
    mutable QMutex mMutex;

private:
    /* key is the document type and property name, value is the type of property it is.
       If there is no entry, assume it is a PlainType */
    static QHash<QPair<QVersitDocument::VersitType,QString>, QVersitProperty::ValueType>* mValueTypeMap;
};

}

#endif // QVERSITREADER_P_H
