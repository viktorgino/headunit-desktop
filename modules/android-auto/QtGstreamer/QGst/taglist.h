/*
    Copyright (C) 2010  Collabora Multimedia.
      @author Mauricio Piacentini <mauricio.piacentini@collabora.co.uk>

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
#ifndef QGST_TAGLIST_H
#define QGST_TAGLIST_H

#include "global.h"
#include "../QGlib/type.h"
#include "../QGlib/value.h"
#include <QtCore/QString>

class QDate;
class QDateTime;

namespace QGst {

/*! \headerfile taglist.h <QGst/TagList>
 * \brief Wrapper class for GstTagList
 *
 * TagList is a collection of tags and values that is associated with TagMessages and TagEvents. used to describe
 * metadata in media files.
 *
 * Tags are usually set with the helper functions:
 * \code
 * TagList tl();
 * tl.setTitle("12th Symphony");
 * tl.setAuthor("Beethoven");
 * tl.setTrackNumber(1);
 * \endcode
 *
 * And retrieved as native Qt types in a similar way:
 * \code
 * qDebug() << tl.title();
 * int track = tl.trackNumber();
 * \endcode
 *
 * Some tags accept multiple values. By default only the first value is returned, but it is possible to determine
 * the number of values available and retrieve a specific one using its index:
  * \code
 * int authors = tl.authorCount();
 * for (int i=0; i<authors; i++) {
 *      myAuthorList << tl.author(i);
 * }
 * \endcode
 *
 * It is also possible to use the optional TagMerge parameters when setting tags that accept multiple values:
  * \code
 * tl.setAuthor("John");
 * tl.setAuthor("Mary", QGst::TagMergeAppend); //now tl.authorCount() equals 2
 * tl.setAuthor("George", QGst::TagMergeReplaceAll); //now there is only one author, "George"
 * \endcode
 *
 * \note This class is implicitly shared.
 */

class QTGSTREAMER_EXPORT TagList
{
public:
    TagList();
    TagList(const GstTagList *taglist);
    TagList(const TagList & other);
    virtual ~TagList();

    TagList & operator=(const TagList & other);

    bool isEmpty() const;

    void insert(const TagList & other, TagMergeMode mode = TagMergeAppend);
    static TagList merge(const TagList & firstList, const TagList & secondList,
                         TagMergeMode mode = TagMergeAppend);

    QGlib::Value tagValue(const char *tag, int index = 0) const;
    void setTagValue(const char *tag, const QGlib::Value & value,
                     TagMergeMode mode = TagMergeReplaceAll);
    int tagValueCount(const char *tag) const;

    void clear();
    void removeTag(const char *tag);

    operator GstTagList*();
    operator const GstTagList*() const;

    //Begin helpers

    QString title(int index = 0) const;
    void setTitle(const QString & value, TagMergeMode mode = TagMergeReplaceAll);
    int titleCount() const;

    QString titleSortName() const;
    void setTitleSortName(const QString & value);

    QString artist(int index = 0) const;
    void setArtist(const QString & value, TagMergeMode mode = TagMergeReplaceAll);
    int artistCount() const;

    QString artistSortName() const;
    void setArtistSortName(const QString & value);

    QString composer(int index = 0) const;
    void setComposer(const QString & value, TagMergeMode mode = TagMergeReplaceAll);
    int composerCount() const;

    QDate date() const;
    void setDate(const QDate & value);

    QString genre(int index = 0) const;
    void setGenre(const QString & value, TagMergeMode mode = TagMergeReplaceAll);
    int genreCount() const;

    QString comment(int index = 0) const;
    void setComment(const QString & value, TagMergeMode mode = TagMergeReplaceAll);
    int commentCount() const;

    QString extendedComment(int index = 0) const;
    void setExtendedComment(const QString & value, TagMergeMode mode = TagMergeReplaceAll);;
    int extendedCommentCount() const;

    quint32 trackNumber() const;
    void setTrackNumber(quint32 value);

    quint32 trackCount() const;
    void setTrackCount(quint32 value);

    quint32 albumVolumeNumber() const;
    void setAlbumVolumeNumber(quint32 value);

    quint32 albumVolumeCount() const;
    void setAlbumVolumeCount(quint32 value);

    QString location(int index = 0) const;
    void setLocation(const QString & value, TagMergeMode mode = TagMergeReplaceAll);
    int locationCount() const;

    QString homepage(int index = 0) const;
    void setHomepage(const QString & value, TagMergeMode mode = TagMergeReplaceAll);
    int homepageCount() const;

    QString description(int index = 0) const;
    void setDescription(const QString & value, TagMergeMode mode = TagMergeReplaceAll);
    int descriptionCount() const;

    QString version() const;
    void setVersion(const QString & value);

    QString isrc() const;
    void setIsrc(const QString & value);

    QString organization(int index = 0) const;
    void setOrganization(const QString & value, TagMergeMode mode = TagMergeReplaceAll);
    int organizationCount() const;

    QString copyright() const;
    void setCopyright(const QString & value);

