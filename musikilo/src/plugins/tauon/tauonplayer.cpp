#include "tauonplayer.h"

TauonPlayer::TauonPlayer(TauonManager *manager, QObject *parent) : PlayerInterface(parent), _manager(manager)
{
    connect(manager, &TauonManager::errorOccured, this, &PlayerInterface::errorOccured);
    connect(manager, &TauonManager::gotStatus, this, &TauonPlayer::onGotStatus);
    connect(manager, &TauonManager::gotPlaylists, this, &TauonPlayer::onGotPlaylists);
}

QMediaPlayer::State TauonPlayer::getState()
{
    if (_status.status == "paused") {
        return QMediaPlayer::PausedState;
    } else if (_status.status == "playing") {
        return QMediaPlayer::PlayingState;
    } else {
        return QMediaPlayer::StoppedState;
    }
}

void TauonPlayer::play()
{
    _manager->play();
}

void TauonPlayer::pause()
{
    _manager->pause();
}

void TauonPlayer::stop()
{
    _manager->pause();
}

qint64 TauonPlayer::getDuration() const
{
    return _status.duration;
}

qint64 TauonPlayer::getPosition() const
{
    return _status.progress;
}

void TauonPlayer::setPosition(qint64 position)
{
    _manager->seek(position);
}

QString TauonPlayer::getTitle()
{
    return _status.title;
}

QString TauonPlayer::getArtist()
{
    return _status.artist;
}

QString TauonPlayer::getAlbum()
{
    return _status.album;
}

QString TauonPlayer::getBitrate()
{
    return QString();
}

bool TauonPlayer::getRepeat() const
{
    return _status.repeat;
}

void TauonPlayer::setRepeat(bool repeat)
{
    _manager->setRepeat(repeat);
}

bool TauonPlayer::getShuffle() const
{
    return _status.shuffle;
}

void TauonPlayer::setShuffle(bool shuffle)
{
    _manager->setShuffle(shuffle);
}

bool TauonPlayer::getConsume() const
{
    return false;
}

void TauonPlayer::setConsume(bool consume)
{
    Q_UNUSED(consume)
}

QVariantMap TauonPlayer::getControls()
{
    QVariantMap controls;

    QStringList playerNames;
    for (TauonManager::Playlist playlist : _playlists) {
        playerNames << playlist.name;
    }
    controls["playlists"] = playerNames;

    controls["active_playlist"] = _activePlaylist;

    controls["volume"] = _status.volume;

    return controls;
}

void TauonPlayer::setControls(QVariantMap controls)
{
    if (_activePlaylist != controls["active_playlist"]) {
        _activePlaylist = controls["active_playlist"].toInt();
        _manager->setActivePlaylist(_playlists[_activePlaylist].id);
    }

    if (controls["volume"].toInt() != _status.volume) {
        _manager->setVolume(controls["volume"].toInt());
    }
}

void TauonPlayer::onGotStatus(Status status)
{
    Status oldStatus = _status;

    _status = status;

    if (oldStatus.status != status.status) {
        emit stateChanged();
    }

    if (oldStatus.progress != status.progress) {
        emit positionChanged();
    }

    if (oldStatus.duration != status.duration) {
        emit durationChanged();
    }

    if (oldStatus.currentTrackId != status.currentTrackId) {
        emit metadataChanged();
    }

    if (oldStatus.repeat != status.repeat) {
        emit repeatChanged();
    }

    if (oldStatus.shuffle != status.shuffle) {
        emit shuffleChanged();
    }

    emit controlsChanged();
}

void TauonPlayer::onGotPlaylists(QVector<TauonManager::Playlist> playlists)
{
    _playlists = playlists;

    if (_activePlaylist == -1 && !_playlists.isEmpty()) {
        _activePlaylist = 0;
        _manager->setActivePlaylist(_playlists[_activePlaylist].id);
    }

    if (_playlists.isEmpty()) {
        _activePlaylist = -1;
        _manager->setActivePlaylist(0);
    }

}
