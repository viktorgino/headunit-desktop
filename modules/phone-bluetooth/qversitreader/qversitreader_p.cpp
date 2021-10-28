
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

#include "qversitreader_p.h"

#include <QtCore/qregularexpression.h>
#include <QtCore/qbuffer.h>
#include <QtCore/qvariant.h>

#include <QTextCodec>

#include "qversitutils_p.h"

namespace QtVersit {

// Some big enough value for nested versit documents to prevent infinite recursion
#define MAX_VERSIT_DOCUMENT_NESTING_DEPTH 20

QHash<QPair<QVersitDocument::VersitType,QString>, QVersitProperty::ValueType>*
    QVersitReaderPrivate::mValueTypeMap = 0;

/*!
  \class LineReader
  \brief The LineReader class is a wrapper around a QIODevice that allows line-by-line reading.
  \internal

  This class keeps an internal buffer which it uses to temporarily store data which it has read from
  the device but not returned to the user.

  The isCodecCertain constructor parameter/getter can be used by the client to indicate whether
  the codec supplied is known for sure, or if it was a guess.
 */

/*!
  Constructs a LineReader that reads from the given \a device using the given \a codec.
  If the \a codec is null, it is guessed at by sniffing the first few bytes of the input.
  */
LineReader::LineReader(QIODevice* device, QTextCodec *codec)
    : mDevice(device),
    mCodec(codec),
    mIsCodecUtf8Compatible(false),
    mChunkSize(10000), // Read 10kB at a time
    mOdometer(0),
    mSearchFrom(0)
{
    if (!mCodec) {
        static QTextCodec* utf16be = QTextCodec::codecForName("UTF-16BE");
        static QTextCodec* utf16le = QTextCodec::codecForName("UTF-16LE");
        static QTextCodec* utf32be = QTextCodec::codecForName("UTF-32BE");
        static QTextCodec* utf32le = QTextCodec::codecForName("UTF-32LE");
        static const QByteArray beginUtf16be(VersitUtils::encode("BEGIN:", utf16be));
        static const QByteArray beginUtf16le(VersitUtils::encode("BEGIN:", utf16le));
        static const QByteArray beginUtf32be(VersitUtils::encode("BEGIN:", utf32be));
        static const QByteArray beginUtf32le(VersitUtils::encode("BEGIN:", utf32le));

        // Do some basic charset detection using the byte-order-mark (BOM)
        // We need 4 bytes to do BOM sniffing for UTF-32, UTF-16 and UTF-8
        QByteArray firstSixBytes = mDevice->read(6);
        mCodec = QTextCodec::codecForUtfText(firstSixBytes, NULL);
        if (mCodec) {
            mIsCodecCertain = true;
        } else {
            if (beginUtf16be.startsWith(firstSixBytes)) {
                mCodec = utf16be;
                mIsCodecCertain = true;
            } else if (beginUtf16le.startsWith(firstSixBytes)) {
                mCodec = utf16le;
                mIsCodecCertain = true;
            } else if (beginUtf32be.startsWith(firstSixBytes)) {
                mCodec = utf32be;
                mIsCodecCertain = true;
            } else if (beginUtf32le.startsWith(firstSixBytes)) {
                mCodec = utf32le;
                mIsCodecCertain = true;
            } else {
                mCodec = QTextCodec::codecForLocale();
                mIsCodecCertain = false;
                mIsCodecUtf8Compatible = true;
            }
        }
        mBuffer = LByteArray(firstSixBytes, 0, 0);
    } else {
        mIsCodecCertain = true;
    }
}

/*!
  Constructs a LineReader that reads from the given \a device using the given \a codec.
  \a chunkSize is the number of bytes to read at a time (it is useful for testing but this
  constructor shouldn't otherwise be used).
  */
LineReader::LineReader(QIODevice* device, QTextCodec *codec, int chunkSize)
    : mDevice(device),
    mCodec(codec),
    mIsCodecCertain(true),
    mChunkSize(chunkSize),
    mOdometer(0),
    mSearchFrom(0)
{
    Q_ASSERT(mCodec != NULL);
}

/*!
  Attempts to read a line and returns an LByteArray containing the line.
  This wraps around readOneLine and provides a hack to do additional unwrapping for a malformed
  vCard where a space is not added to the start of the line continuation.

  Some malformed vCards we get look like this: (Case 1)
  ORG:A
   B
  C
  (CRLF-SPACE wrapping is employed for the first time, then the space is subsequently omitted).
  But a valid vCard can be weirdly wrapped without the CRLF-SPACE, if it's quoted-printable and
  ends in an equals, eg. (Case 2)
  ORG;ENCODING=QUOTED-PRINTABLE:A=
  B=
  C
  Unwrap in Case 1 but not in Case 2 - leave that for the QP-decoder in QVR::unencode
  */
LByteArray LineReader::readLine()
{
    QByteArray colon(VersitUtils::encode(':', mCodec));
    QByteArray equals(VersitUtils::encode('=', mCodec));
    if (!mPushedLines.isEmpty()) {
        LByteArray retval(mPushedLines.pop());
        return retval;
    }
    readOneLine(&mBuffer);
    // Hack: read the next line and see if it's a continuation of this line
    while (true) {
        int prevStart = mBuffer.mStart;
        int prevEnd = mBuffer.mEnd;
        // readOneLine only appends to mBuffer so these saved offsets should remain valid
        readOneLine(&mBuffer);

        // Get an LByteArray of the previous line.  This should be fast because copying the
        // LByteArray copies the QByteArray, which is implicitly shared
        LByteArray prevLine(mBuffer.mData, prevStart, prevEnd);
        if (mBuffer.isEmpty()
                || mBuffer.contains(colon)
                || prevLine.endsWith(equals)) {
            // Normal, the next line is empty, or a new property, or it's been wrapped using
            // QUOTED-PRINTABLE.  Rewind it back one line so it gets read next time round.
            mBuffer.setBounds(prevStart, prevEnd);
            break;
        } else {
            // Some silly vCard generator has probably wrapped a line without prepending a space
            // Join the previous line with this line by deleting the characters between prevEnd and
            // mStart (eg. any newline characters)
            int crlfLen = mBuffer.mStart-prevEnd;
            mBuffer.mData.remove(prevEnd, crlfLen);
            mBuffer.setBounds(prevStart, mBuffer.mEnd - crlfLen);
        }
    }
    mBuffer.dropOldData();
    mOdometer += mBuffer.size();
    return mBuffer;
}

/*!
  Attempts to read a line and updates \a cursor to contain the line.  This performes basic
  line unwrapping as per the vCard specification (eg. if a line begins with a space, it is a
  continuation of the next line)
  */
void LineReader::readOneLine(LByteArray* cursor) {
    QByteArray cr(VersitUtils::encode('\r', mCodec));
    QByteArray lf(VersitUtils::encode('\n', mCodec));
    QByteArray crlf(VersitUtils::encode("\r\n", mCodec));

    cursor->mStart = cursor->mEnd;
    mSearchFrom = cursor->mStart;

    // First, look for a newline in the already-existing buffer.  If found, return the line.
    if (tryReadLine(cursor, false)) {
        return;
    }

    // Otherwise, keep reading more data until either a CRLF is found, or there's no more to read.
    while (!mDevice->atEnd()) {
        QByteArray temp = mDevice->read(mChunkSize);
        if (!temp.isEmpty()) {
            cursor->mData.append(temp);

            // Sanitise CRLF before proceeding to handle mixed line endings.
            // Convert the other two possible newline representations to '\n'.
            cursor->mData.replace(crlf, lf);
            if (cursor->mData.endsWith(cr)) {
                // Corner case: line ends with \r - could also be \r\n after the
                // next read, so do not replace the last occurrence (yet)
                // So restore it for now.
                cursor->mData.replace(cr, lf);
                cursor->mData.truncate(cursor->mData.length() - lf.length());
                cursor->mData.append(cr);
            } else {
                cursor->mData.replace(cr, lf);
            }

            if (tryReadLine(cursor, false))
                return;

        } else {
            mDevice->waitForReadyRead(500);
        }
    }

    // We've reached the end of the stream.  Find a newline from the buffer (or return what's left).
    // But first, strip the last occurrence of \r - if present - left from before.
    if (cursor->mData.endsWith(cr))
        cursor->mData.truncate(cursor->mData.length() - cr.length());

    tryReadLine(cursor, true);

    return;
}

/*!
  Push a line onto the front of the line reader so it will be returned on the next call to readLine().
  If multiple lines are pushed onto a line reader, they are read back in first-in-last-out order
  */
void LineReader::pushLine(const QByteArray& line)
{
    mPushedLines.push(line);
}

/*!
  How many bytes have been returned in the LByteArray in the lifetime of the LineReader.
 */
int LineReader::odometer() const
{
    return mOdometer;
}

/*!
  Returns true if there are no more lines left for readLine() to return.  It is possible for atEnd()
  to return false and for there to be no more data left (eg. if there are trailing newlines at the
  end of the input.  In this case, readLine() will return an empty line.
 */
bool LineReader::atEnd() const
{
    return mPushedLines.isEmpty() && mDevice->atEnd() && mBuffer.mEnd == mBuffer.mData.size();
}

/*!
  Returns the codec that the LineReader reads with.
 */
QTextCodec* LineReader::codec() const
{
    return mCodec;
}

/*!
  Returns true if the line reader has been told for sure what the codec is, or if a byte-order-mark
  has told us for sure what the codec is.
 */
bool LineReader::isCodecCertain() const
{
    return mIsCodecCertain;
}

/*! Valid if isCodecCertain(), false iff we've seen an invalid utf8 sequence */
bool LineReader::isCodecUtf8Compatible() const {
    return mIsCodecUtf8Compatible;
}

void LineReader::setCodecUtf8Incompatible() {
    mIsCodecUtf8Compatible = false;
}

/*!
 * Get the next line of input from the device to parse.  Also performs unfolding by removing
 * sequences of newline-space from the retrieved line.  Skips over any newlines at the start of the
 * input.
 *
 * \a cursor filled with the line to parse
 * \a atEnd is true if we've reached the end of the stream
 * Returns true if a line was completely read (ie. a newline character was found)
 *
 * Expects all newline sequences (\r\n, \r and \n) already changed to \n (referred as NL or 'newline'
 * in the code) by the caller.
 */
bool LineReader::tryReadLine(LByteArray *cursor, bool atEnd)
{
    int nlPos = -1;
    int doubleNlCheck = -1;

    QByteArray nl(VersitUtils::encode('\n', mCodec));
    QByteArray tab(VersitUtils::encode('\t', mCodec));
    QByteArray space(VersitUtils::encode(' ', mCodec));
    QByteArray equals(VersitUtils::encode('=', mCodec));

    int nlLength = nl.length();
    int spaceLength = space.length();
    int equalsLength = equals.length();

    forever {
        nlPos = cursor->mData.indexOf(nl, mSearchFrom);
        doubleNlCheck = cursor->mData.indexOf(nl, mSearchFrom + nlLength);
        if ((nlPos == cursor->mStart) && (doubleNlCheck != nlPos + nlLength)) {
            // Single newline at start of line - ignore and set mStart to directly after it.
            cursor->mStart += nlLength;
            mSearchFrom = cursor->mStart;
            continue;
        } else if ((nlPos == cursor->mStart) && (doubleNlCheck == nlPos + nlLength)) {
            // Found '=NLNL' - we choose to see this as badly formed,
            // but clearly marks the end of the versit property.
            cursor->mData.remove(nlPos, nlLength);
            cursor->mEnd = nlPos;
            if (QVersitReaderPrivate::containsAt(cursor->mData, equals, nlPos - equalsLength) ) {
                cursor->mData.remove(nlPos - 1, 1);
            }
            return true;
        } else if (nlPos > cursor->mStart) {
            // Found the first occurrence of newline in the current buffer.
            if (QVersitReaderPrivate::containsAt(cursor->mData, space, nlPos + nlLength)
                || QVersitReaderPrivate::containsAt(cursor->mData, tab, nlPos + nlLength)) {
                // If it's followed by whitespace, collapse it.
                cursor->mData.remove(nlPos, nlLength + spaceLength);
                mSearchFrom = nlPos;
                continue;
            } else if (!atEnd && nlPos + nlLength + spaceLength >= cursor->mData.size()) {
                // If our newline is at the end of the current buffer but there's more to read,
                // it's possible that a space could be hiding on the next read from the device.
                // Just pretend we didn't see the newline and pick it up the next time round.
                mSearchFrom = nlPos;
                return false;
            } else {
                // Found the newline.
                // Hack: if malformed vCard files (having no NL or NLNL ending) are
                // concatenated, we can get a malformed line in the document which looks like:
                // END:VCARDBEGIN:VCARD
                // In that situation, we should actually insert the newline sequence manually,
                // and return mEnd after the END:VCARD + NL position.
                QByteArray ev(VersitUtils::encode(QByteArray("END:VCARD"), mCodec));
                QByteArray evbv(VersitUtils::encode(QByteArray("END:VCARDBEGIN:VCARD"), mCodec));
                QByteArray evnl(VersitUtils::encode(QByteArray("END:VCARD\n"), mCodec));

                QByteArray possiblyMalformedLine = cursor->mData.mid(cursor->mStart, nlPos-cursor->mStart);
                int pmlEnd = possiblyMalformedLine.size() - 1;
                while (true) {
                    if (QVersitReaderPrivate::containsAt(possiblyMalformedLine, nl, pmlEnd - nlLength))
                        possiblyMalformedLine.chop(nlLength);
                    else
                        break;
                }
                if (possiblyMalformedLine == evbv) {
                    // fix up the malformed line, return the end cursor after it.
                    cursor->mData.replace(cursor->mStart, ev.length(), evnl);
                    cursor->mEnd = cursor->mStart + evnl.length();
                } else {
                    // A well-formed line.
                    cursor->mEnd = nlPos;
                }

                return true;
            }
        }
        if (nlPos == -1) {
            // No newline found.
            cursor->mEnd = cursor->mData.size();
            // Next time, continue searching from here.
            // The largest newline will have a size of 4 bytes, so we should backtrack 4 bytes
            mSearchFrom = qMax(mSearchFrom, cursor->mEnd - 4);
            return false;
        }
    }
}

/*! Links the signals from this to the signals of \a reader. */
void QVersitReaderPrivate::init(QVersitReader* reader)
{
    qRegisterMetaType<QVersitReader::State>("QVersitReader::State");
    connect(this, SIGNAL(stateChanged(QVersitReader::State)),
            reader, SIGNAL(stateChanged(QVersitReader::State)),Qt::DirectConnection);
    connect(this, SIGNAL(resultsAvailable()),
            reader, SIGNAL(resultsAvailable()), Qt::DirectConnection);
}

/*! Construct a reader. */
QVersitReaderPrivate::QVersitReaderPrivate()
    : mIoDevice(0),
    mDocumentNestingLevel(0),
    mDefaultCodec(0),
    mState(QVersitReader::InactiveState),
    mError(QVersitReader::NoError),
    mIsCanceling(false)
{
}

/*! Destroy a reader. */
QVersitReaderPrivate::~QVersitReaderPrivate()
{
}

QHash<QPair<QVersitDocument::VersitType,QString>, QVersitProperty::ValueType>*
QVersitReaderPrivate::valueTypeMap() {
    if (mValueTypeMap == 0) {
        mValueTypeMap = new QHash<QPair<QVersitDocument::VersitType,QString>, QVersitProperty::ValueType>();
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard21Type, QString::fromLatin1("AGENT")),
                             QVersitProperty::VersitDocumentType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard30Type, QString::fromLatin1("AGENT")),
                             QVersitProperty::VersitDocumentType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard40Type, QString::fromLatin1("AGENT")),
                             QVersitProperty::VersitDocumentType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard21Type, QString::fromLatin1("N")),
                             QVersitProperty::CompoundType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard30Type, QString::fromLatin1("N")),
                             QVersitProperty::CompoundType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard40Type, QString::fromLatin1("N")),
                             QVersitProperty::CompoundType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard21Type, QString::fromLatin1("ADR")),
                             QVersitProperty::CompoundType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard30Type, QString::fromLatin1("ADR")),
                             QVersitProperty::CompoundType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard40Type, QString::fromLatin1("ADR")),
                             QVersitProperty::CompoundType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard21Type, QString::fromLatin1("GEO")),
                             QVersitProperty::CompoundType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard30Type, QString::fromLatin1("GEO")),
                             QVersitProperty::CompoundType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard40Type, QString::fromLatin1("GEO")),
                             QVersitProperty::CompoundType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard21Type, QString::fromLatin1("ORG")),
                             QVersitProperty::CompoundType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard30Type, QString::fromLatin1("ORG")),
                             QVersitProperty::CompoundType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard40Type, QString::fromLatin1("ORG")),
                             QVersitProperty::CompoundType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard21Type, QString::fromLatin1("NICKNAME")),
                             QVersitProperty::ListType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard30Type, QString::fromLatin1("NICKNAME")),
                             QVersitProperty::ListType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard40Type, QString::fromLatin1("NICKNAME")),
                             QVersitProperty::ListType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard21Type, QString::fromLatin1("CATEGORIES")),
                             QVersitProperty::ListType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard30Type, QString::fromLatin1("CATEGORIES")),
                             QVersitProperty::ListType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard40Type, QString::fromLatin1("CATEGORIES")),
                             QVersitProperty::ListType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard21Type, QString::fromLatin1("X-CHILDREN")),
                             QVersitProperty::ListType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard30Type, QString::fromLatin1("X-CHILDREN")),
                             QVersitProperty::ListType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard40Type, QString::fromLatin1("X-CHILDREN")),
                             QVersitProperty::ListType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard21Type, QString::fromLatin1("X-NICKNAME")),
                             QVersitProperty::ListType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard21Type, QString::fromLatin1("X-FOLKS-FAVOURITE")),
                              QVersitProperty::CompoundType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard30Type, QString::fromLatin1("X-FOLKS-FAVOURITE")),
                             QVersitProperty::CompoundType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard40Type, QString::fromLatin1("X-FOLKS-FAVOURITE")),
                             QVersitProperty::CompoundType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard30Type, QString::fromLatin1("X-NICKNAME")),
                             QVersitProperty::ListType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard40Type, QString::fromLatin1("X-NICKNAME")),
                             QVersitProperty::ListType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard21Type, QString::fromLatin1("X-QTPROJECT-EXTENDED-DETAIL")),
                             QVersitProperty::CompoundType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard30Type, QString::fromLatin1("X-QTPROJECT-EXTENDED-DETAIL")),
                             QVersitProperty::CompoundType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard40Type, QString::fromLatin1("X-QTPROJECT-EXTENDED-DETAIL")),
                             QVersitProperty::CompoundType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::ICalendar20Type, QString::fromLatin1("X-QTPROJECT-EXTENDED-DETAIL")),
                             QVersitProperty::CompoundType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard21Type, QString::fromLatin1("X-QTPROJECT-FAVORITE")),
                              QVersitProperty::CompoundType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard30Type, QString::fromLatin1("X-QTPROJECT-FAVORITE")),
                             QVersitProperty::CompoundType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard40Type, QString::fromLatin1("X-QTPROJECT-FAVORITE")),
                             QVersitProperty::CompoundType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard21Type, QString::fromLatin1("X-QTPROJECT-VERSION")),
                             QVersitProperty::CompoundType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard30Type, QString::fromLatin1("X-QTPROJECT-VERSION")),
                             QVersitProperty::CompoundType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard40Type, QString::fromLatin1("X-QTPROJECT-VERSION")),
                             QVersitProperty::CompoundType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::ICalendar20Type, QString::fromLatin1("X-QTPROJECT-VERSION")),
                             QVersitProperty::CompoundType);

        // Some MeeGo specific types, for EDS/SyncEvolution roundtripping until the API allows
        // better control over the type of custom properties.
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard21Type, QString::fromLatin1("X-EDS-QTCONTACTS")),
                             QVersitProperty::CompoundType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard30Type, QString::fromLatin1("X-EDS-QTCONTACTS")),
                             QVersitProperty::CompoundType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard40Type, QString::fromLatin1("X-EDS-QTCONTACTS")),
                             QVersitProperty::CompoundType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard21Type, QString::fromLatin1("X-SYNCEVO-QTCONTACTS")),
                             QVersitProperty::CompoundType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard30Type, QString::fromLatin1("X-SYNCEVO-QTCONTACTS")),
                             QVersitProperty::CompoundType);
        mValueTypeMap->insert(qMakePair(QVersitDocument::VCard40Type, QString::fromLatin1("X-SYNCEVO-QTCONTACTS")),
                             QVersitProperty::CompoundType);

    }
    return mValueTypeMap;
}

