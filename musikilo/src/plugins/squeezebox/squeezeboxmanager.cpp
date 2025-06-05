#include "squeezeboxmanager.h"

#include <QJsonArray>
#include <QVector>

SqueezeBoxManager::SqueezeBoxManager(QObject *parent) : QObject(parent), _manager(this)
{
    connect(&_manager, &QNetworkAccessManager::finished, this, &SqueezeBoxManager::onRequestFinished);
}

void SqueezeBoxManager::setConnectionSettings(QString host, int port)
{
    _host = host;
    _port = port;
}

void SqueezeBoxManager::getFilesList(QString path)
{
    if (path == "/") {
        makeRequest(MUSICFOLDER);
    } else {
        makeRequest(MUSICFOLDER, "0", {QString("url:") + path});
    }
}

void SqueezeBoxManager::getMusicFolder()
{
    makeRequest(MUSICFOLDER, "0", {"return_top:1"});
}

void SqueezeBoxManager::getStatus()
{
    makeRequest(STATUS, _currentPlayerId);
}

void SqueezeBoxManager::getPlayers(int count)
{
    if (count > 0) {
        makeRequest(PLAYERS, "0", {"0", QString::number(count)});
    } else {
        makeRequest(PLAYERS);
    }
}

void SqueezeBoxManager::setActivePlayer(QString id)
{
    _currentPlayerId = id;
}

void SqueezeBoxManager::getPlaylist(int quantity)
{
    makeRequest(PLAYLIST, _currentPlayerId, {"0", QString::number(quantity), "tags:cdegilqtuyAAGIKNPSxxs"});
}

void SqueezeBoxManager::addFile(QString url)
{
    makeRequest(ADD_FILE, _currentPlayerId, {url});
}

void SqueezeBoxManager::playFile(QString url)
{
    makeRequest(PLAY_FILE, _currentPlayerId, {url});
}

void SqueezeBoxManager::play(int index)
{
    makeRequest(PLAY_INDEX, _currentPlayerId, {QString::number(index)});
}

void SqueezeBoxManager::nextSong()
{
    makeRequest(NEXT, _currentPlayerId, {"+1"});
}

void SqueezeBoxManager::prevSong()
{
    makeRequest(NEXT, _currentPlayerId, {"-1"});
}

void SqueezeBoxManager::clear()
{
    makeRequest(CLEAR, _currentPlayerId);
}

void SqueezeBoxManager::play()
{
    makeRequest(PLAY, _currentPlayerId);
}

void SqueezeBoxManager::pause()
{
    makeRequest(PAUSE, _currentPlayerId);
}

void SqueezeBoxManager::stop()
{
    makeRequest(STOP, _currentPlayerId);
}

void SqueezeBoxManager::seek(qint64 position)
{
    makeRequest(SEEK, _currentPlayerId, {QString::number(position)});
}

void SqueezeBoxManager::setShuffle(bool shuffle)
{
    makeRequest(SHUFFLE, _currentPlayerId, {QString::number(shuffle)});
}

void SqueezeBoxManager::setRepeat(bool repeat)
{
    makeRequest(REPEAT, _currentPlayerId, {repeat ? "2" : "0"});
}

void SqueezeBoxManager::setSingle(bool single)
{
    makeRequest(REPEAT, _currentPlayerId, {single ? "1" : "0"});
}

void SqueezeBoxManager::refreshDatabase()
{
    makeRequest(RESCAN);
}

void SqueezeBoxManager::setVolume(int volume)
{
    makeRequest(VOLUME, _currentPlayerId, {QString::number(volume)});
}

void SqueezeBoxManager::getSyncPlayers()
{
    makeRequest(SYNC, _currentPlayerId, {"?"});
}

void SqueezeBoxManager::syncPlayer(QString id)
{
    makeRequest(SYNC, _currentPlayerId, {id});
}

void SqueezeBoxManager::desyncPlayer(QString id)
{
    makeRequest(SYNC, id, {"-"});
}

void SqueezeBoxManager::remove(int index)
{
    makeRequest(DELETE, _currentPlayerId, {QString::number(index)});
}

