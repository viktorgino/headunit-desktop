#ifndef MEDIADB_H
#define MEDIADB_H

#include <QObject>
#include <QtSql>
#include <QSqlRecord>
#include <QString>
#include <QStringList>
#include <QVariant>

/**
 * @brief Database abstraction class, used to fetch and insert/update data to/from the media library's SQLITE database
 *
 * QVariantMap and QVariantList is used as parameters and return types for convenience so we can easily pass data from and to QML
 * The media library currently uses 3 tables: locations, scanned_folders and media_files. Their purpose are as follows:
 *
 * ###locations###
 * We store the locations that are added to the database. Locations are used as the root for media files that are scanned and added
 * to the DB. To add a new location we have to first find the root for the partition its located at. The root of the partition will be stored
 * in the 'volume_path' filed. Then we have to find a unique identifier for the partition which we will store in the 'volume_unique_id' field.
 * The 'relative_path' is used to store the added locations path relative to the volume's root. This is done this way so if the mounting point
 * for the drive changes we can change it simply without having to delete the location and completely rescan it. This will also help us later
 * when implementing separated database files for each partition.
 *
 *   - name (text) - Used to store the drive label. This field is purely for display purposes.
 *   - relative_path (text) - The path to the location relative to the mounting point of the volume
 *   - volume_unique_id (text) - The UUID of the volume (on Linux this may be found in /dev/disk/by-uuid)
 *   - volume_path (text) - The path to the volume (on Linux this is the mount point on Windows its the drive letter)
 *   - is_present (integer) - This indicates whether if the volume is mounted and accessible or not. Possible values : 1 (true) | 0 (false). If this field is set
 * to 0 the media files and folders from this location will be excluded from all queries.
 *
 * ###scanned_folders###
 * This table is used to store the folders that were found and scanned within a location. The fields are used as follows:
 *   - name  (text) - The name of the folder
 *   - location_id  (integer) - The foreign key for the location this folder is located in.
 *   - relative_path  (text) - The path to the folder relative to the location path (this excludes the folder name)
 *   - last_modified (text) - The last modified stamp of the folder (currently not used)
 *   - has_audio (integer) - Indicates whether if there are any audio files directly in the folder. possible values : 1 (true) | 0 (false)
 *   - has_video (integer) - Indicates whether if there are any video files directly in the folder. possible values : 1 (true) | 0 (false)
 *   - thumbnail (text) - The name of the thumbnail image
 *
 * ###media_files###
 * This table contains the media files that were found in the scanned folders.
 *   - filename (text) - The name of the media file including the file type extension
 *   - folder_id (text) - The foreign key for the folder that contains the media file.
 *   - media_type (integer) - The media type id. Possible values are: 1 (AUDIO) | 2 (VIDEO) | 3 (PLAYLIST)
 *   - artist (text) (optional, only for audio files) - The performing artist of the audio file (parsed from i3d)
 *   - title (text) (optional, only for audio files) - The title of the audio file (parsed from i3d)
 *   - album (text) (optional, only for audio files) - The album of the audio file (parsed from i3d)
 *   - genre (text) (optional, only for audio files) - The genre of the audio file (parsed from i3d)
 *
 *
 * @todo Create separate SQLITE databases for each partition and store it in the volume root rather than a single file next to executable
 * @todo Add favourites
 * @todo Add custom playlists
 *
 */
