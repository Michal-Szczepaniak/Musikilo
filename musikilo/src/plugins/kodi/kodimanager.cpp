#include "kodimanager.h"

#include <QFileInfo>
#include <QJsonArray>
#include <QTime>
#include <QVector>

KodiManager::KodiManager(QObject *parent) : QObject(parent), _manager(this)
{
    connect(&_manager, &QNetworkAccessManager::finished, this, &KodiManager::onRequestFinished);
}

void KodiManager::setConnectionSettings(QString host, int port)
{
    _host = host;
    _port = port;
}

void KodiManager::getFilesList(QString path)
{
    if (path == "/") {
        return getSources();
    }

    makeRequest(FILELIST, {{"directory", path}, {"media", "files"}, {"sort", QJsonObject({{"method", "label"}})}});
}

void KodiManager::getPlaylist()
{
    makeRequest(PLAYLIST);
}

void KodiManager::getSources()
{
    makeRequest(SOURCES, {{"media", "video"}});
}

void KodiManager::getStatus()
{
    makeRequest(GET_VOLUME, { {"properties", QJsonArray({"volume"})} });
    makeRequest(PLAYER_ITEM, { {"properties", QJsonArray({"title","tagline"})} });
    makeRequest(PLAYER_PROPETIRES, { {"properties", QJsonArray({
        "repeat", "shuffled", "time", "totaltime", "speed", "position",
        "audiostreams", "videostreams", "subtitles", "currentaudiostream", "currentvideostream", "currentsubtitle"
    })} });
    getPlaylist();
}

void KodiManager::play(int index)
{
    makeRequest(PLAYER_OPEN, {{"item", QJsonObject({{"playlistid", 1}, {"position", index}})}});
}

void KodiManager::addFile(QString url)
{
    makeRequest(ADD, {{"item", QJsonArray({QJsonObject({{"file", url}})})}});
}

void KodiManager::playFile(QString url)
{
    makeRequest(PLAY_FILE, {{"item", QJsonArray({QJsonObject({{"file", url}})})}});
}

void KodiManager::nextSong()
{
    makeRequest(GOTO, { {"to", "next"}});
}

void KodiManager::prevSong()
{
    makeRequest(GOTO, { {"to", "previous"}});
}

void KodiManager::remove(int index)
{
    if (index == -1) return;

    makeRequest(REMOVE, { {"playlistid", 1}, {"position", index}});
}

void KodiManager::clearPlaylist()
{
    makeRequest(CLEAR);
}

void KodiManager::play()
{
    makeRequest(PLAY, { {"play", true} });
}

void KodiManager::pause()
{
    makeRequest(PLAY, { {"play", false} });
}

void KodiManager::stop()
{
    makeRequest(STOP);
}

void KodiManager::seek(qint64 position)
{
    QTime time = QTime::fromMSecsSinceStartOfDay(position);

    makeRequest(SEEK, { {"value", QJsonObject({ {"time", QJsonObject({ {"hours", time.hour()}, {"minutes", time.minute()}, {"seconds", time.second()}, {"milliseconds", time.msec()} })} })} });
}

void KodiManager::setShuffle(bool shuffle)
{
    makeRequest(SHUFFLE, { {"shuffle", shuffle} });
}

void KodiManager::setRepeat(bool repeat)
{
    makeRequest(REPEAT, { {"repeat", repeat ? "all" : "off"} });
}

void KodiManager::setSingle(bool single)
{
    makeRequest(REPEAT, { {"repeat", single ? "one" : "off"} });
}

void KodiManager::setVolume(int volume)
{
    makeRequest(SET_VOLUME, {{"volume", volume}});
}

void KodiManager::setAudioStream(int index)
{
    makeRequest(AUDIO_STREAM, {{"stream", index}});
}

void KodiManager::setVideoStream(int index)
{
    makeRequest(VIDEO_STREAM, {{"stream", index}});
}

void KodiManager::setSubtitle(int index)
{
    makeRequest(SUBTITLE, {{"subtitle", index}});
}