/*!
 * Inherited from QThread, called by QThread when the thread has been started.
 */
void QVersitReaderPrivate::run()
{
    read();
}

/*!
 * Does the actual reading and sets the error and state as appropriate.
 * If \a async, then stateChanged() signals are emitted as the reading happens.
 */
void QVersitReaderPrivate::read()
{
    mMutex.lock();
    mVersitDocuments.clear();
    mMutex.unlock();
    bool canceled = false;

    LineReader lineReader(mIoDevice, mDefaultCodec);
    while(!lineReader.atEnd()) {
        if (isCanceling()) {
            canceled = true;
            break;
        }
        QVersitDocument document;
        int oldPos = lineReader.odometer();
        bool ok = parseVersitDocument(&lineReader, &document);

        if (ok) {
            if (document.isEmpty())
                break;
            else {
                QMutexLocker locker(&mMutex);
                mVersitDocuments.append(document);
                emit resultsAvailable();
            }
        } else {
            setError(QVersitReader::ParseError);
            if (lineReader.odometer() == oldPos)
                break;
        }
    };
    if (canceled)
        setState(QVersitReader::CanceledState);
    else
        setState(QVersitReader::FinishedState);
}

void QVersitReaderPrivate::setState(QVersitReader::State state)
{
    mMutex.lock();
    mState = state;
    mMutex.unlock();
    emit stateChanged(state);
}