    QString copyrightUri() const;
    void setCopyrightUri(const QString & value);

    QString contact(int index = 0) const;
    void setContact(const QString & value, TagMergeMode mode = TagMergeReplaceAll);
    int contactCount() const;

    QString license() const;
    void setLicense(const QString & value);

    QString licenseUri() const;
    void setLicenseUri(const QString & value);

    QString performer(int index = 0) const;
    void setPerformer(const QString & value, TagMergeMode mode = TagMergeReplaceAll);
    int performerCount() const;

    quint64 duration() const;
    void setDuration(quint64 value);

    QString codec() const;
    void setCodec(const QString & value);

    QString videoCodec() const;
    void setVideoCodec(const QString & value);

    QString audioCodec() const;
    void setAudioCodec(const QString & value);

    QString subtitleCodec() const;
    void setSubtitleCodec(const QString & value);

    QString containerFormat() const;
    void setContainerFormat(const QString & value);

    quint32 bitrate() const;
    void setBitrate(quint32 value);

    quint32 nominalBitrate() const;
    void setNominalBitrate(quint32 value);

    quint32 minimumBitrate() const;
    void setMinimumBitrate(quint32 value);

    quint32 maximumBitrate() const;
    void setMaximumBitrate(quint32 value);

    quint32 serial() const;
    void setSerial(quint32 value);

    QString encoder() const;
    void setEncoder(const QString & value);

    quint32 encoderVersion() const;
    void setEncoderVersion(quint32 value);

    double trackGain() const;
    void setTrackGain(double value);

    double trackPeak() const;
    void setTrackPeak(double value);

    double albumGain() const;
    void setAlbumGain(double value);

    double albumPeak() const;
    void setAlbumPeak(double value);

    double referenceLevel() const;
    void setReferenceLevel(double value);

    QString languageCode() const;
    void setLanguageCode(const QString & value);

    SamplePtr image(int index = 0) const;
    void setImage(const SamplePtr & value, TagMergeMode mode = TagMergeReplaceAll);
    int imageCount() const;

    SamplePtr previewImage() const;
    void setPreviewImage(const SamplePtr & value);

    SamplePtr attachment(int index = 0) const;
    void setAttachment(const SamplePtr & value, TagMergeMode mode = TagMergeReplaceAll);
    int attachmentCount() const;

    double beatsPerMinute() const;
    void setBeatsPerMinute(double value);

    QString keywords(int index = 0) const;
    void setKeywords(const QString & value, TagMergeMode mode = TagMergeReplaceAll);
    int keywordsCount() const;

    QString geoLocationName() const;
    void seGeoLocationName(const QString & value);

    double geoLocationLatitude() const;
    void setGeoLocationLatitude(double value);

    double geoLocationLongitude() const;
    void setGeoLocationLongitude(double value);

    double geoLocationElevation() const;
    void setGeoLocationElevation(double value);

    QString geoLocationCountry() const;
    void setGeoLocationCountry(const QString & value);

    QString geoLocationCity() const;
    void setGeoLocationCity(const QString & value);

    QString geoLocationSublocation() const;
    void setGeoLocationSublocation(const QString & value);

    double geoLocationMovementSpeed() const;
    void setGeoLocationMovementSpeed(double value);

    double geoLocationMovementDirection() const;
    void setGeoLocationMovementDirection(double value);

    double geoLocationCaptureDirection() const;
    void setGeoLocationCaptureDirector(double value);

    QString showName(int index = 0) const;
    void setShowName(const QString & value, TagMergeMode mode = TagMergeReplaceAll);
    int showNameCount() const;

    QString showSortName() const;
    void setShowSortName(const QString & value);

    quint32 showEpisodeNumber() const;
    void setShowEpisodeNumber(quint32 value);

    quint32 showSeasonNumber() const;
    void setShowSeasonNumber(quint32 value);

    QString lyrics(int index = 0) const;
    void setLyrics(const QString & value, TagMergeMode mode = TagMergeReplaceAll);
    int lyricsCount() const;

    QString composerSortName() const;
    void setComposerSortName(const QString & value);

    QString grouping() const;
    void setGrouping(const QString & value);

    quint32 userRating() const;
    void setUserRating(quint32 value);

    QString deviceManufacturer() const;
    void setDeviceManufacturer(const QString & value);

    QString deviceModel() const;
    void setDeviceModel(const QString & value);

    QString imageOrientation() const;
    void setImageOrientation(const QString & value);

    QString applicationName() const;
    void setApplicationName(const QString & value);

    SamplePtr applicationData() const;
    void setApplicationData(const SamplePtr & value);

    QDateTime dateTime() const;
    void setDateTime(const QDateTime & value);

    double geoLocationHorizontalError() const;
    void setGeoLocationHorizontalError(double value);

private:
    struct Data;
    QSharedDataPointer<Data> d;
};

/*! \relates QGst::TagList */
QTGSTREAMER_EXPORT QDebug operator<<(QDebug debug, const TagList & taglist);

} //namespace QGst

QGST_REGISTER_TYPE(QGst::TagList)

#endif
