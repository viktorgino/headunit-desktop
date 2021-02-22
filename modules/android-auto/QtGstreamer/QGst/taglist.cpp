/*
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
#include "taglist.h"
#include "sample.h"
#include "../QGlib/string_p.h"
#include <gst/gst.h>
#include <QtCore/QDebug>
#include <QtCore/QDate>

namespace QGst {

QString getStringTag(GstTagList * list, const gchar * tag, int index)
{
    gchar * value;
    if (gst_tag_list_get_string_index(list, tag, index, &value)) {
        return QGlib::Private::stringFromGCharPtr(value);
    }
    return QString::null;
}

quint32 getUintTag(GstTagList * list, const gchar * tag, int index)
{
    guint value = 0;
    gst_tag_list_get_uint_index(list, tag, index, &value);
    return value;
}

double getDoubleTag(GstTagList * list, const gchar * tag, int index)
{
    double value = 0;
    gst_tag_list_get_double_index(list, tag, index, &value);
    return value;
}

SamplePtr getSampleTag(GstTagList * list, const gchar * tag, int index)
{
    GstSample *s = NULL;
    gst_tag_list_get_sample_index(list, tag, index, &s);
    //Buffer is already refd()
    return SamplePtr::wrap(s, false);
}

#ifndef DOXYGEN_RUN

struct QTGSTREAMER_NO_EXPORT TagList::Data : public QSharedData
{
    Data();
    Data(const GstTagList *tl);
    Data(const Data & other);
    ~Data();

    GstTagList *taglist;
};

TagList::Data::Data()
    : QSharedData()
{
    taglist = gst_tag_list_new_empty();
}

TagList::Data::Data(const GstTagList *tl)
    : QSharedData()
{
    if (tl && GST_IS_TAG_LIST(tl)) {
        taglist = gst_tag_list_copy(tl);
    } else {
        taglist = gst_tag_list_new_empty();
    }
}

TagList::Data::Data(const TagList::Data & other)
    : QSharedData(other)
{
    taglist = gst_tag_list_copy(other.taglist);
}

TagList::Data::~Data()
{
    gst_tag_list_unref(taglist);
}

#endif //DOXYGEN_RUN

TagList::TagList()
    : d(new Data)
{
}

TagList::TagList(const GstTagList *taglist)
    : d(new Data(taglist))
{
}

TagList::~TagList()
{
}

TagList::TagList(const TagList & other)
    : d(other.d)
{
}

TagList & TagList::operator=(const TagList & other)
{
    d = other.d;
    return *this;
}

bool TagList::isEmpty() const
{
    return gst_tag_list_is_empty(d->taglist);
}

void TagList::insert(const TagList & other, TagMergeMode mode)
{
    gst_tag_list_insert(d->taglist, other.d->taglist, static_cast<GstTagMergeMode>(mode));
}

//static
TagList TagList::merge(const TagList & firstList, const TagList & secondList, TagMergeMode mode)
{
    GstTagList *taglist = gst_tag_list_merge(firstList, secondList,
                                             static_cast<GstTagMergeMode>(mode));

    //avoid copying the merged taglist by freeing the new one and assigning this one to d->taglist
    TagList tl;
    gst_tag_list_unref(tl.d->taglist);
    tl.d->taglist = taglist;
    return tl;
}

QGlib::Value TagList::tagValue(const char *tag, int index) const
{
    return QGlib::Value(gst_tag_list_get_value_index(d->taglist, tag, index));
}

void TagList::setTagValue(const char *tag, const QGlib::Value & value, TagMergeMode mode)
{
    gst_tag_list_add_value(d->taglist, static_cast<GstTagMergeMode>(mode), tag, value);
}

int TagList::tagValueCount(const char *tag) const
{
    return gst_tag_list_get_tag_size(d->taglist, tag);
}

void TagList::clear()
{
    gst_tag_list_unref(d->taglist);
    d->taglist = gst_tag_list_new_empty();
}

void TagList::removeTag(const char *tag)
{
    gst_tag_list_remove_tag(d->taglist, tag);
}

TagList::operator GstTagList*()
{
    return d->taglist;
}

TagList::operator const GstTagList*() const
{
    return d->taglist;
}

QString TagList::title(int index) const
{
    return getStringTag(d->taglist, GST_TAG_TITLE, index);
}

void TagList::setTitle(const QString & value, TagMergeMode mode)
{
    gst_tag_list_add_value(d->taglist, static_cast<GstTagMergeMode>(mode),
                           GST_TAG_TITLE, QGlib::Value::create(value));
}

int TagList::titleCount() const
{
    return gst_tag_list_get_tag_size(d->taglist, GST_TAG_TITLE);
}

QString TagList::titleSortName() const
{
    return getStringTag(d->taglist, GST_TAG_TITLE_SORTNAME, 0);
}

void TagList::setTitleSortName(const QString & value)
{
    gst_tag_list_add_value(d->taglist, GST_TAG_MERGE_REPLACE_ALL,
                           GST_TAG_TITLE_SORTNAME, QGlib::Value::create(value));
}

QString TagList::artist(int index) const
{
    return getStringTag(d->taglist, GST_TAG_ARTIST, index);
}

void TagList::setArtist(const QString & value, TagMergeMode mode)
{
    gst_tag_list_add_value(d->taglist, static_cast<GstTagMergeMode>(mode),
                           GST_TAG_ARTIST, QGlib::Value::create(value));
}

int TagList::artistCount() const
{
    return gst_tag_list_get_tag_size(d->taglist, GST_TAG_ARTIST);
}

QString TagList::artistSortName() const
{
    return getStringTag(d->taglist, GST_TAG_ARTIST_SORTNAME, 0);
}

void TagList::setArtistSortName(const QString & value)
{
    gst_tag_list_add_value(d->taglist, GST_TAG_MERGE_REPLACE_ALL,
                           GST_TAG_ARTIST_SORTNAME, QGlib::Value::create(value));
}

QString TagList::composer(int index) const
{
    return getStringTag(d->taglist, GST_TAG_COMPOSER, index);
}

void TagList::setComposer(const QString & value, TagMergeMode mode)
{
    gst_tag_list_add_value(d->taglist, static_cast<GstTagMergeMode>(mode),
                           GST_TAG_COMPOSER, QGlib::Value::create(value));
}

int TagList::composerCount() const
{
    return gst_tag_list_get_tag_size(d->taglist, GST_TAG_COMPOSER);
}

QDate TagList::date() const
{
    GDate * value;
    if (gst_tag_list_get_date(d->taglist, GST_TAG_DATE, &value)) {
        QDate ret = QDate::fromJulianDay(g_date_get_julian(value));
        g_date_free(value);
        return ret;
    }
    return QDate();
}

void TagList::setDate(const QDate & value)
{
    GDate * date = g_date_new_julian(value.toJulianDay());
    gst_tag_list_add (d->taglist, GST_TAG_MERGE_REPLACE_ALL, GST_TAG_DATE, date, NULL);
}

QString TagList::genre(int index) const
{
    return getStringTag(d->taglist, GST_TAG_GENRE, index);
}

void TagList::setGenre(const QString & value, TagMergeMode mode)
{
    gst_tag_list_add_value(d->taglist, static_cast<GstTagMergeMode>(mode),
                           GST_TAG_GENRE, QGlib::Value::create(value));
}

int TagList::genreCount() const
{
    return gst_tag_list_get_tag_size(d->taglist, GST_TAG_GENRE);
}

QString TagList::comment(int index) const
{
    return getStringTag(d->taglist, GST_TAG_COMMENT, index);
}

void TagList::setComment(const QString & value, TagMergeMode mode)
{
    gst_tag_list_add_value(d->taglist, static_cast<GstTagMergeMode>(mode),
                           GST_TAG_COMMENT, QGlib::Value::create(value));
}

int TagList::commentCount() const
{
    return gst_tag_list_get_tag_size(d->taglist, GST_TAG_COMMENT);
}

QString TagList::extendedComment(int index) const
{
    return getStringTag(d->taglist, GST_TAG_EXTENDED_COMMENT, index);
}

void TagList::setExtendedComment(const QString & value, TagMergeMode mode)
{
    gst_tag_list_add_value(d->taglist, static_cast<GstTagMergeMode>(mode),
                           GST_TAG_EXTENDED_COMMENT, QGlib::Value::create(value));
}

int TagList::extendedCommentCount() const
{
        return gst_tag_list_get_tag_size(d->taglist, GST_TAG_EXTENDED_COMMENT);
}

quint32 TagList::trackNumber() const
{
    return getUintTag(d->taglist, GST_TAG_TRACK_NUMBER, 0);
}

void TagList::setTrackNumber(quint32 value)
{
    gst_tag_list_add(d->taglist, GST_TAG_MERGE_REPLACE_ALL, GST_TAG_TRACK_NUMBER, value, NULL);
}

quint32 TagList::trackCount() const
{
    return getUintTag(d->taglist, GST_TAG_TRACK_COUNT, 0);
}

void TagList::setTrackCount(quint32 value)
{
    gst_tag_list_add(d->taglist, GST_TAG_MERGE_REPLACE_ALL, GST_TAG_TRACK_COUNT, value, NULL);
}

quint32 TagList::albumVolumeNumber() const
{
    return getUintTag(d->taglist, GST_TAG_ALBUM_VOLUME_NUMBER, 0);
}

void TagList::setAlbumVolumeNumber(quint32 value)
{
    gst_tag_list_add(d->taglist, GST_TAG_MERGE_REPLACE_ALL, GST_TAG_ALBUM_VOLUME_NUMBER, value, NULL);
}

quint32 TagList::albumVolumeCount() const
{
    return getUintTag(d->taglist, GST_TAG_ALBUM_VOLUME_COUNT, 0);
}

void TagList::setAlbumVolumeCount(quint32 value)
{
    gst_tag_list_add(d->taglist, GST_TAG_MERGE_REPLACE_ALL, GST_TAG_ALBUM_VOLUME_COUNT, value, NULL);
}

QString TagList::location(int index) const
{
    return getStringTag(d->taglist, GST_TAG_LOCATION, index);
}

void TagList::setLocation(const QString & value, TagMergeMode mode)
{
    gst_tag_list_add_value(d->taglist, static_cast<GstTagMergeMode>(mode),
                           GST_TAG_LOCATION, QGlib::Value::create(value));
}

int TagList::locationCount() const
{
    return gst_tag_list_get_tag_size(d->taglist, GST_TAG_LOCATION);
}

QString TagList::homepage(int index) const
{
    return getStringTag(d->taglist, GST_TAG_HOMEPAGE, index);
}

void TagList::setHomepage(const QString & value, TagMergeMode mode)
{
    gst_tag_list_add_value(d->taglist, static_cast<GstTagMergeMode>(mode),
                           GST_TAG_HOMEPAGE, QGlib::Value::create(value));
}

int TagList::homepageCount() const
{
    return gst_tag_list_get_tag_size(d->taglist, GST_TAG_HOMEPAGE);
}

QString TagList::description(int index) const
{
    return getStringTag(d->taglist, GST_TAG_DESCRIPTION, index);
}

void TagList::setDescription(const QString & value, TagMergeMode mode)
{
    gst_tag_list_add_value(d->taglist, static_cast<GstTagMergeMode>(mode),
                           GST_TAG_DESCRIPTION, QGlib::Value::create(value));
}

int TagList::descriptionCount() const
{
    return gst_tag_list_get_tag_size(d->taglist, GST_TAG_DESCRIPTION);
}

QString TagList::version() const
{
    return getStringTag(d->taglist, GST_TAG_VERSION, 0);
}

void TagList::setVersion(const QString & value)
{
    gst_tag_list_add_value(d->taglist, GST_TAG_MERGE_REPLACE_ALL,
                           GST_TAG_VERSION, QGlib::Value::create(value));
}

QString TagList::isrc() const
{
    return getStringTag(d->taglist, GST_TAG_ISRC, 0);
}

void TagList::setIsrc(const QString & value)
{
    gst_tag_list_add_value(d->taglist, GST_TAG_MERGE_REPLACE_ALL,
                           GST_TAG_ISRC, QGlib::Value::create(value));
}

QString TagList::organization(int index) const
{
    return getStringTag(d->taglist, GST_TAG_ORGANIZATION, index);
}

void TagList::setOrganization(const QString & value, TagMergeMode mode)
{
    gst_tag_list_add_value(d->taglist, static_cast<GstTagMergeMode>(mode),
                           GST_TAG_ORGANIZATION, QGlib::Value::create(value));
}

int TagList::organizationCount() const
{
    return gst_tag_list_get_tag_size(d->taglist, GST_TAG_ORGANIZATION);
}

QString TagList::copyright() const
{
    return getStringTag(d->taglist, GST_TAG_COPYRIGHT, 0);
}

void TagList::setCopyright(const QString & value)
{
    gst_tag_list_add_value(d->taglist, GST_TAG_MERGE_REPLACE_ALL,
                           GST_TAG_COPYRIGHT, QGlib::Value::create(value));
}

QString TagList::copyrightUri() const
{
    return getStringTag(d->taglist, GST_TAG_COPYRIGHT_URI, 0);
}

void TagList::setCopyrightUri(const QString & value)
{
    gst_tag_list_add_value(d->taglist, GST_TAG_MERGE_REPLACE_ALL,
                           GST_TAG_COPYRIGHT_URI, QGlib::Value::create(value));
}

QString TagList::contact(int index) const
{
    return getStringTag(d->taglist, GST_TAG_CONTACT, index);
}

void TagList::setContact(const QString & value, TagMergeMode mode)
{
    gst_tag_list_add_value(d->taglist, static_cast<GstTagMergeMode>(mode),
                           GST_TAG_CONTACT, QGlib::Value::create(value));
}

int TagList::contactCount() const
{
    return gst_tag_list_get_tag_size(d->taglist, GST_TAG_CONTACT);
}

QString TagList::license() const
{
    return getStringTag(d->taglist, GST_TAG_LICENSE, 0);
}

void TagList::setLicense(const QString & value)
{
    gst_tag_list_add_value(d->taglist, GST_TAG_MERGE_REPLACE_ALL,
                           GST_TAG_LICENSE, QGlib::Value::create(value));
}

QString TagList::licenseUri() const
{
    return getStringTag(d->taglist, GST_TAG_LICENSE_URI, 0);
}

void TagList::setLicenseUri(const QString & value)
{
    gst_tag_list_add_value(d->taglist, GST_TAG_MERGE_REPLACE_ALL,
                           GST_TAG_LICENSE_URI, QGlib::Value::create(value));
}

QString TagList::performer(int index) const
{
    return getStringTag(d->taglist, GST_TAG_PERFORMER, index);
}

void TagList::setPerformer(const QString & value, TagMergeMode mode)
{
    gst_tag_list_add_value(d->taglist, static_cast<GstTagMergeMode>(mode),
                           GST_TAG_PERFORMER, QGlib::Value::create(value));
}

int TagList::performerCount() const
{
    return gst_tag_list_get_tag_size(d->taglist, GST_TAG_PERFORMER);
}

quint64 TagList::duration() const
{
        guint64 value = 0;
        gst_tag_list_get_uint64_index(d->taglist, GST_TAG_DURATION, 0, &value);
        return value;
}

void TagList::setDuration(quint64 value)
{
    gst_tag_list_add(d->taglist, GST_TAG_MERGE_REPLACE_ALL, GST_TAG_DURATION, value, NULL);
}

QString TagList::codec() const
{
    return getStringTag(d->taglist, GST_TAG_CODEC, 0);
}

void TagList::setCodec(const QString & value)
{
    gst_tag_list_add_value(d->taglist, GST_TAG_MERGE_REPLACE_ALL,
                           GST_TAG_CODEC, QGlib::Value::create(value));
}

QString TagList::videoCodec() const
{
    return getStringTag(d->taglist, GST_TAG_VIDEO_CODEC, 0);
}

void TagList::setVideoCodec(const QString & value)
{
    gst_tag_list_add_value(d->taglist, GST_TAG_MERGE_REPLACE_ALL,
                           GST_TAG_VIDEO_CODEC, QGlib::Value::create(value));
}

QString TagList::audioCodec() const
{
    return getStringTag(d->taglist, GST_TAG_AUDIO_CODEC, 0);
}

void TagList::setAudioCodec(const QString & value)
{
    gst_tag_list_add_value(d->taglist, GST_TAG_MERGE_REPLACE_ALL,
                           GST_TAG_AUDIO_CODEC, QGlib::Value::create(value));
}

QString TagList::subtitleCodec() const
{
    return getStringTag(d->taglist, GST_TAG_SUBTITLE_CODEC, 0);
}

void TagList::setSubtitleCodec(const QString & value)
{
    gst_tag_list_add_value(d->taglist, GST_TAG_MERGE_REPLACE_ALL,
                           GST_TAG_SUBTITLE_CODEC, QGlib::Value::create(value));
}

QString TagList::containerFormat() const
{
    return getStringTag(d->taglist, GST_TAG_CONTAINER_FORMAT, 0);
}

void TagList::setContainerFormat(const QString & value)
{
    gst_tag_list_add_value(d->taglist, GST_TAG_MERGE_REPLACE_ALL,
                           GST_TAG_CONTAINER_FORMAT, QGlib::Value::create(value));
}

quint32 TagList::bitrate() const
{
    return getUintTag(d->taglist, GST_TAG_BITRATE, 0);
}

void TagList::setBitrate(quint32 value)
{
    gst_tag_list_add(d->taglist, GST_TAG_MERGE_REPLACE_ALL, GST_TAG_BITRATE, value, NULL);
}

quint32 TagList::nominalBitrate() const
{
    return getUintTag(d->taglist, GST_TAG_NOMINAL_BITRATE, 0);
}

void TagList::setNominalBitrate(quint32 value)
{
    gst_tag_list_add(d->taglist, GST_TAG_MERGE_REPLACE_ALL, GST_TAG_NOMINAL_BITRATE, value, NULL);
}

quint32 TagList::minimumBitrate() const
{
    return getUintTag(d->taglist, GST_TAG_MINIMUM_BITRATE, 0);
}

void TagList::setMinimumBitrate(quint32 value)
{
    gst_tag_list_add(d->taglist, GST_TAG_MERGE_REPLACE_ALL, GST_TAG_MINIMUM_BITRATE, value, NULL);
}

quint32 TagList::maximumBitrate() const
{
    return getUintTag(d->taglist, GST_TAG_MAXIMUM_BITRATE, 0);
}

void TagList::setMaximumBitrate(quint32 value)
{
    gst_tag_list_add(d->taglist, GST_TAG_MERGE_REPLACE_ALL, GST_TAG_MAXIMUM_BITRATE, value, NULL);
}

quint32 TagList::serial() const
{
    return getUintTag(d->taglist, GST_TAG_SERIAL, 0);
}

void TagList::setSerial(quint32 value)
{
    gst_tag_list_add(d->taglist, GST_TAG_MERGE_REPLACE_ALL, GST_TAG_SERIAL, value, NULL);
}

QString TagList::encoder() const
{
    return getStringTag(d->taglist, GST_TAG_ENCODER, 0);
}

void TagList::setEncoder(const QString & value)
{
    gst_tag_list_add_value(d->taglist, GST_TAG_MERGE_REPLACE_ALL,
                           GST_TAG_ENCODER, QGlib::Value::create(value));
}

quint32 TagList::encoderVersion() const
{
    return getUintTag(d->taglist, GST_TAG_ENCODER_VERSION, 0);
}

void TagList::setEncoderVersion(quint32 value)
{
    gst_tag_list_add(d->taglist, GST_TAG_MERGE_REPLACE_ALL, GST_TAG_ENCODER_VERSION, value, NULL);
}

double TagList::trackGain() const
{
    return getDoubleTag(d->taglist, GST_TAG_TRACK_GAIN, 0);
}

void TagList::setTrackGain(double value)
{
    gst_tag_list_add(d->taglist, GST_TAG_MERGE_REPLACE_ALL, GST_TAG_TRACK_GAIN, value, NULL);
}

double TagList::trackPeak() const
{
    return getDoubleTag(d->taglist, GST_TAG_TRACK_PEAK, 0);
}

void TagList::setTrackPeak(double value)
{
    gst_tag_list_add(d->taglist, GST_TAG_MERGE_REPLACE_ALL, GST_TAG_TRACK_PEAK, value, NULL);
}

double TagList::albumGain() const
{
    return getDoubleTag(d->taglist, GST_TAG_ALBUM_GAIN, 0);
}

void TagList::setAlbumGain(double value)
{
    gst_tag_list_add(d->taglist, GST_TAG_MERGE_REPLACE_ALL, GST_TAG_ALBUM_GAIN, value, NULL);
}

double TagList::albumPeak() const
{
    return getDoubleTag(d->taglist, GST_TAG_ALBUM_PEAK, 0);
}

void TagList::setAlbumPeak(double value)
{
    gst_tag_list_add(d->taglist, GST_TAG_MERGE_REPLACE_ALL, GST_TAG_ALBUM_PEAK, value, NULL);
}

double TagList::referenceLevel() const
{
    return getDoubleTag(d->taglist, GST_TAG_REFERENCE_LEVEL, 0);
}

void TagList::setReferenceLevel(double value)
{
    gst_tag_list_add(d->taglist, GST_TAG_MERGE_REPLACE_ALL, GST_TAG_REFERENCE_LEVEL, value, NULL);
}

QString TagList::languageCode() const
{
    return getStringTag(d->taglist, GST_TAG_LANGUAGE_CODE, 0);
}

void TagList::setLanguageCode(const QString & value)
{
    gst_tag_list_add_value(d->taglist, GST_TAG_MERGE_REPLACE_ALL,
                           GST_TAG_LANGUAGE_CODE, QGlib::Value::create(value));
}

SamplePtr TagList::image(int index) const
{
    return getSampleTag(d->taglist, GST_TAG_IMAGE, index);
}

void TagList::setImage(const SamplePtr & value, TagMergeMode mode)
{
    gst_tag_list_add_value(d->taglist, static_cast<GstTagMergeMode>(mode),
                           GST_TAG_IMAGE, QGlib::Value::create(value));
}

int TagList::imageCount() const
{
    return gst_tag_list_get_tag_size(d->taglist, GST_TAG_IMAGE);
}

SamplePtr TagList::previewImage() const
{
    return getSampleTag(d->taglist, GST_TAG_PREVIEW_IMAGE, 0);
}

void TagList::setPreviewImage(const SamplePtr & value)
{
    gst_tag_list_add_value(d->taglist, GST_TAG_MERGE_REPLACE_ALL,
                           GST_TAG_PREVIEW_IMAGE, QGlib::Value::create(value));
}

SamplePtr TagList::attachment(int index) const
{
    return getSampleTag(d->taglist, GST_TAG_ATTACHMENT, index);
}

void TagList::setAttachment(const SamplePtr & value, TagMergeMode mode)
{
    gst_tag_list_add_value(d->taglist, static_cast<GstTagMergeMode>(mode),
                           GST_TAG_ATTACHMENT, QGlib::Value::create(value));
}

int TagList::attachmentCount() const
{
    return gst_tag_list_get_tag_size(d->taglist, GST_TAG_ATTACHMENT);
}

double TagList::beatsPerMinute() const
{
    return getDoubleTag(d->taglist, GST_TAG_BEATS_PER_MINUTE, 0);
}

void TagList::setBeatsPerMinute(double value)
{
    gst_tag_list_add(d->taglist, GST_TAG_MERGE_REPLACE_ALL, GST_TAG_BEATS_PER_MINUTE, value, NULL);
}

QString TagList::keywords(int index) const
{
    return getStringTag(d->taglist, GST_TAG_KEYWORDS, index);
}

void TagList::setKeywords(const QString & value, TagMergeMode mode)
{
    gst_tag_list_add_value(d->taglist, static_cast<GstTagMergeMode>(mode),
                           GST_TAG_KEYWORDS, QGlib::Value::create(value));
}

int TagList::keywordsCount() const
{
    return gst_tag_list_get_tag_size(d->taglist, GST_TAG_KEYWORDS);
}

QString TagList::geoLocationName() const
{
    return getStringTag(d->taglist, GST_TAG_GEO_LOCATION_NAME, 0);
}

void TagList::seGeoLocationName(const QString & value)
{
    gst_tag_list_add_value(d->taglist, GST_TAG_MERGE_REPLACE_ALL,
                           GST_TAG_GEO_LOCATION_NAME, QGlib::Value::create(value));
}

double TagList::geoLocationLatitude() const
{
    return getDoubleTag(d->taglist, GST_TAG_GEO_LOCATION_LATITUDE, 0);
}

void TagList::setGeoLocationLatitude(double value)
{
    gst_tag_list_add(d->taglist, GST_TAG_MERGE_REPLACE_ALL,
                     GST_TAG_GEO_LOCATION_LATITUDE, value, NULL);
}

double TagList::geoLocationLongitude() const
{
    return getDoubleTag(d->taglist, GST_TAG_GEO_LOCATION_LONGITUDE, 0);
}

void TagList::setGeoLocationLongitude(double value)
{
    gst_tag_list_add(d->taglist, GST_TAG_MERGE_REPLACE_ALL,
                     GST_TAG_GEO_LOCATION_LONGITUDE, value, NULL);
}

double TagList::geoLocationElevation() const
{
    return getDoubleTag(d->taglist, GST_TAG_GEO_LOCATION_ELEVATION, 0);
}

void TagList::setGeoLocationElevation(double value)
{
    gst_tag_list_add(d->taglist, GST_TAG_MERGE_REPLACE_ALL,
                     GST_TAG_GEO_LOCATION_ELEVATION, value, NULL);
}

QString TagList::geoLocationCountry() const
{
    return getStringTag(d->taglist, GST_TAG_GEO_LOCATION_COUNTRY, 0);
}

void TagList::setGeoLocationCountry(const QString & value)
{
    gst_tag_list_add_value(d->taglist, GST_TAG_MERGE_REPLACE_ALL,
                           GST_TAG_GEO_LOCATION_COUNTRY, QGlib::Value::create(value));
}

QString TagList::geoLocationCity() const
{
    return getStringTag(d->taglist, GST_TAG_GEO_LOCATION_CITY, 0);
}

void TagList::setGeoLocationCity(const QString & value)
{
    gst_tag_list_add_value(d->taglist, GST_TAG_MERGE_REPLACE_ALL,
                           GST_TAG_GEO_LOCATION_CITY, QGlib::Value::create(value));
}

QString TagList::geoLocationSublocation() const
{
    return getStringTag(d->taglist, GST_TAG_GEO_LOCATION_SUBLOCATION, 0);
}

void TagList::setGeoLocationSublocation(const QString & value)
{
    gst_tag_list_add_value(d->taglist, GST_TAG_MERGE_REPLACE_ALL,
                           GST_TAG_GEO_LOCATION_SUBLOCATION, QGlib::Value::create(value));
}

double TagList::geoLocationMovementSpeed() const
{
    return getDoubleTag(d->taglist, GST_TAG_GEO_LOCATION_MOVEMENT_SPEED, 0);
}

void TagList::setGeoLocationMovementSpeed(double value)
{
    gst_tag_list_add(d->taglist, GST_TAG_MERGE_REPLACE_ALL,
                     GST_TAG_GEO_LOCATION_MOVEMENT_SPEED, value, NULL);
}

double TagList::geoLocationMovementDirection() const
{
    return getDoubleTag(d->taglist, GST_TAG_GEO_LOCATION_MOVEMENT_DIRECTION, 0);
}

void TagList::setGeoLocationMovementDirection(double value)
{
    gst_tag_list_add(d->taglist, GST_TAG_MERGE_REPLACE_ALL,
                     GST_TAG_GEO_LOCATION_MOVEMENT_DIRECTION, value, NULL);
}

double TagList::geoLocationCaptureDirection() const
{
    return getDoubleTag(d->taglist, GST_TAG_GEO_LOCATION_CAPTURE_DIRECTION, 0);
}

void TagList::setGeoLocationCaptureDirector(double value)
{
    gst_tag_list_add(d->taglist, GST_TAG_MERGE_REPLACE_ALL,
                     GST_TAG_GEO_LOCATION_CAPTURE_DIRECTION, value, NULL);
}

QString TagList::showName(int index) const
{
    return getStringTag(d->taglist, GST_TAG_SHOW_NAME, index);
}

void TagList::setShowName(const QString & value, TagMergeMode mode)
{
    gst_tag_list_add_value(d->taglist, static_cast<GstTagMergeMode>(mode),
                           GST_TAG_SHOW_NAME, QGlib::Value::create(value));
}

int TagList::showNameCount() const
{
    return gst_tag_list_get_tag_size(d->taglist, GST_TAG_SHOW_NAME);
}

QString TagList::showSortName() const
{
    return getStringTag(d->taglist, GST_TAG_SHOW_SORTNAME, 0);
}

void TagList::setShowSortName(const QString & value)
{
    gst_tag_list_add_value(d->taglist, GST_TAG_MERGE_REPLACE_ALL,
                           GST_TAG_SHOW_SORTNAME, QGlib::Value::create(value));
}

quint32 TagList::showEpisodeNumber() const
{
    return getUintTag(d->taglist, GST_TAG_SHOW_EPISODE_NUMBER, 0);
}

void TagList::setShowEpisodeNumber(quint32 value)
{
    gst_tag_list_add(d->taglist, GST_TAG_MERGE_REPLACE_ALL, GST_TAG_SHOW_EPISODE_NUMBER, value, NULL);
}

quint32 TagList::showSeasonNumber() const
{
    return getUintTag(d->taglist, GST_TAG_SHOW_SEASON_NUMBER, 0);
}

void TagList::setShowSeasonNumber(quint32 value)
{
    gst_tag_list_add(d->taglist, GST_TAG_MERGE_REPLACE_ALL, GST_TAG_SHOW_SEASON_NUMBER, value, NULL);
}

QString TagList::lyrics(int index) const
{
    return getStringTag(d->taglist, GST_TAG_LYRICS, index);
}

void TagList::setLyrics(const QString & value, TagMergeMode mode)
{
    gst_tag_list_add_value(d->taglist, static_cast<GstTagMergeMode>(mode),
                           GST_TAG_LYRICS, QGlib::Value::create(value));
}

int TagList::lyricsCount() const
{
    return gst_tag_list_get_tag_size(d->taglist, GST_TAG_LYRICS);
}

QString TagList::composerSortName() const
{
    return getStringTag(d->taglist, GST_TAG_COMPOSER_SORTNAME, 0);
}

void TagList::setComposerSortName(const QString & value)
{
    gst_tag_list_add_value(d->taglist, GST_TAG_MERGE_REPLACE_ALL,
                           GST_TAG_COMPOSER_SORTNAME, QGlib::Value::create(value));
}

QString TagList::grouping() const
{
    return getStringTag(d->taglist, GST_TAG_GROUPING, 0);
}

void TagList::setGrouping(const QString & value)
{
    gst_tag_list_add_value(d->taglist, GST_TAG_MERGE_REPLACE_ALL,
                           GST_TAG_GROUPING, QGlib::Value::create(value));
}

quint32 TagList::userRating() const
{
    return getUintTag(d->taglist, GST_TAG_USER_RATING, 0);
}

void TagList::setUserRating(quint32 value)
{
    gst_tag_list_add(d->taglist, GST_TAG_MERGE_REPLACE_ALL, GST_TAG_USER_RATING, value, NULL);
}

QString TagList::deviceManufacturer() const
{
    return getStringTag(d->taglist, GST_TAG_DEVICE_MANUFACTURER, 0);
}

void TagList::setDeviceManufacturer(const QString & value)
{
    gst_tag_list_add_value(d->taglist, GST_TAG_MERGE_REPLACE_ALL,
                           GST_TAG_DEVICE_MANUFACTURER, QGlib::Value::create(value));
}

QString TagList::deviceModel() const
{
    return getStringTag(d->taglist, GST_TAG_DEVICE_MODEL, 0);
}

void TagList::setDeviceModel(const QString & value)
{
    gst_tag_list_add_value(d->taglist, GST_TAG_MERGE_REPLACE_ALL,
                           GST_TAG_DEVICE_MODEL, QGlib::Value::create(value));
}

QString TagList::imageOrientation() const
{
    return getStringTag(d->taglist, GST_TAG_IMAGE_ORIENTATION, 0);
}

void TagList::setImageOrientation(const QString & value)
{
    gst_tag_list_add_value(d->taglist, GST_TAG_MERGE_REPLACE_ALL,
                           GST_TAG_IMAGE_ORIENTATION, QGlib::Value::create(value));
}

double TagList::geoLocationHorizontalError() const
{
    return getDoubleTag(d->taglist, GST_TAG_GEO_LOCATION_HORIZONTAL_ERROR, 0);
}

void TagList::setGeoLocationHorizontalError(double value)
{
    gst_tag_list_add(d->taglist, GST_TAG_MERGE_REPLACE_ALL, GST_TAG_GEO_LOCATION_HORIZONTAL_ERROR, value, NULL);
}

QString TagList::applicationName() const
{
    return getStringTag(d->taglist, GST_TAG_APPLICATION_NAME, 0);
}

void TagList::setApplicationName(const QString & value)
{
    gst_tag_list_add_value(d->taglist, GST_TAG_MERGE_REPLACE_ALL,
                           GST_TAG_APPLICATION_NAME, QGlib::Value::create(value));
}

SamplePtr TagList::applicationData() const
{
    return getSampleTag(d->taglist, GST_TAG_APPLICATION_DATA, 0);
}

void TagList::setApplicationData(const SamplePtr & value)
{
    gst_tag_list_add_value(d->taglist, GST_TAG_MERGE_REPLACE_ALL,
                           GST_TAG_APPLICATION_DATA, QGlib::Value::create(value));
}

QDateTime TagList::dateTime() const
{
    return tagValue(GST_TAG_DATE_TIME).get<QDateTime>();
}

void TagList::setDateTime(const QDateTime & value)
{
    gst_tag_list_add_value(d->taglist, GST_TAG_MERGE_REPLACE_ALL,
                           GST_TAG_DATE_TIME, QGlib::Value::create(value));
}


QDebug operator<<(QDebug debug, const TagList & taglist)
{
    debug.nospace() << "QGst::TagList("
                    << QGlib::Private::stringFromGCharPtr(gst_tag_list_to_string(taglist)) << ")";
    return debug.space();
}

} //namespace QGst