QVersitReader::State QVersitReaderPrivate::state() const
{
    QMutexLocker locker(&mMutex);
    return mState;
}

void QVersitReaderPrivate::setError(QVersitReader::Error error)
{
    QMutexLocker locker(&mMutex);
    mError = error;
}

QVersitReader::Error QVersitReaderPrivate::error() const
{
    QMutexLocker locker(&mMutex);
    return mError;
}

void QVersitReaderPrivate::setCanceling(bool canceling)
{
    QMutexLocker locker(&mMutex);
    mIsCanceling = canceling;
}

bool QVersitReaderPrivate::isCanceling()
{
    QMutexLocker locker(&mMutex);
    return mIsCanceling;
}

/*!
 * Parses a versit document. Returns true if the parsing was successful.
 */
bool QVersitReaderPrivate::parseVersitDocument(LineReader* lineReader, QVersitDocument* document)
{
    if (mDocumentNestingLevel >= MAX_VERSIT_DOCUMENT_NESTING_DEPTH)
        return false; // To prevent infinite recursion

    // If we don't know what type it is, just assume it's a vCard 3.0
    if (document->type() == QVersitDocument::InvalidType)
        document->setType(QVersitDocument::VCard30Type);

    QVersitProperty property;

    property = parseNextVersitProperty(document->type(), lineReader);
    while (property.isEmpty() && !lineReader->atEnd()) {
        // Work around malformed documents by ignoring empty leading lines
        property = parseNextVersitProperty(document->type(), lineReader);
    }

    const QString propertyValue = property.value().trimmed().toUpper();
    if (property.isEmpty()) {
        // A blank document (or end of file) was found.
        document->clear();
        return true;
    } else if (property.name() == QStringLiteral("BEGIN")) {
        if (propertyValue == QStringLiteral("VCARD")) {
            document->setComponentType(propertyValue);
        } else if (propertyValue == QStringLiteral("VCALENDAR")) {
            document->setType(QVersitDocument::ICalendar20Type);
            document->setComponentType(propertyValue);
        } else {
            // Unknown document type
            document->clear();
            return false;
        }
    } else {
        // Some property other than BEGIN was found.
        document->clear();
        return false;
    }

    return parseVersitDocumentBody(lineReader, document);
}

