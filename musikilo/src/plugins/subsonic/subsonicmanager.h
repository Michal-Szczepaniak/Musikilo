#ifndef SUBSONICMANAGER_H
#define SUBSONICMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include <QUrlQuery>
#include <QDomDocument>

class SubsonicManager : public QObject
{
    Q_OBJECT
public:
    enum Command {
        PING,
        INDEXES,
        MUSIC_DIRECTORY,
        LIST_DIRECTORY,
        SONG,
        FILE,
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
        QString id;
        QString title;
        QString album;
        QString artist;
        QString track;
    };

    explicit SubsonicManager(QObject *parent = nullptr);

    void setConnectionSettings(QString host, int port, QString username, QString password, int connectionType);

    void getFilesList(QString path);
    void listDirectory(QString path);
    QNetworkRequest getStreamRequest(QString id);
    void getSong(QString id);
    void ping();

signals:
    void errorOccured(QString error);
    void gotPing();
    void gotFileList(QVector<File> folder);
    void gotTrackList(QList<PlaylistItem> list);
    void gotSong(PlaylistItem song);

protected slots:
    void onRequestFinished(QNetworkReply *reply);

private:
    int _requestId = 1;
    QString _host;
    int _port;
    QString _username;
    QString _password;
    int _connectionType;
    QNetworkAccessManager _manager;
    QVector<File> _filesCache;

    void makeRequest(Command command, QUrlQuery parameters = {});
    QString getCommandPath(Command command);
    FileType fileTypeFromString(QString type);
    QString fileTypeName(FileType type);

    void onGotIndexes(QDomDocument result);
    void onGotMusicDirectory(QDomDocument result);
    void onListDirectory(QDomDocument result);
    void onGotSong(QDomDocument result);
    void onGotFile(QDomDocument result);
};

#endif // SUBSONICMANAGER_H
