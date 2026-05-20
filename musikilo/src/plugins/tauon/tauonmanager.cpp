#include "tauonmanager.h"

#include <QFileInfo>
#include <QJsonArray>
#include <QVector>

TauonManager::TauonManager(QObject *parent) : QObject(parent), _manager(this)
{
    connect(&_manager, &QNetworkAccessManager::finished, this, &TauonManager::onRequestFinished);
}

void TauonManager::setConnectionSettings(QString host, int port)
{
    _host = host;
    _port = port;
}

void TauonManager::getTrackList()
{
    if (_currentPlaylistId == 0) {
        return;
    }

    makeRequest(TRACKLIST, {QString::number(_currentPlaylistId)});
}

QVector<TauonManager::File> TauonManager::getFilesList(QString path)
{
    if (_trackTree.empty() || !_trackTree.contains(path)) {
        return {};
    }

    if (path == "/") {
        while (_trackTree.contains(path)) {
            if (_trackTree[path].size() > 1) {
                break;
            }

            path = _trackTree[path].first().path;
        }
    }


    qDebug() << path;
    for (TauonManager::File file : _trackTree[path]) {
        qDebug() << file.path << " " << file.name;
    }

    return _trackTree[path];
}

void TauonManager::getPlaylists()
{
    makeRequest(PLAYLISTS);
}

void TauonManager::getStatus()
{
    makeRequest(STATUS);
}

void TauonManager::play(int index)
{
    makeRequest(PLAY_INDEX, {QString::number(_currentPlaylistId), QString::number(index)});
}

void TauonManager::playPath(QString path)
{
    QFileInfo pathInfo(path);

    for (TauonManager::File file : _trackTree[pathInfo.path()]) {
        if (file.name == pathInfo.fileName()) {
            makeRequest(PLAY_INDEX, {QString::number(_currentPlaylistId), QString::number(file.id)});
        }
    }
}

void TauonManager::nextSong()
{
    makeRequest(NEXT);
}

void TauonManager::prevSong()
{
    makeRequest(PREV);
}

void TauonManager::play()
{
    makeRequest(PLAY);
}

void TauonManager::pause()
{
    makeRequest(PAUSE);
}

void TauonManager::seek(qint64 position)
{
    makeRequest(SEEK, {QString::number(position)});
}

void TauonManager::setShuffle(bool shuffle)
{
    makeRequest(SHUFFLE);
}

void TauonManager::setRepeat(bool repeat)
{
    makeRequest(REPEAT);
}

void TauonManager::setVolume(int volume)
{
    makeRequest(VOLUME, {QString::number(volume)});
}

void TauonManager::getVersion()
{
    makeRequest(VERSION);
}

void TauonManager::setActivePlaylist(int id)
{
    _currentPlaylistId = id;

    getTrackList();
}

void TauonManager::onRequestFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Reply error: " << reply->errorString();
        emit errorOccured(reply->errorString());
        return;
    }

    QJsonParseError error;
    QString json = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(json.toLatin1(), &error);
    if (error.error != QJsonParseError::NoError && json != "OK") {
        emit errorOccured(error.errorString());
    }

    QJsonObject root = doc.object();
    QString path = reply->url().path();

    if (path.startsWith("/api1/" + getCommandPath(VERSION))) {
        emit gotVersion();
    } else if (path.startsWith("/api1/" + getCommandPath(STATUS))) {
        emit onGotStatus(root);
    } else if (path.startsWith("/api1/" + getCommandPath(PLAYLISTS))) {
        emit onGotPlaylists(root);
    } else if (path.startsWith("/api1/" + getCommandPath(TRACKLIST))) {
        emit onGotTracklist(root);
    }
}

void TauonManager::update()
{
    getStatus();
}

void TauonManager::makeRequest(Command command, QStringList parameters)
{
    QString path = "/api1/" + getCommandPath(command);
    QString arg;
    foreach(arg, parameters) {
        path += "/" + arg;
    }

    QUrl url;
    url.setHost(_host);
    url.setPort(_port);
    url.setPath(path);
    url.setScheme("http");

    qDebug() << "Request: " << url;

    QNetworkRequest request(url);
    _manager.get(request);
}

