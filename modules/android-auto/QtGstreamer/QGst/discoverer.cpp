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
#include "discoverer.h"

#include <QGlib/Error>
#include <QGst/Caps>

#include <gst/pbutils/gstdiscoverer.h>

namespace QGst {

static QList<DiscovererStreamInfoPtr> wrapStreamInfoList(GList *input, bool increaseRef)
{
    QList<DiscovererStreamInfoPtr> output;

    while(input) {
        output += DiscovererStreamInfoPtr::wrap(GST_DISCOVERER_STREAM_INFO(input->data), increaseRef);
        input = g_list_delete_link(input, input);
    }

    return output;
}

DiscovererStreamInfoPtr DiscovererStreamInfo::previous() const
{
    return DiscovererStreamInfoPtr::wrap(gst_discoverer_stream_info_get_previous(object<GstDiscovererStreamInfo>()), false);
}

DiscovererStreamInfoPtr DiscovererStreamInfo::next() const
{
    return DiscovererStreamInfoPtr::wrap(gst_discoverer_stream_info_get_next(object<GstDiscovererStreamInfo>()), false);
}

QString DiscovererStreamInfo::streamTypeNick() const
{
    return QString::fromUtf8(gst_discoverer_stream_info_get_stream_type_nick(object<GstDiscovererStreamInfo>()));
}

CapsPtr DiscovererStreamInfo::caps() const
{
    return CapsPtr::wrap(gst_discoverer_stream_info_get_caps(object<GstDiscovererStreamInfo>()), false);
}

TagList DiscovererStreamInfo::tags() const
{
    return gst_discoverer_stream_info_get_tags(object<GstDiscovererStreamInfo>());
}

Structure DiscovererStreamInfo::misc() const
{
    const GstStructure *const misc = gst_discoverer_stream_info_get_misc(object<GstDiscovererStreamInfo>());
    return misc ? Structure(misc) : Structure();
}

QList<DiscovererStreamInfoPtr> DiscovererContainerInfo::streams() const
{
    return wrapStreamInfoList(gst_discoverer_container_info_get_streams(object<GstDiscovererContainerInfo>()), false);
}

uint DiscovererAudioInfo::channels() const
{
    return gst_discoverer_audio_info_get_channels(object<GstDiscovererAudioInfo>());
}

uint DiscovererAudioInfo::sampleRate() const
{
    return gst_discoverer_audio_info_get_sample_rate(object<GstDiscovererAudioInfo>());
}

uint DiscovererAudioInfo::depth() const
{
    return gst_discoverer_audio_info_get_depth(object<GstDiscovererAudioInfo>());
}

uint DiscovererAudioInfo::bitrate() const
{
    return gst_discoverer_audio_info_get_bitrate(object<GstDiscovererAudioInfo>());
}

uint DiscovererAudioInfo::maxBitrate() const
{
    return gst_discoverer_audio_info_get_max_bitrate(object<GstDiscovererAudioInfo>());
}

QString DiscovererAudioInfo::language() const
{
    return QString::fromUtf8(gst_discoverer_audio_info_get_language(object<GstDiscovererAudioInfo>()));
}

uint DiscovererVideoInfo::width() const
{
    return gst_discoverer_video_info_get_width(object<GstDiscovererVideoInfo>());
}

uint DiscovererVideoInfo::height() const
{
    return gst_discoverer_video_info_get_height(object<GstDiscovererVideoInfo>());
}

uint DiscovererVideoInfo::depth() const
{
    return gst_discoverer_video_info_get_depth(object<GstDiscovererVideoInfo>());
}

Fraction DiscovererVideoInfo::framerate() const
{
    return Fraction(gst_discoverer_video_info_get_framerate_num(object<GstDiscovererVideoInfo>()),
                    gst_discoverer_video_info_get_framerate_denom(object<GstDiscovererVideoInfo>()));
}

Fraction DiscovererVideoInfo::pixelAspectRatio() const
{
    return Fraction(gst_discoverer_video_info_get_par_num(object<GstDiscovererVideoInfo>()),
                    gst_discoverer_video_info_get_par_denom(object<GstDiscovererVideoInfo>()));
}

uint DiscovererVideoInfo::bitrate() const
{
    return gst_discoverer_video_info_get_bitrate(object<GstDiscovererVideoInfo>());
}

uint DiscovererVideoInfo::maxBitrate() const
{
    return gst_discoverer_video_info_get_max_bitrate(object<GstDiscovererVideoInfo>());
}

bool DiscovererVideoInfo::isInterlaced() const
{
    return gst_discoverer_video_info_is_interlaced(object<GstDiscovererVideoInfo>());
}

bool DiscovererVideoInfo::isImage() const
{
    return gst_discoverer_video_info_is_image(object<GstDiscovererVideoInfo>());
}

QString DiscovererSubtitleInfo::language() const
{
    return QString::fromUtf8(gst_discoverer_subtitle_info_get_language(object<GstDiscovererSubtitleInfo>()));
}

QUrl DiscovererInfo::uri() const
{
    return QUrl::fromEncoded(gst_discoverer_info_get_uri(object<GstDiscovererInfo>()));
}

DiscovererResult DiscovererInfo::result() const
{
    return DiscovererResult(gst_discoverer_info_get_result(object<GstDiscovererInfo>()));
}

ClockTime DiscovererInfo::duration() const
{
    return gst_discoverer_info_get_duration(object<GstDiscovererInfo>());
}

bool DiscovererInfo::seekable() const
{
    return gst_discoverer_info_get_seekable(object<GstDiscovererInfo>());
}

Structure DiscovererInfo::misc() const
{
    const GstStructure *const misc = gst_discoverer_info_get_misc(object<GstDiscovererInfo>());
    return misc ? Structure(misc) : Structure();
}

TagList DiscovererInfo::tags() const
{
    return gst_discoverer_info_get_tags(object<GstDiscovererInfo>());
}

DiscovererStreamInfoPtr DiscovererInfo::streamInfo() const
{
    return DiscovererStreamInfoPtr::wrap(gst_discoverer_info_get_stream_info(object<GstDiscovererInfo>()), false);
}

QList<DiscovererStreamInfoPtr> DiscovererInfo::streams() const
{
    return wrapStreamInfoList(gst_discoverer_info_get_stream_list(object<GstDiscovererInfo>()), false);
}

QList<DiscovererStreamInfoPtr> DiscovererInfo::streams(QGlib::Type streamType) const
{
    return wrapStreamInfoList(gst_discoverer_info_get_streams(object<GstDiscovererInfo>(), streamType), false);
}

QList<DiscovererStreamInfoPtr> DiscovererInfo::audioStreams() const
{
    return wrapStreamInfoList(gst_discoverer_info_get_audio_streams(object<GstDiscovererInfo>()), false);
}

QList<DiscovererStreamInfoPtr> DiscovererInfo::videoStreams() const
{
    return wrapStreamInfoList(gst_discoverer_info_get_video_streams(object<GstDiscovererInfo>()), false);
}

QList<DiscovererStreamInfoPtr> DiscovererInfo::subtitleStreams() const
{
    return wrapStreamInfoList(gst_discoverer_info_get_subtitle_streams(object<GstDiscovererInfo>()), false);
}

QList<DiscovererStreamInfoPtr> DiscovererInfo::containerStreams() const
{
    return wrapStreamInfoList(gst_discoverer_info_get_container_streams(object<GstDiscovererInfo>()), false);
}

DiscovererPtr Discoverer::create(ClockTime timeout)
{
    GError *error = NULL;
    GstDiscoverer *discoverer = gst_discoverer_new(timeout, &error);
    if (error) {
        throw QGlib::Error(error);
    }
    if (discoverer) {
        g_object_ref_sink(discoverer);
    }
    return DiscovererPtr::wrap(discoverer, false);
}

void Discoverer::start()
{
    gst_discoverer_start(object<GstDiscoverer>());
}

void Discoverer::stop()
{
    gst_discoverer_stop(object<GstDiscoverer>());
}

bool Discoverer::discoverUriAsync(const char *uri)
{
    return gst_discoverer_discover_uri_async(object<GstDiscoverer>(), uri);
}

DiscovererInfoPtr Discoverer::discoverUri(const char *uri)
{
    GError *error = NULL;
    GstDiscovererInfo *info = gst_discoverer_discover_uri(object<GstDiscoverer>(), uri, &error);
    if (error) {
        throw QGlib::Error(error);
    }
    return DiscovererInfoPtr::wrap(info, false);
}

QDebug operator<<(QDebug debug, DiscovererResult result)
{
    switch(result) {
    case DiscovererOk:
        return debug << "QGst::DiscovererOk";
    case DiscovererUriInvalid:
        return debug << "QGst::DiscovererUriInvalid";
    case DiscovererError:
        return debug << "QGst::DiscovererError";
    case DiscovererTimeout:
        return debug << "QGst::DiscovererTimeout";
    case DiscovererBusy:
        return debug << "QGst::DiscovererBusy";
    case DiscovererMissingPlugins:
        return debug << "QGst::DiscovererMissingPlugins";
    }

    return (debug.nospace() << "QGst::DiscovererResult(" << uint(result) << ")").maybeSpace();
}

template <typename T>
static const char *typeName();

template <typename T>
static QDebug printStreamInfoDetails(QDebug debug, const T &);

template <>
const char *typeName<DiscovererStreamInfoPtr>()
{
    return "QGst::DiscovererStreamInfoPtr";
}

template <>
const char *typeName<DiscovererContainerInfoPtr>()
{
    return "QGst::DiscovererContainerInfoPtr";
}

template <>
const char *typeName<DiscovererAudioInfoPtr>()
{
    return "QGst::DiscovererAudioInfoPtr";
}

template <>
const char *typeName<DiscovererVideoInfoPtr>()
{
    return "QGst::DiscovererVideoInfoPtr";
}

template <>
const char *typeName<DiscovererSubtitleInfoPtr>()
{
    return "QGst::DiscovererSubtitleInfoPtr";
}

template <>
QDebug printStreamInfoDetails(QDebug debug, const DiscovererStreamInfoPtr &)
{
    return debug;
}

template <>
QDebug printStreamInfoDetails(QDebug debug, const DiscovererContainerInfoPtr &info)
{
    return debug.nospace()
            << ", streams=" << info->streams();
}

template <>
QDebug printStreamInfoDetails(QDebug debug, const DiscovererAudioInfoPtr &info)
{
    return debug.nospace()
            << ", channels=" << info->channels()
            << ", sampleRate=" << info->sampleRate()
            << ", depth=" << info->depth()
            << ", bitrate=" << info->bitrate()
            << ", maxBitrate=" << info->maxBitrate()
            << ", language=" << info->language();
}

template <>
QDebug printStreamInfoDetails(QDebug debug, const DiscovererVideoInfoPtr &info)
{
    return debug.nospace()
            << ", width=" << info->width()
            << ", height=" << info->height()
            << ", depth=" << info->depth()
            << ", framerate=" << info->framerate()
            << ", pixelAspectRatio=" << info->pixelAspectRatio()
            << ", bitrate=" << info->bitrate()
            << ", maxBitrate=" << info->maxBitrate()
            << ", isInterlaced=" << info->isInterlaced()
            << ", isImage=" << info->isImage();
}

template <>
QDebug printStreamInfoDetails(QDebug debug, const DiscovererSubtitleInfoPtr &info)
{
    return debug.nospace()
            << ", language=" << info->language();
}

template <typename T>
static QDebug printStreamInfo(QDebug debug, const T &info)
{
    debug.nospace() << typeName<T>() << "(";

    if (info) {
        debug.nospace() << info->streamTypeNick()
                        << ", caps=" << info->caps();
        debug.nospace() << ", tags=" << info->tags();
        debug.nospace() << ", misc=" << info->misc();
        debug.nospace() << ", hasPrevious=" << !info->previous().isNull();
        debug.nospace() << ", hasNext=" << !info->next().isNull();
        debug = printStreamInfoDetails(debug, info);
    } else {
        debug << "<null>";
    }

    return (debug << ")").maybeSpace();
}

QDebug operator<<(QDebug debug, const DiscovererStreamInfoPtr &info)
{
    if (const DiscovererContainerInfoPtr containerInfo = info.dynamicCast<DiscovererContainerInfo>()) {
        return printStreamInfo(debug, containerInfo);
    }
    if (const DiscovererAudioInfoPtr audioInfo = info.dynamicCast<DiscovererAudioInfo>()) {
        return printStreamInfo(debug, audioInfo);
    }
    if (const DiscovererVideoInfoPtr videoInfo = info.dynamicCast<DiscovererVideoInfo>()) {
        return printStreamInfo(debug, videoInfo);
    }
    if (const DiscovererSubtitleInfoPtr subtitleInfo = info.dynamicCast<DiscovererSubtitleInfo>()) {
        return printStreamInfo(debug, subtitleInfo);
    }

    return printStreamInfo(debug, info);
}

QDebug operator<<(QDebug debug, const DiscovererInfoPtr &info)
{
    debug.nospace() << "QGst::DiscovererInfoPtr(";

    if (info) {
        debug.nospace() << "result=" << info->result()
                        << ", uri=" << info->uri()
                        << ", duration=" << info->duration()
                        << ", seekable=" << info->seekable()
                        << ", misc=" << info->misc();
        debug.nospace() << ", tags=" << info->tags();
        debug.nospace() << ", streamInfo=" << info->streamInfo();
        debug.nospace() << ", streams=" << info->streams();
        debug.nospace() << ", audioStreams=" << info->audioStreams();
        debug.nospace() << ", videoStreams=" << info->videoStreams();
        debug.nospace() << ", subtitleStreams=" << info->subtitleStreams();
        debug.nospace() << ", containerStreams=" << info->containerStreams();
    } else {
        debug << "<null>";
    }

    return (debug << ")").maybeSpace();
}

} // namespace QGst