void KodiManager::ping()
{
    makeRequest(PING);
}

void KodiManager::onRequestFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Reply error: " << reply->errorString();
        emit errorOccured(reply->errorString());
        return;
    }

    QJsonParseError error;
    QString json = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(json.toLatin1(), &error);
    if (error.error != QJsonParseError::NoError) {
        emit errorOccured(error.errorString());
        qDebug() << "Error: " << doc.toJson(QJsonDocument::Compact);
        return;
    }

    QJsonObject root = doc.object();

    int requestId = root["id"].toInt();
    Request request = _requests.take(requestId);

    if (root.contains("error") && request.command != PLAY && request.command != PAUSE && request.command != STOP && request.command != REMOVE) {
        qDebug() << "Error: " << doc.toJson(QJsonDocument::Compact);
        emit errorOccured(root["error"].toObject()["message"].toString());
        return;
    }

    switch (request.command) {
    case PLAYER_PROPETIRES:
        onGotPlayerProperties(root);
        break;
    case PLAYER_ITEM:
        onGotPlayerItem(root);
        break;
    case GET_VOLUME:
        onGotVolume(root);
        break;
    case PLAYLIST:
        onGotPlaylist(root);
        break;
    case SOURCES:
        onGotSources(root);
        break;
    case FILELIST:
        onGotFileList(root);
        break;
    case PLAY_FILE:
        play(0);
        break;
    case PING:
        emit gotPing();
        break;
    default:
        break;
    }
}

void KodiManager::update()
{
    getStatus();
}

void KodiManager::makeRequest(Command command, QJsonObject parameters)
{
    QJsonDocument doc;
    QJsonObject root;
    root["id"] = _requestId;
    root["method"] = getCommand(command);
    root["jsonrpc"] = "2.0";

    if (getCommand(command).startsWith("Player.") && getCommand(command) != "Player.Open") {
        parameters["playerid"] = 1;
    }

    if (getCommand(command).startsWith("Playlist.")) {
        parameters["playlistid"] = 1;
    }

    root["params"] = parameters;
    doc.setObject(root);

    QUrl url;
    url.setHost(_host);
    url.setPort(_port);
    url.setPath("/jsonrpc");
    url.setScheme("http");

    qDebug() << "Request: " << doc.toJson(QJsonDocument::Compact);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/json");

    _requests[_requestId] = {command, parameters};
    _requestId++;
    _manager.post(request, doc.toJson(QJsonDocument::Compact));
}

QString KodiManager::getCommand(Command command)
{
    switch (command) {
    case PLAYER_PROPETIRES:
        return "Player.GetProperties";
    case PLAYER_ITEM:
        return "Player.GetItem";
    case PLAY:
        return "Player.PlayPause";
    case PAUSE:
        return "Player.PlayPause";
    case STOP:
        return "Player.Stop";
    case GET_VOLUME:
        return "Application.GetProperties";
    case SET_VOLUME:
        return "Application.SetVolume";
    case SHUFFLE:
        return "Player.SetShuffle";
    case REPEAT:
        return "Player.SetRepeat";
    case SEEK:
        return "Player.Seek";
    case GOTO:
        return "Player.GoTo";
    case PLAYER_OPEN:
        return "Player.Open";
    case PLAYLIST:
        return "Playlist.GetItems";
    case REMOVE:
        return "Playlist.Remove";
    case FILELIST:
        return "Files.GetDirectory";
    case SOURCES:
        return "Files.GetSources";
    case ADD:
        return "Playlist.Add";
    case PLAY_FILE:
        return "Playlist.Add";
    case CLEAR:
        return "Playlist.Clear";
    case PING:
        return "JSONRPC.Ping";
    case AUDIO_STREAM:
        return "Player.SetAudioStream";
    case VIDEO_STREAM:
        return "Player.SetVideoStream";
    case SUBTITLE:
        return "Player.SetSubtitle";
    }

    return {};
}

KodiManager::FileType KodiManager::fileTypeFromString(QString type)
{
    if (type == "folder") {
        return FileType::Folder;
    } else if (type == "track") {
        return FileType::Track;
    }

    return FileType::Unknown;
}