/*! Parse the rest of a versit document after finding a BEGIN line */
bool QVersitReaderPrivate::parseVersitDocumentBody(LineReader* lineReader, QVersitDocument* document)
{
    mDocumentNestingLevel++;
    bool parsingOk = true;
    while (true) {
        /* Grab it */
        QVersitProperty property = parseNextVersitProperty(document->type(), lineReader);
        while (property.isEmpty() && !lineReader->atEnd()) {
            // Work around malformed documents by ignoring empty interior lines
            property = parseNextVersitProperty(document->type(), lineReader);
        }

        if (property.name() == QStringLiteral("BEGIN")) {
            // Nested Versit document
            QVersitDocument subDocument;
            subDocument.setType(document->type()); // the nested document inherits the parent's type
            subDocument.setComponentType(property.value().trimmed().toUpper());
            if (!parseVersitDocumentBody(lineReader, &subDocument))
                break;
            document->addSubDocument(subDocument);
        } else if (property.name() == QStringLiteral("VERSION")) {
            // A version property
            if (!setVersionFromProperty(document, property)) {
                parsingOk = false;
                break;
            }
        } else if (property.name() == QStringLiteral("END")) {
            // End of document
            break;
        } else if (property.name().isEmpty()) {
            // End of input or some other error
            parsingOk = false;
            break;
        } else {
            // A normal property - just add it.
            document->addProperty(property);
        }
    }
    if (!parsingOk)
        document->clear();
    mDocumentNestingLevel--;

    return parsingOk;
}

