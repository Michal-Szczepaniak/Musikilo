#include "kodiplayer.h"

KodiPlayer::KodiPlayer(KodiManager *manager, QObject *parent) : PlayerInterface(parent), _manager(manager)
{
    connect(manager, &KodiManager::errorOccured, this, &PlayerInterface::errorOccured);
    connect(manager, &KodiManager::gotStatus, this, &KodiPlayer::onGotStatus);
}

QMediaPlayer::State KodiPlayer::getState()
{
    if (_status.playing) {
        return QMediaPlayer::PlayingState;
    } else {
        if (_status.title.isEmpty()) {
            return QMediaPlayer::StoppedState;
        } else {
            return QMediaPlayer::PausedState;
        }
    }
}

void KodiPlayer::play()
{
    _manager->play();
}

void KodiPlayer::pause()
{
    _manager->pause();
}

void KodiPlayer::stop()
{
    _manager->stop();
}

qint64 KodiPlayer::getDuration() const
{
    return _status.duration;
}

qint64 KodiPlayer::getPosition() const
{
    return _status.progress;
}

void KodiPlayer::setPosition(qint64 position)
{
    _manager->seek(position);
}

QString KodiPlayer::getTitle()
{
    return _status.title;
}

QString KodiPlayer::getArtist()
{
    return _status.tagline;
}

QString KodiPlayer::getAlbum()
{
    return {};
}

QString KodiPlayer::getBitrate()
{
    return {};
}

bool KodiPlayer::getRepeat() const
{
    return _status.repeat;
}

void KodiPlayer::setRepeat(bool repeat)
{
    _manager->setRepeat(repeat);
}

bool KodiPlayer::getSingle() const
{
    return _status.single;
}

void KodiPlayer::setSingle(bool single)
{
    _manager->setSingle(single);
}

bool KodiPlayer::getShuffle() const
{
    return _status.shuffle;
}

void KodiPlayer::setShuffle(bool shuffle)
{
    _manager->setShuffle(shuffle);
}

bool KodiPlayer::getConsume() const
{
    return false;
}

void KodiPlayer::setConsume(bool consume)
{
    Q_UNUSED(consume)
}

QVariantMap KodiPlayer::getControls()
{
    QVariantMap controls;

    controls["volume"] = _status.volume;
    controls["stop"] = false;
    controls["currentAudioStream"] = _status.currentAudioStream;
    controls["currentVideoStream"] = _status.currentVideoStream;
    controls["currentSubtitle"] = _status.currentSubtitle;
    controls["audioStreams"] = _status.audioStreams;
    controls["videoStreams"] = _status.videoStreams;
    controls["subtitles"] = _status.subtitles;

    return controls;
}

void KodiPlayer::setControls(QVariantMap controls)
{
    if (controls["volume"].toInt() != _status.volume) {
        _manager->setVolume(controls["volume"].toInt());
    }

    if (controls["stop"].toBool()) {
        _manager->stop();
    }

    if (controls["left"].toBool()) {
        _manager->left();
    }

    if (controls["right"].toBool()) {
        _manager->right();
    }

    if (controls["up"].toBool()) {
        _manager->up();
    }

    if (controls["down"].toBool()) {
        _manager->down();
    }

    if (controls["menu"].toBool()) {
        _manager->menu();
    }

    if (controls["home"].toBool()) {
        _manager->home();
    }

    if (controls["back"].toBool()) {
        _manager->back();
    }

    if (controls["select"].toBool()) {
        _manager->select();
    }

    if (controls["prevChapter"].toBool()) {
        _manager->prevChapter();
    }

    if (controls["nextChapter"].toBool()) {
        _manager->nextChapter();
    }

    if (controls["currentAudioStream"].toInt() != _status.currentAudioStream) {
        _manager->setAudioStream(controls["currentAudioStream"].toInt());
    }

    if (controls["currentVideoStream"].toInt() != _status.currentVideoStream) {
        _manager->setVideoStream(controls["currentVideoStream"].toInt());
    }

    if (controls["currentSubtitle"].toInt() != _status.currentSubtitle) {
        _manager->setSubtitle(controls["currentSubtitle"].toInt());
    }
}

void KodiPlayer::onGotStatus(Status status)
{
    Status oldStatus = _status;

    _status = status;
/*
    qDebug() << "Status"
             << " playing: " << status.playing
             << " shuffle: " << status.shuffle
             << " repeat: " << status.repeat
             << " single: " << status.single
             << " progress: " << status.progress
             << " duration: " << status.duration
             << " volume: " << status.volume
             << " title: " << status.title;
*/
    if (oldStatus.playing != status.playing) {
        emit stateChanged();
    }

    if (oldStatus.progress != status.progress) {
        emit positionChanged();
    }

    if (oldStatus.duration != status.duration) {
        emit durationChanged();
    }

    if (oldStatus.title != status.title || oldStatus.tagline != status.tagline) {
        emit metadataChanged();
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

    if (oldStatus.volume != status.volume) {
        emit controlsChanged();
    }

    if (oldStatus.currentAudioStream != status.currentAudioStream) {
        emit controlsChanged();
    }

    if (oldStatus.currentVideoStream != status.currentVideoStream) {
        emit controlsChanged();
    }

    if (oldStatus.currentSubtitle != status.currentSubtitle) {
        emit controlsChanged();
    }

    if (oldStatus.audioStreams != status.audioStreams) {
        emit controlsChanged();
    }

    if (oldStatus.videoStreams != status.videoStreams) {
        emit controlsChanged();
    }

    if (oldStatus.subtitles != status.subtitles) {
        emit controlsChanged();
    }
}
