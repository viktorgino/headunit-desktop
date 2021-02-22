/*
    Copyright (C) 2012 Openismus GmbH
      @author Mathias Hasselmann <mathias@openismus.com>

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
#ifndef QGST_DISCOVERER_H
#define QGST_DISCOVERER_H

#include <QGst/ClockTime>
#include <QGst/Fraction>
#include <QGst/MiniObject>
#include <QGst/Object>
#include <QGst/Structure>
#include <QGst/TagList>

#include <QtCore/QUrl>

#include "global.h"

namespace QGst {

class QTGSTREAMER_EXPORT DiscovererStreamInfo : public QGlib::Object
{
    QGST_WRAPPER(DiscovererStreamInfo)

public:
    DiscovererStreamInfoPtr previous() const;
    DiscovererStreamInfoPtr next() const;
    QString streamTypeNick() const;
    CapsPtr caps() const;
    TagList tags() const;
    Structure misc() const;
};

class QTGSTREAMER_EXPORT DiscovererContainerInfo : public DiscovererStreamInfo
{
    QGST_WRAPPER(DiscovererContainerInfo)

public:
    QList<DiscovererStreamInfoPtr> streams() const;
};

class QTGSTREAMER_EXPORT DiscovererAudioInfo : public DiscovererStreamInfo
{
    QGST_WRAPPER(DiscovererAudioInfo)

public:
    uint channels() const;
    uint sampleRate() const;
    uint depth() const;
    uint bitrate() const;
    uint maxBitrate() const;
    QString language() const;
};

class QTGSTREAMER_EXPORT DiscovererVideoInfo : public DiscovererStreamInfo
{
    QGST_WRAPPER(DiscovererVideoInfo)

public:
    uint width() const;
    uint height() const;
    uint depth() const;
    Fraction framerate() const;
    Fraction pixelAspectRatio() const;
    uint bitrate() const;
    uint maxBitrate() const;
    bool isInterlaced() const;
    bool isImage() const;
};

class QTGSTREAMER_EXPORT DiscovererSubtitleInfo : public DiscovererStreamInfo
{
    QGST_WRAPPER(DiscovererSubtitleInfo)

public:
    QString language() const;
};

class QTGSTREAMER_EXPORT DiscovererInfo : public QGlib::Object
{
    QGST_WRAPPER(DiscovererInfo)

public:
    QUrl uri() const;
    DiscovererResult result() const;

    ClockTime duration() const;
    bool seekable() const;
    Structure misc() const;
    TagList tags() const;

    DiscovererStreamInfoPtr streamInfo() const;
    QList<DiscovererStreamInfoPtr> streams() const;
    QList<DiscovererStreamInfoPtr> streams(QGlib::Type streamType) const;
    QList<DiscovererStreamInfoPtr> audioStreams() const;
    QList<DiscovererStreamInfoPtr> videoStreams() const;
    QList<DiscovererStreamInfoPtr> subtitleStreams() const;
    QList<DiscovererStreamInfoPtr> containerStreams() const;
};

class QTGSTREAMER_EXPORT Discoverer : public QGlib::Object
{
    QGST_WRAPPER(Discoverer)

public:
    /*! Creates a new discoverer with the provided timeout.
     * \throws QGlib::Error when there was a problem creating the discoverer
     */
    static DiscovererPtr create(ClockTime timeout);

    /*! Allow asynchronous discovering of URIs to take place.
     * \note A GLib event loop must be available for QGst::Discoverer to work
     * properly in asynchronous mode. This feature might not be available on all
     * platforms supported by Qt.
     * \ingroup async
     */
    void start();

    /*! Stop the discovery of any pending URIs and clears the list of pending URIS (if any).
     * \ingroup async
     */
    void stop();

    /*! Appends the given \param uri to the list of URIs to discoverer.
     * The actual discovery of the \param uri will only take place if start() has been called.
     * \ingroup async
     */
    bool discoverUriAsync(const char *uri);

    /*! \overload */
    inline bool discoverUriAsync(const QUrl &uri);

    /*! Synchronously discovers the given \param uri.
     * \throws QGlib::Error if an error occurred
     * \ingroup sync
     */
    DiscovererInfoPtr discoverUri(const char *uri);

    /*! \overload */
    inline DiscovererInfoPtr discoverUri(const QUrl &uri);
};

inline bool Discoverer::discoverUriAsync(const QUrl &uri)
{
    return discoverUriAsync(uri.toEncoded().constData());
}

inline DiscovererInfoPtr Discoverer::discoverUri(const QUrl &uri)
{
    return discoverUri(uri.toEncoded().constData());
}

QTGSTREAMER_EXPORT QDebug operator<<(QDebug debug, DiscovererResult result);
QTGSTREAMER_EXPORT QDebug operator<<(QDebug debug, const DiscovererStreamInfoPtr &info);
QTGSTREAMER_EXPORT QDebug operator<<(QDebug debug, const DiscovererInfoPtr &info);

} // namespace QGst

QGST_REGISTER_TYPE(QGst::DiscovererStreamInfo)
QGST_REGISTER_TYPE(QGst::DiscovererContainerInfo)
QGST_REGISTER_TYPE(QGst::DiscovererAudioInfo)
QGST_REGISTER_TYPE(QGst::DiscovererVideoInfo)
QGST_REGISTER_TYPE(QGst::DiscovererSubtitleInfo)
QGST_REGISTER_TYPE(QGst::DiscovererInfo)
QGST_REGISTER_TYPE(QGst::Discoverer)

#endif // QGST_DISCOVERER_H