/*!
 * Parses a versit document and returns whether parsing succeeded.
 */
QVersitProperty QVersitReaderPrivate::parseNextVersitProperty(
        QVersitDocument::VersitType versitType,
        LineReader* lineReader)
{
    LByteArray line = lineReader->readLine();
    if (line.isEmpty() || line.toByteArray().trimmed().isEmpty())
        return QVersitProperty();

    // Otherwise, do stuff.
    QPair<QStringList,QString> groupsAndName =
            extractPropertyGroupsAndName(&line, lineReader->codec());

    QVersitProperty property;
    property.setGroups(groupsAndName.first);
    property.setName(groupsAndName.second);
    // set the propertyValueType
    QPair<QVersitDocument::VersitType, QString> key =
        qMakePair(versitType, property.name());
    if (valueTypeMap()->contains(key))
        property.setValueType(valueTypeMap()->value(key));

    if (versitType == QVersitDocument::VCard21Type)
        parseVCard21Property(&line, &property, lineReader);
    else if (versitType == QVersitDocument::VCard30Type
            || versitType == QVersitDocument::VCard40Type
            || versitType == QVersitDocument::ICalendar20Type)
        parseVCard30Property(versitType, &line, &property, lineReader);

    return property;
}

/*!
 * Parses the property according to vCard 2.1 syntax.
 */
void QVersitReaderPrivate::parseVCard21Property(LByteArray* line, QVersitProperty* property,
                                                LineReader* lineReader)
{
    property->setParameters(extractVCard21PropertyParams(line, lineReader->codec()));

    QByteArray value = line->toByteArray();
    if (property->valueType() == QVersitProperty::VersitDocumentType) {
        // Hack to handle cases where start of document is on the same or next line as "AGENT:"
        if (value == "BEGIN:VCARD") {
            lineReader->pushLine(value);
        } else if (value.isEmpty()) {
        } else {
            property->clear();
            return;
        }
        QVersitDocument subDocument(QVersitDocument::VCard21Type);
        if (!parseVersitDocument(lineReader, &subDocument)) {
            property->clear();
        } else {
            property->setValue(QVariant::fromValue(subDocument));
        }
    } else {
        bool isBinary = unencode(&value, property, lineReader);
        if (isBinary) {
            property->setValue(value);
            property->setValueType(QVersitProperty::BinaryType);
        }
        else {
            QTextCodec* ignored = 0;
            property->setValue(decodeCharset(value, property, lineReader, &ignored));
            splitStructuredValue(property, false);
        }
    }
}

/*!
 * Parses the property according to vCard 3.0 syntax.  This function is called for both vCard 3.0
 * and iCalendar properties.
 */