QString KodiManager::fileTypeName(FileType type)
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

void KodiManager::onGotVolume(QJsonObject result)
{
    _status.volume = result["result"].toObject()["volume"].toInt();
}

void KodiManager::onGotPlayerProperties(QJsonObject result)
{
    QJsonObject resultObj = result["result"].toObject();

    _status.repeat = resultObj["repeat"].toString() == "all";
    _status.single = resultObj["repeat"].toString() == "one";
    _status.shuffle = resultObj["shuffled"].toBool();
    _status.playing = resultObj["speed"].toInt() != 0;
    _status.position = resultObj["position"].toInt();
    _status.currentAudioStream = resultObj["currentaudiostream"].toObject()["index"].toInt();
    _status.currentVideoStream = resultObj["currentvideostream"].toObject()["index"].toInt();
    _status.currentSubtitle = resultObj["currentsubtitle"].toObject()["index"].toInt();

    _status.audioStreams.clear();
    for (QJsonValue v: resultObj["audiostreams"].toArray()) {
        QJsonObject audioStream = v.toObject();

        QString name = audioStream["language"].toString();

        _status.audioStreams.append(name);
    }

    _status.videoStreams.clear();
    for (QJsonValue v: resultObj["videostreams"].toArray()) {
        QJsonObject videoStream = v.toObject();

        QString name = videoStream["language"].toString();

        _status.videoStreams.append(name);
    }

    _status.subtitles.clear();
    for (QJsonValue v: resultObj["subtitles"].toArray()) {
        QJsonObject subtitle = v.toObject();

        QString name = subtitle["name"].toString();
        if (name.isEmpty()) {
            name = subtitle["language"].toString();
        }

        _status.subtitles.append(name);
    }

    QJsonObject timeObj = resultObj["time"].toObject();
    QTime time(timeObj["hours"].toInt(), timeObj["minutes"].toInt(), timeObj["seconds"].toInt(), timeObj["milliseconds"].toInt());
    _status.progress = time.msecsSinceStartOfDay();

    QJsonObject totaltimeObj = resultObj["totaltime"].toObject();
    QTime totaltime(totaltimeObj["hours"].toInt(), totaltimeObj["minutes"].toInt(), totaltimeObj["seconds"].toInt(), totaltimeObj["milliseconds"].toInt());
    _status.duration = totaltime.msecsSinceStartOfDay();

    emit gotStatus(_status);
}

void KodiManager::onGotPlayerItem(QJsonObject result)
{
    _status.title = result["result"].toObject()["item"].toObject()["label"].toString();
    _status.tagline = result["result"].toObject()["item"].toObject()["tagline"].toString();
}

void KodiManager::onGotPlaylist(QJsonObject result)
{
    QVector<PlaylistItem> playlist;

    QJsonArray items = result["result"].toObject()["items"].toArray();
    for(QJsonArray::Iterator it = items.begin(); it != items.end(); it++) {
        QJsonObject itemJson = it->toObject();

        playlist.append({
            .index = it.i,
            .id = itemJson["id"].toInt(),
            .title = itemJson["label"].toString()
        });
    }

    emit gotPlaylist(playlist);
}

void KodiManager::onGotSources(QJsonObject result)
{
    QVector<File> files;

    for (QJsonValue val : result["result"].toObject()["sources"].toArray()) {
        QJsonObject obj = val.toObject();

        files.append({
            .name = obj["label"].toString(),
            .path = obj["file"].toString(),
            .type = FileType::Folder,
        });
    }

    emit gotSources(files);
}

void KodiManager::onGotFileList(QJsonObject result)
{
    QVector<File> files;

    for (QJsonValue val : result["result"].toObject()["files"].toArray()) {
        QJsonObject obj = val.toObject();

        files.append({
            .name = obj["label"].toString(),
            .path = obj["file"].toString(),
            .type = obj["filetype"].toString() == "directory" ? FileType::Folder : FileType::Track,
        });
    }

    emit gotFileList(files);
}