QString TauonManager::getCommandPath(Command command)
{
    switch (command) {
    case TRACKLIST:
        return "tracklist";
    case PLAYLISTS:
        return "playlists";
    case STATUS:
        return "status";
    case PLAY_INDEX:
        return "start";
    case NEXT:
        return "next";
    case PREV:
        return "back";
    case PLAY:
        return "play";
    case PAUSE:
        return "pause";
    case SEEK:
        return "seek";
    case SHUFFLE:
        return "shuffle";
    case REPEAT:
        return "repeat";
    case VOLUME:
        return "setvolume";
    case VERSION:
        return "version";
    }

    return {};
}

TauonManager::FileType TauonManager::fileTypeFromString(QString type)
{
    if (type == "folder") {
        return FileType::Folder;
    } else if (type == "track") {
        return FileType::Track;
    }

    return FileType::Unknown;
}

QString TauonManager::fileTypeName(FileType type)
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

void TauonManager::onGotTracklist(QJsonObject result)
{
    _currentTracklist.clear();
    _trackTree.clear();

    QJsonValue entry;
    foreach(entry, result["tracks"].toArray()) {
        QJsonObject obj = entry.toObject();

        _currentTracklist.append({
            obj["position"].toInt(),
            obj["id"].toInt(),
            obj["title"].toString(),
            obj["artist"].toString(),
            obj["album"].toString(),
            obj["path"].toString(),
            obj["track_number"].toString(),
        });
    }

    Song song;
    foreach(song, _currentTracklist) {
        QUrl filepath(song.path);
        QString path = filepath.path();
        int pos = 0;

//        qDebug() << "File: " << filepath;

        while (pos != -1) {
            int nextPos = path.indexOf('/', pos + 1);
            QString leftPath = path.left(pos);
            QString name = path.mid(pos + 1, nextPos - pos - 1);
            bool directory = nextPos > 0;

//            qDebug() << "Part pos: " << pos << " nextpos: " << nextPos << " leftpath: " << leftPath << " name: " << name << " is directory: " << directory;

            File file = {
                .name = directory ? name : filepath.fileName(),
                .path = leftPath + "/" + (directory ? name : filepath.fileName()),
                .id = directory ? 0 : song.index,
                .type = directory ? FileType::Folder : FileType::Track,
            };

            leftPath = leftPath == "" ? "/" : leftPath;

            if (_trackTree.contains(leftPath)) {
                if (!directory || !_trackTree.contains(file.path)) {
                    _trackTree[leftPath].append(file);
                }
            } else {
                _trackTree[leftPath] = {file};
            }

            pos = nextPos;
        }
    }

    emit gotTracklist(_currentTracklist);
}

void TauonManager::onGotStatus(QJsonObject result)
{
    Status status = {
        .status = result["status"].toString(),
        .shuffle = result["shuffle"].toBool(),
        .repeat = result["repeat"].toBool(),
        .progress = result["progress"].toInt(),
        .autoStop = result["auto_stop"].toBool(),
        .volume = result["volume"].toInt(),
        .playlist = result["playlist"].toString().toInt(),
        .currentTrackId = result["id"].toInt(),
        .title = result["title"].toString(),
        .artist = result["artist"].toString(),
        .album = result["album"].toString(),
        .position = result["position"].toInt(),
        .duration = 0,
    };

    if (status.position < 0) {
        status.position = 0;
    }

    if (result.contains("track")) {
        status.duration = result["track"].toObject()["duration"].toInt();
    }

    emit gotStatus(status);
}

void TauonManager::onGotPlaylists(QJsonObject result)
{
    QVector<Playlist> playlists;

    for(QJsonValue val : result["playlists"].toArray()) {
        QJsonObject obj = val.toObject();

        playlists.append({
            .name = obj["name"].toString(),
            .id = obj["id"].toString().toInt(),
        });
    }

    emit gotPlaylists(playlists);
}