void QVersitReaderPrivate::parseVCard30Property(QVersitDocument::VersitType versitType,
                                                LByteArray* line, QVersitProperty* property,
                                                LineReader* lineReader)
{
    property->setParameters(extractVCard30PropertyParams(line, lineReader->codec()));

    QByteArray value = line->toByteArray();


    if (property->valueType() == QVersitProperty::VersitDocumentType) {
        QTextCodec* codec;
        QString valueString(decodeCharset(value, property, lineReader, &codec));
        removeBackSlashEscaping(&valueString);
        // Make a line reader from the value of the property.
        QByteArray subDocumentValue(codec->fromUnicode(valueString));
        QBuffer subDocumentData(&subDocumentValue);
        subDocumentData.open(QIODevice::ReadOnly);
        subDocumentData.seek(0);
        LineReader subDocumentLineReader(&subDocumentData, codec);

        // Recursive call!
        QVersitDocument subDocument(versitType);
        if (!parseVersitDocument(&subDocumentLineReader, &subDocument)) {
            property->clear();
        } else {
            property->setValue(QVariant::fromValue(subDocument));
        }
    } else {
        bool isBinary = unencode(&value, property, lineReader);
        if (isBinary) {
            property->setValue(value);
            property->setValueType(QVersitProperty::BinaryType);
        } else {
            QTextCodec* ignored = 0;
            property->setValue(decodeCharset(value, property, lineReader, &ignored));
            bool isList = splitStructuredValue(property, true);
            // Do backslash unescaping
            if (isList) {
                QStringList list = property->value<QStringList>();
                for (int i = 0; i < list.length(); i++) {
                    removeBackSlashEscaping(&list[i]);
                }
                property->setValue(list);
            } else {
                QString value = property->value();
                removeBackSlashEscaping(&value);
                property->setValue(value);
            }
        }
    }
}

/*!
 * Sets version to \a document if \a property contains a supported version.
 */
bool QVersitReaderPrivate::setVersionFromProperty(QVersitDocument* document, const QVersitProperty& property) const
{
    QString value = property.value().trimmed();
    if (document->componentType() == QStringLiteral("VCARD")
            && value == QStringLiteral("2.1")) {
        document->setType(QVersitDocument::VCard21Type);
    } else if (document->componentType() == QStringLiteral("VCARD")
            && value == QStringLiteral("3.0")) {
        document->setType(QVersitDocument::VCard30Type);
    } else if (document->componentType() == QStringLiteral("VCARD")
            && value == QStringLiteral("4.0")) {
        document->setType(QVersitDocument::VCard40Type);
    } else if ((document->componentType() == QStringLiteral("VCALENDAR")
                || document->type() == QVersitDocument::ICalendar20Type) // covers VEVENT, etc. when nested inside a VCALENDAR
            && value == QStringLiteral("2.0")) {
        document->setType(QVersitDocument::ICalendar20Type);
    } else {
        return false;
    }
    return true;
}

/*!
 * On entry, \a value should be the byte array to unencode.  It is modified to be the unencoded
 * version.  Returns true if and only if the value was base-64 encoded.  (This is used as a
 * heuristic later to decide whether to decode the byte array as text)
 * \a lineReader is supplied in case more lines need to be read (for quoted-printable).  The
 * \a property is supplied so we know what kind of encoding was used.
 */
bool QVersitReaderPrivate::unencode(QByteArray* value,
                                    QVersitProperty* property,
                                    LineReader* lineReader) const
{
    QStringList encodingParameters = property->parameters().values(QStringLiteral("ENCODING"));
    QStringList typeParameters = property->parameters().values(QStringLiteral("TYPE"));
    if (encodingParameters.contains(QStringLiteral("QUOTED-PRINTABLE"), Qt::CaseInsensitive)) {
        // At this point, we need to accumulate bytes until we hit a real line break (no = before
        // it) value already contains everything up to the character before the newline
        while (value->endsWith('=')) {
            value->chop(1); // Get rid of '='
            // We add each line (minus the escaped = and newline chars)
            value->append(lineReader->readLine().toByteArray());
        }
        decodeQuotedPrintable(value);
        // Remove the encoding parameter as the value is now decoded
        property->removeParameters(QStringLiteral("ENCODING"));
        return false;
    } else if (encodingParameters.contains(QStringLiteral("BASE64"), Qt::CaseInsensitive)
        || encodingParameters.contains(QStringLiteral("B"), Qt::CaseInsensitive)
        || typeParameters.contains(QStringLiteral("BASE64"), Qt::CaseInsensitive)
        || typeParameters.contains(QStringLiteral("B"), Qt::CaseInsensitive)) {
        *value = QByteArray::fromBase64(*value);
        // Remove the encoding parameter as the value is now decoded
        property->removeParameters(QStringLiteral("ENCODING"));
        return true;
    }
    return false;
}

/*!
 * Decodes \a value, after working out what charset it is in using the context of \a property and
 * returns it.  The codec used to decode is returned in \a codec.  If the CHARSET parameter was
 * specified, *charsetSpecified is set to true (else, false).
 */
QString QVersitReaderPrivate::decodeCharset(const QByteArray& value,
                                            QVersitProperty* property,
                                            LineReader* lineReader,
                                            QTextCodec** codec) const
{
    static const QString charset(QStringLiteral("CHARSET"));

    *codec = NULL;
    if (property->parameters().contains(charset)) {
        QString charsetValue = *property->parameters().find(charset);
        property->removeParameters(charset);
        *codec = QTextCodec::codecForName(charsetValue.toLatin1());
    } else if (!lineReader->isCodecCertain()
            && lineReader->isCodecUtf8Compatible()) {
        // Guess the codec because we don't know for sure what it is and it could possibly be
        // either UTF-8 or an 8-bit codec.
        if (VersitUtils::isValidUtf8(value)) {
            // Valid UTF-8
            *codec = QTextCodec::codecForName("UTF-8");
        } else {
            // Invalid UTF-8 - don't try to test future properties for UTF-8-compatibility
            lineReader->setCodecUtf8Incompatible();
        }
    }

    if (*codec == NULL)
        *codec = lineReader->codec();

    return (*codec)->toUnicode(value);
}

/*!
 * Decodes Quoted-Printable encoded (RFC 1521) characters in /a text.
 */
