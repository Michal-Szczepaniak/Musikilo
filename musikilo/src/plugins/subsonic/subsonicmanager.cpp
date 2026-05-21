#include "subsonicmanager.h"

#include <QFileInfo>
#include <QJsonArray>
#include <QTime>
#include <QUuid>
#include <QVector>

SubsonicManager::SubsonicManager(QObject *parent) : QObject(parent), _manager(this)
{
    connect(&_manager, &QNetworkAccessManager::finished, this, &SubsonicManager::onRequestFinished);
}

void SubsonicManager::setConnectionSettings(QString host, int port, QString username, QString password, int connectionType)
{
    _host = host;
    _port = port;
    _username = username;
    _password = password;
    _connectionType = connectionType;
}

void SubsonicManager::getFilesList(QString path)
{
    if (path == "/") {
        makeRequest(INDEXES);
        return;
    }

    QUrlQuery q;
    q.addQueryItem("id", path);
    makeRequest(MUSIC_DIRECTORY, q);
}

void SubsonicManager::listDirectory(QString path)
{
    QUrlQuery q;
    q.addQueryItem("id", path);

    for (const File &file : _filesCache) {
        if (file.path == path) {
            makeRequest(FILE, q);
            return;
        }
    }

    makeRequest(LIST_DIRECTORY, q);
}

QNetworkRequest SubsonicManager::getStreamRequest(QString id)
{
    QString path = "/rest/stream";

    QUrlQuery parameters;
    parameters.addQueryItem("u", _username);
    QString salt = QUuid::createUuid().toString().remove('{').remove('}').left(8);
    parameters.addQueryItem("s", salt);
    parameters.addQueryItem("t", QCryptographicHash::hash((_password + salt).toLatin1(), QCryptographicHash::Md5).toHex());
    parameters.addQueryItem("v", "1.13.0");
    parameters.addQueryItem("c", "Musikilo");
    parameters.addQueryItem("id", id);

    QUrl url;
    url.setHost(_host);
    url.setPort(_port);
    url.setPath(path);
    url.setQuery(parameters);
    url.setScheme(_connectionType == 1 ? "https" : "http");

    QNetworkRequest request(url);
    return request;
}

void SubsonicManager::getSong(QString id)
{
    QUrlQuery q;
    q.addQueryItem("id", id);
    makeRequest(SONG, q);
}

void SubsonicManager::ping()
{
    makeRequest(PING);
}

void SubsonicManager::onRequestFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Reply error: " << reply->errorString();
        emit errorOccured(reply->errorString());
        return;
    }

    QString error;
    QString xml = reply->readAll();

    QDomDocument doc;
    doc.setContent(xml, &error);
    if (!error.isEmpty()) {
        emit errorOccured(error);
        qDebug() << "Error: " << error;
        return;
    }

    QDomNodeList errorNodes = doc.elementsByTagName("error");

    if (!errorNodes.isEmpty()) {
        QDomElement errorElement = errorNodes.at(0).toElement();
        qDebug() << "Error: " << errorElement.attribute("message");
        emit errorOccured(errorElement.attribute("message"));
        return;
    }

    qDebug() << xml;

    int command = QUrlQuery(reply->url().query()).queryItemValue("cid").toInt();
    QString prefix = "/rest/";

    switch (command) {
    case PING:
        emit gotPing();
        return;
    case INDEXES:
        emit onGotIndexes(doc);
        return;
    case MUSIC_DIRECTORY:
        emit onGotMusicDirectory(doc);
        return;
    case LIST_DIRECTORY:
        emit onListDirectory(doc);
        return;
    case SONG:
        emit onGotSong(doc);
        return;
    case FILE:
        emit onGotFile(doc);
        return;
    }
}

void SubsonicManager::makeRequest(Command command, QUrlQuery parameters)
{
    QString path = "/rest/" + getCommandPath(command);

    parameters.addQueryItem("u", _username);
    QString salt = QUuid::createUuid().toString().remove('{').remove('}').left(8);
    parameters.addQueryItem("s", salt);
    parameters.addQueryItem("t", QCryptographicHash::hash((_password + salt).toLatin1(), QCryptographicHash::Md5).toHex());
    parameters.addQueryItem("v", "1.13.0");
    parameters.addQueryItem("c", "Musikilo");
    parameters.addQueryItem("cid", QString::number(command));

    QUrl url;
    url.setHost(_host);
    url.setPort(_port);
    url.setPath(path);
    url.setQuery(parameters);
    url.setScheme(_connectionType == 1 ? "https" : "http");

    qDebug() << "Request: " << url;

    QNetworkRequest request(url);
    _manager.get(request);
}

