#include "squeezeboxplayer.h"

SqueezeBoxPlayer::SqueezeBoxPlayer(SqueezeBoxManager *manager, QObject *parent) : PlayerInterface(parent), _manager(manager)
{
    connect(manager, &SqueezeBoxManager::errorOccured, this, &PlayerInterface::errorOccured);
    connect(manager, &SqueezeBoxManager::gotStatus, this, &SqueezeBoxPlayer::onGotStatus);
    connect(manager, &SqueezeBoxManager::gotPlayers, this, &SqueezeBoxPlayer::onGotPlayers);
    connect(manager, &SqueezeBoxManager::gotSyncPlayers, this, &SqueezeBoxPlayer::onGotSyncPlayers);
}

QMediaPlayer::State SqueezeBoxPlayer::getState()
{
    if (_status.mode == "play") {
        return QMediaPlayer::PlayingState;
    } else if (_status.mode == "pause") {
        return QMediaPlayer::PausedState;
    } else {
        return QMediaPlayer::StoppedState;
    }
}

void SqueezeBoxPlayer::play()
{
    _manager->play();
}

void SqueezeBoxPlayer::pause()
{
    _manager->pause();
}

void SqueezeBoxPlayer::stop()
{
    _manager->stop();
}

qint64 SqueezeBoxPlayer::getDuration() const
{
    return _status.duration * 1000.0l;
}

qint64 SqueezeBoxPlayer::getPosition() const
{
    return _status.position * 1000.0l;
}

void SqueezeBoxPlayer::setPosition(qint64 position)
{
    _manager->seek(position/1000.0l);
}

QString SqueezeBoxPlayer::getTitle()
{
    if (_status.tracks == 0) return QString();

    return _status.song.title;
}

QString SqueezeBoxPlayer::getArtist()
{
    if (_status.tracks == 0) return QString();

    return _status.song.artist;
}

QString SqueezeBoxPlayer::getAlbum()
{
    if (_status.tracks == 0) return QString();

    return _status.song.album;
}

QString SqueezeBoxPlayer::getBitrate()
{
    return QString();
}

bool SqueezeBoxPlayer::getRepeat() const
{
    return _status.repeat;
}

void SqueezeBoxPlayer::setRepeat(bool repeat)
{
    _manager->setRepeat(repeat);
}

bool SqueezeBoxPlayer::getSingle() const
{
    return _status.single;
}

void SqueezeBoxPlayer::setSingle(bool single)
{
    _manager->setSingle(single);
}

bool SqueezeBoxPlayer::getShuffle() const
{
    return _status.shuffle;
}

void SqueezeBoxPlayer::setShuffle(bool shuffle)
{
    _manager->setShuffle(shuffle);
}

bool SqueezeBoxPlayer::getConsume() const
{
    return false;
}

void SqueezeBoxPlayer::setConsume(bool consume)
{
    Q_UNUSED(consume)
}

QVariantMap SqueezeBoxPlayer::getControls()
{
    QVariantMap controls;

    QStringList playerNames;
    for (SqueezeBoxManager::Player player : _players) {
        playerNames << player.name;
    }
    controls["player_names"] = playerNames;

    QStringList playerIds;
    for (SqueezeBoxManager::Player player : _players) {
        playerIds << player.id;
    }
    controls["player_ids"] = playerIds;

    controls["active_player"] = _activePlayer;

    controls["volume"] = _status.volume;

    QVariantList syncPlayers;
    for (QString player : _syncPlayers) {
        syncPlayers << _playerIds[player];
    }

//    qDebug() << "Sync players: " << syncPlayers;

    controls["sync_players"] = syncPlayers;

    return controls;
}

void SqueezeBoxPlayer::setControls(QVariantMap controls)
{
    if (_activePlayer != controls["active_player"]) {
        _activePlayer = controls["active_player"].toInt();
        _manager->setActivePlayer(_players[_activePlayer].id);
    }

    if (controls.contains("refresh")) {
        _manager->refreshDatabase();
        controls.remove("refresh");
    }

    if (controls.contains("fetch_sync")) {
        _manager->getSyncPlayers();
        controls.remove("fetch_sync");
    }

    if (controls["volume"].toInt() != _status.volume) {
        _manager->setVolume(controls["volume"].toInt());
    }

    QVariantList syncPlayers;
    for (QString player : _syncPlayers) {
        syncPlayers << _playerIds[player];
    }

    if (syncPlayers != controls["sync_players"].toList()) {
        syncPlayers = controls["sync_players"].toList();

        for (int i = 0; i < _players.size(); i++) {
            if (i == _activePlayer) continue;

            if (syncPlayers.contains(i) && !_syncPlayers.contains(_players[i].id)) {
                _manager->syncPlayer(_players[i].id);
            }

            if (!syncPlayers.contains(i) && _syncPlayers.contains(_players[i].id)) {
                _manager->desyncPlayer(_players[i].id);
            }
        }
    }
}

void SqueezeBoxPlayer::onGotStatus(Status status)
{
    Status oldStatus = _status;

    _status = status;

    if (oldStatus.position != status.position) {
        emit positionChanged();
    }

    if (oldStatus.duration != status.duration) {
        emit durationChanged();
    }

    if (oldStatus.mode != status.mode) {
        emit stateChanged();
    }

    if (oldStatus.repeat != status.repeat) {
        emit repeatChanged();
    }

    if (oldStatus.single != status.single) {
        emit singleChanged();
    }

    if (oldStatus.shuffle != status.shuffle) {
        emit shuffleChanged();
    }

    if (oldStatus.song.id != status.song.id) {
        emit metadataChanged();
    }

    emit controlsChanged();
}

void SqueezeBoxPlayer::onGotPlayers(QVector<SqueezeBoxManager::Player> players)
{
    _players = players;

    _playerIds.clear();
    for (int i = 0; i < _players.size(); i++) {
        _playerIds[_players[i].id] = i;
    }

    if (_activePlayer == -1 && !_players.isEmpty()) {
        _activePlayer = 0;
        _manager->setActivePlayer(_players[_activePlayer].id);
    }

    if (_players.isEmpty()) {
        _activePlayer = -1;
        _manager->setActivePlayer("");
    }
}

void SqueezeBoxPlayer::onGotSyncPlayers(QStringList players)
{
    _syncPlayers = players;

    emit controlsChanged();
}