void SqueezeBoxManager::getVersion()
{
    makeRequest(VERSION);
}

void SqueezeBoxManager::onRequestFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Reply error: " << reply->errorString();
        emit errorOccured(reply->errorString());
        return;
    }

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll(), &error);
    if (error.error != QJsonParseError::NoError) {
        emit errorOccured(error.errorString());
    }

    QJsonObject root = doc.object();

    int requestId = root["id"].toInt();
    Request request = _requests.take(requestId);

    switch (request.command) {
    case PLAYER_ID:
        return onGotPlayerId(root["result"].toObject(), request.params);
    case MUSICFOLDER:
        return onGotMusicFolder(root["result"].toObject(), request.params);
    case STATUS:
        return onGotStatus(root["result"].toObject());
    case PLAYERS:
        return onGotPlayers(root["result"].toObject());
    case PLAYLIST:
        return onGotPlaylist(root["result"].toObject());
    case ADD_FILE:
    case PLAY_FILE:
    case PLAY_INDEX:
    case NEXT:
    case PREV:
    case CLEAR:
    case PLAY:
    case PAUSE:
    case STOP:
    case SEEK:
    case SHUFFLE:
    case REPEAT:
    case RESCAN:
    case VOLUME:
    case DELETE:
        break;
    case SYNC:
        return onGotSyncPlayers(root["result"].toObject(), request.params);
    case VERSION:
        emit gotVersion();
        break;
    }
}

void SqueezeBoxManager::update()
{
    getPlayers();

    if (!_currentPlayerId.isEmpty()) {
        getStatus();
    }
}

void SqueezeBoxManager::makeRequest(Command command, QString playerId, QStringList parameters)
{
    QJsonDocument doc;
    QJsonObject root;
    root["id"] = _requestId;
    root["method"] = "slim.request";

    QJsonArray params;
    params << playerId;

    QJsonArray options;
    QString arg;
    foreach(arg, getCommand(command)) {
        if (arg == "null") {
            options << QJsonValue();
        } else {
            options << arg;
        }
    }

    foreach(arg, parameters) {
        options << arg;
    }

    params.append(options);
    root["params"] = params;
    doc.setObject(root);

    QUrl url;
    url.setHost(_host);
    url.setPort(_port);
    url.setPath("/jsonrpc.js");
    url.setScheme("http");

//    qDebug() << "Request: " << doc.toJson(QJsonDocument::Compact);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/json");

    _requests[_requestId] = {command, parameters};
    _requestId++;
    _manager.post(request, doc.toJson(QJsonDocument::Compact));
}

QStringList SqueezeBoxManager::getCommand(Command command)
{
    switch (command) {
    case MUSICFOLDER:
        return {"musicfolder", "0", "null", "tags:u"};
    case PLAYER_ID:
        return {"player", "id"};
    case STATUS:
        return {"status", "-", "1", "menu:1"};
    case PLAYERS:
        return {"players"};
    case PLAYLIST:
        return {"status"};
    case ADD_FILE:
        return {"playlist", "add"};
    case PLAY_FILE:
        return {"playlist", "play"};
    case PLAY_INDEX:
    case NEXT:
    case PREV:
        return {"playlist", "index"};
    case CLEAR:
        return {"playlist", "clear"};
    case PLAY:
        return {"play"};
    case PAUSE:
        return {"pause"};
    case STOP:
        return {"stop"};
    case SEEK:
        return {"time"};
    case SHUFFLE:
        return {"playlist", "shuffle"};
    case REPEAT:
        return {"playlist", "repeat"};
    case RESCAN:
        return {"rescan"};
    case VOLUME:
        return {"mixer", "volume"};
    case SYNC:
        return {"sync"};
    case DELETE:
        return {"playlist", "delete"};
    case VERSION:
        return {"version", "?"};
    }

    return {};
}

SqueezeBoxManager::FileType SqueezeBoxManager::fileTypeFromString(QString type)
{
    if (type == "folder") {
        return FileType::Folder;
    } else if (type == "track") {
        return FileType::Track;
    }

    return FileType::Unknown;
}

