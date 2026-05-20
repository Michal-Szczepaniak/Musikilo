#ifndef KODIMANAGER_H
#define KODIMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>

class KodiManager : public QObject
{
    Q_OBJECT
public:
    enum Command {
        PING,
        PLAY,
        PAUSE,
        STOP,
        GOTO,
        SEEK,
        SHUFFLE,
        REPEAT,
        PLAYER_PROPETIRES,
        PLAYER_ITEM,
        GET_VOLUME,
        SET_VOLUME,
        PLAYLIST,
        PLAYER_OPEN,
        FILELIST,
        SOURCES,
        REMOVE,
        ADD,
        PLAY_FILE,
        CLEAR,
        AUDIO_STREAM,
        VIDEO_STREAM,
        SUBTITLE,
    };

    struct Request {
        Command command;
        QJsonObject params;
    };

    enum FileType {
        Folder,
        Track,
        Unknown,
    };

    struct File {
        QString name;
        QString path;
        FileType type;
    };

    struct PlaylistItem {
        int index;
        int id;
        QString title;
    };

    struct Status {
        bool playing;
        bool shuffle;
        bool repeat;
        bool single;
        int progress;
        int duration;
        int volume;
        int position;
        QString title;
        QString tagline;
        int currentAudioStream;
        int currentVideoStream;
        int currentSubtitle;
        QStringList audioStreams;
        QStringList videoStreams;
        QStringList subtitles;
    };

    explicit KodiManager(QObject *parent = nullptr);

    void setConnectionSettings(QString host, int port);

    void getFilesList(QString path);
    void getPlaylist();
    void getSources();
    void getStatus();
    void play(int index);
    void addFile(QString url);
    void playFile(QString url);
    void nextSong();
    void prevSong();
    void remove(int index);
    void clearPlaylist();
    void play();
    void pause();
    void stop();
    void seek(qint64 position);
    void setShuffle(bool shuffle);
    void setRepeat(bool repeat);
    void setSingle(bool single);
    void setVolume(int volume);
    void setAudioStream(int index);
    void setVideoStream(int index);
    void setSubtitle(int index);
    void ping();

signals:
    void errorOccured(QString error);
    void gotPing();
    void gotStatus(Status status);
    void gotFileList(QVector<File> folder);
    void gotSources(QVector<File> folder);
    void gotPlaylist(QVector<PlaylistItem> playlist);

public slots:
    void update();

protected slots:
    void onRequestFinished(QNetworkReply *reply);

private:
    int _requestId = 1;
    QString _host;
    int _port;
    QNetworkAccessManager _manager;
    Status _status{};

    void makeRequest(Command command, QJsonObject parameters = {});
    QString getCommand(Command command);
    QMap<int, Request> _requests{};
    FileType fileTypeFromString(QString type);
    QString fileTypeName(FileType type);

    void onGotVolume(QJsonObject result);
    void onGotPlayerProperties(QJsonObject result);
    void onGotPlayerItem(QJsonObject result);
    void onGotPlaylist(QJsonObject result);
    void onGotSources(QJsonObject result);
    void onGotFileList(QJsonObject result);
};

#endif // KODIMANAGER_H