QString SubsonicManager::getCommandPath(Command command)
{
    switch (command) {
    case PING:
        return "ping.view";
    case INDEXES:
        return "getIndexes";
    case MUSIC_DIRECTORY:
    case LIST_DIRECTORY:
        return "getMusicDirectory";
    case SONG:
    case FILE:
        return "getSong";
    }

    return {};
}

SubsonicManager::FileType SubsonicManager::fileTypeFromString(QString type)
{
    if (type == "folder") {
        return FileType::Folder;
    } else if (type == "track") {
        return FileType::Track;
    }

    return FileType::Unknown;
}

QString SubsonicManager::fileTypeName(FileType type)
{
    switch (type) {
    case FileType::Folder:
        return "folder";
    case FileType::Track:
        return "track";
    case FileType::Unknown:
        return "unknown";
    }
}

void SubsonicManager::onGotIndexes(QDomDocument result)
{
    QVector<File> filesList;

    QDomNodeList folders = result.elementsByTagName("artist");
    for (int i = 0; i < folders.count(); i++) {
        QDomElement el = folders.at(i).toElement();

        filesList.append({
            .name = el.attribute("name"),
            .path = el.attribute("id"),
            .type = FileType::Folder,
        });
    }

    _filesCache.clear();
    QDomNodeList files = result.elementsByTagName("child");
    for (int i = 0; i < files.count(); i++) {
        QDomElement el = files.at(i).toElement();
        File file = {
            .name = el.attribute("title"),
            .path = el.attribute("id"),
            .type = FileType::Track,
        };

        filesList.append(file);
        _filesCache.append(file);
    }

    emit gotFileList(filesList);
}

void SubsonicManager::onGotMusicDirectory(QDomDocument result)
{
    QVector<File> filesList;

    File parent;

    QDomElement dir = result.elementsByTagName("directory").at(0).toElement();
    if (dir.hasAttribute("parent")) {
        parent = {
            .name = "..",
            .path = dir.attribute("parent"),
            .type = FileType::Folder,
        };
    } else {
        parent = {
            .name = "..",
            .path = "/",
            .type = FileType::Folder,
        };
    }
    filesList.append(parent);

    _filesCache.clear();
    QDomNodeList files = result.elementsByTagName("child");
    for (int i = 0; i < files.count(); i++) {
        QDomElement el = files.at(i).toElement();
        File file = {
            .name = el.attribute("title"),
            .path = el.attribute("id"),
            .type = el.attribute("isDir") == "true" ? FileType::Folder : FileType::Track,
            };

        filesList.append(file);
        _filesCache.append(file);
    }

    filesList.append(parent);

    emit gotFileList(filesList);
}

void SubsonicManager::onListDirectory(QDomDocument result)
{
    QList<PlaylistItem> playlist;

    QDomNodeList files = result.elementsByTagName("child");
    for (int i = 0; i < files.count(); i++) {
        QDomElement el = files.at(i).toElement();

        if (el.attribute("isDir") == "true") {
            listDirectory(el.attribute("id"));
            continue;
        }

        playlist.append({
            .id = el.attribute("id"),
            .title = el.attribute("title"),
            .album = el.attribute("album"),
            .artist = el.attribute("artist"),
            .track = el.attribute("track"),
        });
    }

    emit gotTrackList(playlist);
}

void SubsonicManager::onGotSong(QDomDocument result)
{
    PlaylistItem song;
    QDomElement el = result.elementsByTagName("song").at(0).toElement();

    song = {
        .id = el.attribute("id"),
        .title = el.attribute("title"),
        .album = el.attribute("album"),
        .artist = el.attribute("artist"),
        .track = el.attribute("track"),
    };

    emit gotSong(song);
}

void SubsonicManager::onGotFile(QDomDocument result)
{
    QList<PlaylistItem> playlist;

    QDomElement el = result.elementsByTagName("song").at(0).toElement();

    playlist.append({
        .id = el.attribute("id"),
        .title = el.attribute("title"),
        .album = el.attribute("album"),
        .artist = el.attribute("artist"),
        .track = el.attribute("track"),
    });

    emit gotTrackList(playlist);
}