void QVersitReaderPrivate::decodeQuotedPrintable(QByteArray* text) const
{
    for (int i=0; i < text->length(); i++) {
        char current = text->at(i);
        if (current == '=' && i+2 < text->length()) {
            char next = text->at(i+1);
            char nextAfterNext = text->at(i+2);
            if (((next >= 'a' && next <= 'f') ||
                 (next >= 'A' && next <= 'F') ||
                 (next >= '0' && next <= '9')) &&
                ((nextAfterNext >= 'a' && nextAfterNext <= 'f') ||
                 (nextAfterNext >= 'A' && nextAfterNext <= 'F') ||
                 (nextAfterNext >= '0' && nextAfterNext <= '9'))) {
                bool ok;
                char decodedChar(text->mid(i+1, 2).toInt(&ok,16));
                if (ok) {
                    (*text)[i] = decodedChar;
                    text->remove(i+1, 2);
                }
            } else if (next == '\r' && nextAfterNext == '\n') {
                // Newlines can still be found here if they are encoded in a non-default charset.
                text->remove(i, 3);
            }
        }
    }
}

/*!
 * Extracts the groups and the name of the property using \a codec to determine the delimiters
 *
 * On entry, \a line should contain a whole line
 * On exit, \a line will be updated to remove the groups and name
 */
QPair<QStringList,QString>QVersitReaderPrivate::extractPropertyGroupsAndName(
        LByteArray* line, QTextCodec *codec) const
{
    const QByteArray semicolon = VersitUtils::encode(';', codec);
    const QByteArray colon = VersitUtils::encode(':', codec);
    const QByteArray backslash = VersitUtils::encode('\\', codec);
    QPair<QStringList,QString> groupsAndName;
    int length = 0;

    int separatorLength = semicolon.length();
    for (int i = 0; i < line->size() - separatorLength + 1; i++) {
        if ((containsAt(*line, semicolon, i) && !containsAt(*line, backslash, i-separatorLength))
            || containsAt(*line, colon, i)) {
            length = i;
            break;
        }
    }
    if (length > 0) {
        QString trimmedGroupsAndName = codec->toUnicode(line->left(length)).trimmed();
        QStringList parts = trimmedGroupsAndName.split(QLatin1Char('.'));
        if (parts.count() > 1) {
            groupsAndName.second = parts.takeLast();
            groupsAndName.first = parts;
        } else {
            groupsAndName.second = trimmedGroupsAndName;
        }
        line->chopLeft(length);
    }

    return groupsAndName;
}

/*!
 * Extracts the property parameters as a QMultiHash using \a codec to determine the delimiters.
 * The parameters without names are added as "TYPE" parameters.
 *
 * On entry \a line should contain the line sans the group and name
 * On exit, line will be updated to have the parameters removed.
 */
QMultiHash<QString,QString> QVersitReaderPrivate::extractVCard21PropertyParams(
        LByteArray* line, QTextCodec *codec) const
{
    QMultiHash<QString,QString> result;
    QList<QByteArray> paramList = extractParams(line, codec);
    while (!paramList.isEmpty()) {
        QByteArray param = paramList.takeLast();
        QString name = paramName(param, codec);
        QString value = paramValue(param, codec);
        result.insert(name,value);
    }

    return result;
}

/*!
 * Extracts the property parameters as a QMultiHash using \a codec to determine the delimiters.
 * The parameters without names are added as "TYPE" parameters.
 *
 * On entry \a line should contain the line sans the group and name
 * On exit, line will be updated to have the parameters removed.
 */
QMultiHash<QString,QString> QVersitReaderPrivate::extractVCard30PropertyParams(
        LByteArray* line, QTextCodec *codec) const
{
    QMultiHash<QString,QString> result;
    QList<QByteArray> paramList = extractParams(line, codec);
    while (!paramList.isEmpty()) {
        QByteArray param = paramList.takeLast();
        QString name(paramName(param, codec));
        removeBackSlashEscaping(&name);
        QString values = paramValue(param, codec);
        QStringList valueList = splitValue(values, QLatin1Char(','), QString::SkipEmptyParts, true);
        foreach (QString value, valueList) {
            removeBackSlashEscaping(&value);
            result.insert(name, value);
        }
    }
    return result;
}


/*!
 * Extracts the parameters as delimited by semicolons using \a codec to determine the delimiters.
 *
 * On entry \a line should contain the content line sans the group and name
 * On exit, \a line will be updated to only have the value remain
 */
QList<QByteArray> QVersitReaderPrivate::extractParams(LByteArray* line, QTextCodec *codec) const
{
    const QByteArray colon = VersitUtils::encode(':', codec);
    const QByteArray semicolon = VersitUtils::encode(';', codec);
    QList<QByteArray> params;

    /* find the end of the name&params */
    int colonIndex = line->indexOf(colon);
    if (colonIndex > 0) {
        QByteArray nameAndParamsString = line->left(colonIndex);
        params = extractParts(nameAndParamsString, semicolon, codec);

        /* Update line */
        line->chopLeft(colonIndex + colon.length());
    } else if (colonIndex == 0) {
        // No parameters.. advance past it
        line->chopLeft(colon.length());
    }

    return params;
}

/*!
 * Extracts the parts separated by separator discarding the separators escaped with a backslash
 * encoded with \a codec
 */
QList<QByteArray> QVersitReaderPrivate::extractParts(
        const QByteArray& text, const QByteArray& separator, QTextCodec* codec) const
{
    QList<QByteArray> parts;
    int partStartIndex = 0;
    int textLength = text.length();
    int separatorLength = separator.length();
    const QByteArray backslash = VersitUtils::encode('\\', codec);
    int backslashLength = backslash.length();

    for (int i=0; i < textLength-separatorLength+1; i++) {
        if (containsAt(text, separator, i)
            && (i < backslashLength
                || !containsAt(text, backslash, i-backslashLength))) {
            int length = i-partStartIndex;
            QByteArray part = extractPart(text,partStartIndex,length);
            if (part.length() > 0)
                parts.append(part);
            partStartIndex = i+separatorLength;
        }
    }

    // Add the last or only part
    QByteArray part = extractPart(text,partStartIndex);
    if (part.length() > 0)
        parts.append(part);
    return parts;
}