class MediaDB : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Open the SQLITE DB file and checks if all the tables are created, if not it creates them.
     *
     * @param parent
     */
    explicit MediaDB(QObject *parent = 0);
    /**
     * @brief Check if a location already exits in the database and if not then adds the location to the database
     *
     * @param name Name of the location
     * @param v_unique_id UUID of the volume the location is at
     * @param v_path The path for the volume (mounting point or drive letter)
     * @param relative_path The path to the location relative to the root of the partition
     * @return Returns -1 if there is some error with accessing the database or binding values to the prepared statement,
     * return -2 if the location exits. Otherwise it returns the ID of the newly created locations
     */
    int addLocation(QString name, QString v_unique_id, QString v_path, QString relative_path);
    /**
     * @brief Check if a folder is in the database if not then adds it to the DB.
     *
     * @param location_id
     * @param name
     * @param relative_path
     * @param last_modified
     * @param thumbnail
     * @return Returns the ID of the folder matching the passed parameters (it queries the location and if it exists return its ID
     * if it is not in the DB yet it creates it and then returns its ID). Returns -1 if there is some error with accessing the database
     * or binding values to the prepared statement.
     */
    int addScannedFolder(int location_id, QString name, QString relative_path, qint64 last_modified, QString thumbnail);
    /**
     * @brief Add a batch of media files to the database
     *
     * Please note that all the parameters are lists, so the number of items should be the same on all the lists.
     *
     * @param filenames A list of filenames
     * @param folder_id A list with the id of the folder the media files are located in
     * @param media_types A list with the media type ({@link AUDIO}, {@link VIDEO}, {@link PLAYLIST})
     * @param artist A list with artists for the scanned media (only used for audio, for other media types pass an empty string)
     * @param title A list with titles for the scanned media (only used for audio, for other media types pass an empty string)
     * @param album A list with albums for the scanned media (only used for audio, for other media types pass an empty string)
     * @param genre A list with genres for the scanned media (only used for audio, for other media types pass an empty string)
     * @return Returns -1 if there is some error with accessing the database or binding values to the prepared statement. If the
     * folder is added successfully it returns 0.
     */
    int addMediaFiles(QVariantList filenames, QVariantList folder_id, QVariantList media_types, QVariantList artist, QVariantList title, QVariantList album, QVariantList genre);
    /**
     * @brief Fetch the info for the location that matches location_id
     *
     * The structure of the map is the following :
     *
     *     {
     *         name : QString,
     *         relative_path: QString,
     *         volume_path: QString
     *     }
     *
     * @param location_id The id of the record to fetch
     * @return Returns an associative array (QVariantMap) with location info. If there isn't a matching record for location_id or there
     * is an error with the query it returns an empty QVariantMap
     */
    QVariantMap getLocationInfo(int location_id);
    /**
     * @brief Update the has_audio, has_video and thumbnail fields for the folder matched by folder_id
     * @param folder_id
     * @param hasAudio
     * @param hasVideo
     * @param thumbnail
     * @return If the row with location_id is successfully updated it returns 0. If there is an SQL error it returns -1.
     */
    int updateFolderInfo(int folder_id, bool hasAudio, bool hasVideo, QString thumbnail);
    /**
     * @brief Set the availability of location_id to isAvailable
     *
     * When the availability of a location is set to true the media files and folders from that location will show up on all the
     * lists ...etc, set the availability of a location to false whenever it is unmounted/not available and it won't cause any error for
     * the media player.
     * @param location_id
     * @param isAvailable
     * @return If the location availability  is successfully updated it returns 0. If there is an SQL error it returns -1.
     */
    int setLocationAvailability(int location_id, bool isAvailable);
    /**
     * @brief Fetch a list of locations
     *
     * @param onlyAvailable Set this to true to only get available locations
     * @return QVariantList
     */
    QVariantList getLocations(bool onlyAvailable);
    /**
     * @brief
     *
     * @param q
     * @return QVariantList
     */
    QVariantList getListFromQuery(QSqlQuery q);
    /**
     * @brief
     *
     * @param q
     * @return QVariantMap
     */
    QVariantMap getListWithFirstLetterFromQuery(QSqlQuery q);
    /**
     * @brief
     *
     * @param mediaType
     * @return QVariantMap
     */
    QVariantMap getMediaFolders(int mediaType);
    /**
     * @brief
     *
     * @param folder_id
     * @param mediaType
     * @return QVariantMap
     */
    QVariantMap getFolderContent(int folder_id, int mediaType);
    /**
     * @brief
     *
     * @param album
     * @return QVariantMap
     */
    QVariantMap getAlbumContent(QString album);
    /**
     * @brief
     *
     * @return QVariantMap
     */
    QVariantMap getPlaylists();
    /**
     * @brief
     *
     * @return QVariantMap
     */
    QVariantMap getAudioFiles();
    /**
     * @brief Contains all the possible list types that we can fetch from the database (albums,artists,genres)
     *
     * @todo Remove playlists and songs
     */
    enum ListType {albums,artists,genres,playlists,songs};

    /**
     * @brief
     *
     * @param listType
     * @return QVariantMap
     */
    QVariantMap getList(ListType listType);
    /**
     * @brief
     *
     * @param listType
     * @param key
     * @return QVariantMap
     */
    QVariantMap getListContent(ListType listType, QString key);

    static const int AUDIO = 1; /**< Media type id used for AUDIO files */
    static const int VIDEO = 2; /**< Media type id used for VIDEO files */
    static const int PLAYLIST = 3; /**< Media type id used for PLAYLIST files */
private:
    QSqlDatabase db; /**< TODO: describe */
signals:

public slots:
};

#endif // MEDIADB_H
