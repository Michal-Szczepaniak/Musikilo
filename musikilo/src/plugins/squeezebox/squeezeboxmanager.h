#ifndef SQUEEZEBOXMANAGER_H
#define SQUEEZEBOXMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>

class SqueezeBoxManager : public QObject
{
    Q_OBJECT
public:
    enum Command {
        MUSICFOLDER,
        PLAYER_ID,
        STATUS,
        PLAYERS,
        PLAYLIST,
        ADD_FILE,
        PLAY_FILE,
        PLAY_INDEX,
        NEXT,
        PREV,
        CLEAR,
        PLAY,
        PAUSE,
        STOP,
        SEEK,
        SHUFFLE,
        REPEAT,
        RESCAN,
        VOLUME,
        SYNC,
        DELETE,
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
        int id;
        FileType type;
        QString url;
    };

    struct Player {
        QString name;
        QString id;
    };

    struct Song {
        int index;
        int id;
        QString title;
        QString album;
        QString artist;
        QString url;
        int trackNr;
    };

    struct Status {
        QString mode;
        double position;
        double duration;
        int volume;
        bool repeat;
        bool single;
        bool shuffle;
        int currentIndex;
        int tracks;
        Song song;
    };

    explicit SqueezeBoxManager(QObject *parent = nullptr);

    void setConnectionSettings(QString host, int port);

    void getFilesList(QString path);
    void getMusicFolder();
    void getStatus();
    void getPlayers(int count = 0);
    void setActivePlayer(QString id);
    void getPlaylist(int quantity);
    void addFile(QString url);
    void playFile(QString url);
    void play(int index);
    void nextSong();
    void prevSong();
    void clear();
    void play();
    void pause();
    void stop();
    void seek(qint64 position);
    void setShuffle(bool shuffle);
    void setRepeat(bool repeat);
    void setSingle(bool single);
    void refreshDatabase();
    void setVolume(int volume);
    void getSyncPlayers();
    void syncPlayer(QString id);
    void desyncPlayer(QString id);
    void remove(int index);
    void getVersion();

signals:
    void errorOccured(QString error);
    void gotMusicFolder(QVector<File> folder);
    void gotTopMusicFolder(File folder);
    void gotStatus(Status status);
    void gotPlayers(QVector<Player> players);
    void gotPlaylist(QVector<Song> playlist);
    void gotSyncPlayers(QStringList players);
    void gotVersion();

public slots:
    void update();

protected slots:
    void onRequestFinished(QNetworkReply *reply);

private:
    int _requestId = 1;
    QString _host;
    int _port;
    QNetworkAccessManager _manager;
    QString _currentPlayerId{};

    void makeRequest(Command command, QString playerId = "0", QStringList parameters = {});
    QStringList getCommand(Command command);
    QMap<int, Request> _requests{};
    FileType fileTypeFromString(QString type);
    QString fileTypeName(FileType type);

    void onGotPlayerId(QJsonObject result, QStringList params);
    void onGotMusicFolder(QJsonObject result, QStringList params);
    void onGotStatus(QJsonObject result);
    void onGotPlayers(QJsonObject result);
    void onGotPlaylist(QJsonObject result);
    void onGotSyncPlayers(QJsonObject result, QStringList params);
};

#endif // SQUEEZEBOXMANAGER_H