/*!
 * Extracts a substring limited by /a startPosition and /a length.
 */
QByteArray QVersitReaderPrivate::extractPart(
        const QByteArray& text, int startPosition, int length) const
{
    QByteArray part;
    if (startPosition >= 0)
        part = text.mid(startPosition,length).trimmed();
    return part;
}

/*!
 * Extracts the name of the parameter using \a codec to determine the delimiters.
 * No name is interpreted as an implicit "TYPE".
 */
QString QVersitReaderPrivate::paramName(const QByteArray& parameter, QTextCodec* codec) const
{
     if (parameter.trimmed().length() == 0)
         return QString();
     const QByteArray equals = VersitUtils::encode('=', codec);
     int equalsIndex = parameter.indexOf(equals);
     if (equalsIndex > 0) {
         return codec->toUnicode(parameter.left(equalsIndex)).trimmed();
     }

     return QStringLiteral("TYPE");
}

/*!
 * Extracts the value of the parameter using \a codec to determine the delimiters
 */
QString QVersitReaderPrivate::paramValue(const QByteArray& parameter, QTextCodec* codec) const
{
    QByteArray value(parameter);
    const QByteArray equals = VersitUtils::encode('=', codec);
    int equalsIndex = parameter.indexOf(equals);
    if (equalsIndex > 0) {
        int valueLength = parameter.length() - (equalsIndex + equals.length());
        value = parameter.right(valueLength).trimmed();
    }

    return codec->toUnicode(value);
}

/*
 * Returns true if and only if \a text contains \a ba at \a index
 *
 * On entry, index must be >= 0
 *
 * T is either a QByteArray or LByteArray
 */
template <class T> bool QVersitReaderPrivate::containsAt(const T& text, const QByteArray& match, int index)
{
    int n = match.length();
    // This check is necessary because constData doesn't ensure it's null terminated at the right place
    if (text.size() - index < n)
        return false;
    const char* textData = text.constData();
    const char* matchData = match.constData();
    return memcmp(textData+index, matchData, n) == 0;
}

/*!
 * If the \a type and the \a property's name is known to contain a structured value, \a property's
 * value is split according to the type of structuring (compound vs. list) it is known to have.
 * Returns true if and only if such a split happened (ie. the property value holds a QStringList on
 * exit).
 */
bool QVersitReaderPrivate::splitStructuredValue(
        QVersitProperty* property,
        bool hasEscapedBackslashes) const
{
    QVariant variant = property->variantValue();
    if (property->valueType() == QVersitProperty::CompoundType) {
        variant.setValue(splitValue(variant.toString(), QLatin1Char(';'),
                                    QString::KeepEmptyParts, hasEscapedBackslashes));
        property->setValue(variant);
        return true;
    } else if (property->valueType() == QVersitProperty::ListType) {
        variant.setValue(splitValue(variant.toString(), QLatin1Char(','),
                                    QString::SkipEmptyParts, hasEscapedBackslashes));
        property->setValue(variant);
        return true;
    }
    return false;
}

/*!
 * Splits the \a string into substrings wherever \a sep occurs.
 * If \a hasEscapedBackslashes is false, then a \a sep preceded by a backslash is not considered
 * a split point (but the backslash is removed).
 * If \a hasEscapedBackslashes is true, then a \a sep preceded by an odd number of backslashes is
 * not considered a split point (but one backslash is removed).
 */
QStringList QVersitReaderPrivate::splitValue(const QString& string,
                                             const QChar& sep,
                                             QString::SplitBehavior behavior,
                                             bool hasEscapedBackslashes)
{
    QStringList list;
    bool isEscaped = false; // is the current character escaped
    int segmentStartIndex = 0;
    QString segment;
    for (int i = 0; i < string.length(); i++) {
        if (string.at(i) == QLatin1Char('\\')) {
            if (hasEscapedBackslashes)
                isEscaped = !isEscaped; // two consecutive backslashes make isEscaped false
            else
                isEscaped = true;
        } else if (string.at(i) == sep) {
            if (isEscaped) {
                // we see an escaped separator - remove the backslash
                segment += string.mid(segmentStartIndex, i-segmentStartIndex-1);
                segment += sep;
            } else {
                // we see a separator
                segment += string.mid(segmentStartIndex, i - segmentStartIndex);
                if (behavior == QString::KeepEmptyParts || !segment.isEmpty())
                    list.append(segment);
                segment.clear();
            }
            segmentStartIndex = i+1;
            isEscaped = false;
        } else { // normal character - keep going
            isEscaped = false;
        }
    }
    // The rest of the string after the last sep.
    segment += string.mid(segmentStartIndex);
    if (behavior == QString::KeepEmptyParts || !segment.isEmpty())
        list.append(segment);
    return list;
}

/*!
 * Removes backslash escaping for line breaks (CRLFs), colons, semicolons, backslashes and commas
 * according to RFC 2426.  This is called on parameter names and values and property values.
 * Colons ARE unescaped because the text of RFC2426 suggests that they should be.
 */
void QVersitReaderPrivate::removeBackSlashEscaping(QString* text)
{
    if (!(text->startsWith(QLatin1Char('"')) && text->endsWith(QLatin1Char('"')))) {
        /* replaces \; with ;
                    \, with ,
                    \: with :
                    \\ with \
         */
        text->replace(QRegularExpression(QStringLiteral("\\\\([;,:\\\\])")), QStringLiteral("\\1"));
        // replaces \n with a CRLF
        text->replace(QStringLiteral("\\n"), QStringLiteral("\r\n"), Qt::CaseInsensitive);
    }
}

}