QString SqueezeBoxManager::fileTypeName(FileType type)
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

void SqueezeBoxManager::onGotPlayerId(QJsonObject result, QStringList params)
{
    qDebug() << "Got id " << result["_id"].toString() << " for player " << params;
}

void SqueezeBoxManager::onGotMusicFolder(QJsonObject result, QStringList params)
{
    QVector<File> musicFolder;

    QJsonValue entry;
    foreach(entry, result["folder_loop"].toArray()) {
        QJsonObject obj = entry.toObject();

        musicFolder.append({
            obj["filename"].toString(),
            obj["id"].toInt(),
            fileTypeFromString(obj["type"].toString()),
            obj["url"].toString(),
        });
    }
    QString param;
    foreach(param, params) {
        if (param.startsWith("return_top:")) {
            emit gotTopMusicFolder(musicFolder.first());
            return;
        }
    }

    emit gotMusicFolder(musicFolder);
}

void SqueezeBoxManager::onGotStatus(QJsonObject result)
{
    double time;
    if (result["time"].isDouble()) {
        time = result["time"].toDouble();
    } else {
        time = result["time"].toString().toDouble();
    }

    Status status = {
        .mode = result["mode"].toString(),
        .position = time,
        .duration = result["duration"].toDouble(),
        .volume = result["mixer volume"].toInt(),
        .repeat = result["playlist repeat"].toInt() == 2,
        .single = result["playlist repeat"].toInt() == 1,
        .shuffle = result["playlist shuffle"].toInt() > 0,
        .currentIndex = result["playlist_cur_index"].isString() ? result["playlist_cur_index"].toString().toInt() : result["playlist_cur_index"].toInt(),
        .tracks = result["playlist_tracks"].toInt(),
        .song = {}
    };

    if (status.tracks > 0) {
        QJsonObject song = result["item_loop"].toArray().first().toObject();

        Song currentSong = {
            .index = song["params"].toObject()["playlist_index"].toInt(),
            .id = song["params"].toObject()["track_id"].toInt(),
            .title = song["track"].toString(),
            .album = song["album"].toString(),
            .artist = song["artist"].toString(),
            .url = "",
            .trackNr = song["tracknum"].toString().toInt(),
            };

        status.song = currentSong;
    }

    emit gotStatus(status);

    getPlaylist(status.tracks);
}

void SqueezeBoxManager::onGotPlayers(QJsonObject result)
{
    if (!result.contains("players_loop")) {
        getPlayers(result["count"].toInt());
        return;
    }

    QVector<Player> players;

    QJsonValue i;
    foreach(i, result["players_loop"].toArray()) {
        QJsonObject player = i.toObject();

        players.append({
            player["name"].toString(),
            player["playerid"].toString(),
        });
    }


    if (!players.isEmpty() && _currentPlayerId.isEmpty()) {
        setActivePlayer(players.first().id);
    }

    emit gotPlayers(players);
}

void SqueezeBoxManager::onGotPlaylist(QJsonObject result)
{
    QVector<Song> playlist;

    for(QJsonValue val : result["playlist_loop"].toArray()) {
        QJsonObject songJson = val.toObject();

        playlist.append({
            .index = songJson["playlist index"].toInt(),
            .id = songJson["id"].toInt(),
            .title = songJson["title"].toString(),
            .album = songJson["album"].toString(),
            .artist = songJson.contains("artist") ? songJson["artist"].toString() : songJson["album"].toString(),
            .url = songJson["url"].toString(),
            .trackNr = songJson["tracknum"].toString().toInt(),
        });
    }

    emit gotPlaylist(playlist);
}

void SqueezeBoxManager::onGotSyncPlayers(QJsonObject result, QStringList params)
{
    if (params.first() != "?") return;

    if (result["_sync"].toString() == "-") {
        emit gotSyncPlayers({});
    }

    QStringList players = result["_sync"].toString().split(',');
    emit gotSyncPlayers(players);
}
