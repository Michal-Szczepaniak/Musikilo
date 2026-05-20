#ifndef TAUONMANAGER_H
#define TAUONMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>

class TauonManager : public QObject
{
    Q_OBJECT
public:
    enum Command {
        TRACKLIST,
        PLAYLISTS,
        STATUS,
        PLAY_INDEX,
        NEXT,
        PREV,
        PLAY,
        PAUSE,
        SEEK,
        SHUFFLE,
        REPEAT,
        VOLUME,
        VERSION,
    };

    struct Request {
        Command command;
        QStringList params;
    };

    enum FileType {
        Folder,
        Track,
        Unknown,
    };

    struct File {
        QString name;
        QString path;
        int id;
        FileType type;
    };

    struct Playlist {
        QString name;
        int id;
    };

    struct Song {
        int index;
        int id;
        QString title;
        QString artist;
        QString album;
        QString path;
        QString trackNumber;
    };

    struct Status {
        QString status;
        bool shuffle;
        bool repeat;
        int progress;
        bool autoStop;
        int volume;
        int playlist;
        int currentTrackId;
        QString title;
        QString artist;
        QString album;
        int position;
        int duration;
    };

    explicit TauonManager(QObject *parent = nullptr);

    void setConnectionSettings(QString host, int port);

    void getTrackList();
    QVector<File> getFilesList(QString path);
    void getPlaylists();
    void getStatus();
    void play(int index);
    void playPath(QString path);
    void nextSong();
    void prevSong();
    void play();
    void pause();
    void seek(qint64 position);
    void setShuffle(bool shuffle);
    void setRepeat(bool repeat);
    void setVolume(int volume);
    void getVersion();

    void setActivePlaylist(int id);

signals:
    void errorOccured(QString error);
    void gotTracklist(QVector<Song> list);
    void gotStatus(Status status);
    void gotPlaylists(QVector<Playlist> playlists);
    void gotSyncPlayers(QStringList players);
    void gotVersion();

public slots:
    void update();

protected slots:
    void onRequestFinished(QNetworkReply *reply);

private:
    QString _host;
    int _port;
    QNetworkAccessManager _manager;
    int _currentPlaylistId{};
    QVector<Song> _currentTracklist{};
    QMap<QString, QVector<File>> _trackTree{};

    void makeRequest(Command command, QStringList parameters = {});
    QString getCommandPath(Command command);
    FileType fileTypeFromString(QString type);
    QString fileTypeName(FileType type);

    void onGotTracklist(QJsonObject result);
    void onGotStatus(QJsonObject result);
    void onGotPlaylists(QJsonObject result);
};

#endif // TAUONMANAGER_H
