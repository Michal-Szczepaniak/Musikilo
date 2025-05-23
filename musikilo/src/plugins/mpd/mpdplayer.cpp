#include "mpdplayer.h"

MPDPlayer::MPDPlayer(NetworkAccess *mpd, QObject *parent) : PlayerInterface(parent), _mpd(mpd), _mpdStatus(mpd->getMPDPlaybackStatus())
{
    connect(_mpdStatus, &MPDPlaybackStatus::playbackStatusChanged, [&](MPD_PLAYBACK_STATE){ emit stateChanged(); });
    connect(_mpdStatus, &MPDPlaybackStatus::currentTimeChanged, this, &PlayerInterface::positionChanged);
    connect(_mpdStatus, &MPDPlaybackStatus::lengthChanged, this, &PlayerInterface::durationChanged);
    connect(_mpdStatus, &MPDPlaybackStatus::titleChanged, this, &PlayerInterface::metadataChanged);
    connect(_mpdStatus, &MPDPlaybackStatus::albumChanged, this, &PlayerInterface::metadataChanged);
    connect(_mpdStatus, &MPDPlaybackStatus::artistChanged, this, &PlayerInterface::metadataChanged);
    connect(_mpdStatus, &MPDPlaybackStatus::bitrateChanged, this, &PlayerInterface::metadataChanged);
    connect(_mpdStatus, &MPDPlaybackStatus::volumeChanged, this, &MPDPlayer::onVolumeChanged);
    connect(_mpdStatus, &MPDPlaybackStatus::repeatChanged, this, &MPDPlayer::repeatChanged);
    connect(_mpdStatus, &MPDPlaybackStatus::singleChanged, this, &MPDPlayer::singleChanged);
    connect(_mpdStatus, &MPDPlaybackStatus::shuffleChanged, this, &MPDPlayer::shuffleChanged);
    connect(_mpdStatus, &MPDPlaybackStatus::consumeChanged, this, &MPDPlayer::consumeChanged);
    connect(_mpd, &NetworkAccess::outputsReady, this, &MPDPlayer::onOutputsChanged);

    connect(this, &MPDPlayer::playTrack, _mpd, &NetworkAccess::playTrack);
    connect(this, &MPDPlayer::mpdPause, _mpd, &NetworkAccess::pause);
    connect(this, &MPDPlayer::mpdStop, _mpd, &NetworkAccess::stop);
    connect(this, &MPDPlayer::seek, _mpd, &NetworkAccess::seek);
    connect(this, &MPDPlayer::mpdSetVolume, _mpd, &NetworkAccess::setVolume);
    connect(this, &MPDPlayer::mpdSetRepeat, _mpd, &NetworkAccess::setRepeat);
    connect(this, &MPDPlayer::mpdSetSingle, _mpd, &NetworkAccess::setSingle);
    connect(this, &MPDPlayer::mpdSetShuffle, _mpd, &NetworkAccess::setRandom);
    connect(this, &MPDPlayer::mpdSetConsume, _mpd, &NetworkAccess::setConsume);
    connect(this, &MPDPlayer::enableOutput, _mpd, &NetworkAccess::enableOutput);
    connect(this, &MPDPlayer::disableOutput, _mpd, &NetworkAccess::disableOutput);
    connect(this, &MPDPlayer::mpdRefreshDatabase, _mpd, &NetworkAccess::updateDB);

    _controls = _settings.value("controls-mpd", QVariantMap({
                                                    {"volume", 0},
                                                    {"outputs", QStringList()},
                                                    {"enabledOutputs", 0},
                                                })).toMap();
}

QMediaPlayer::State MPDPlayer::getState()
{
    MPD_PLAYBACK_STATE state = _mpdStatus->getPlaybackStatus();

    switch (state) {
    case MPD_PAUSE:
        return QMediaPlayer::PausedState;
    case MPD_PLAYING:
        return QMediaPlayer::PlayingState;
    case MPD_STOP:
        return QMediaPlayer::StoppedState;
    }
}

void MPDPlayer::play(QString path)
{
    emit playTrack(path);
}

void MPDPlayer::play()
{
    if (getState() == QMediaPlayer::PausedState) {
        emit mpdPause();
    }
}

void MPDPlayer::pause()
{
    if (getState() == QMediaPlayer::PlayingState) {
        emit mpdPause();
    }
}

void MPDPlayer::stop()
{
    emit mpdStop();
}

qint64 MPDPlayer::getDuration() const
{
    return _mpdStatus->getLength()*1000;
}

qint64 MPDPlayer::getPosition() const
{
    return _mpdStatus->getCurrentTime()*1000;
}

void MPDPlayer::setPosition(qint64 position)
{
    emit seek(position);
}

QString MPDPlayer::getTitle()
{
    return _mpdStatus->getTitle();
}

QString MPDPlayer::getArtist()
{
    return _mpdStatus->getArtist();
}

QString MPDPlayer::getAlbum()
{
    return _mpdStatus->getAlbum();
}

QString MPDPlayer::getBitrate()
{
    return QString::number(_mpdStatus->getBitrate());
}

bool MPDPlayer::getRepeat() const
{
    return _mpdStatus->getRepeat();
}

void MPDPlayer::setRepeat(bool repeat)
{
    emit mpdSetRepeat(repeat);
}

bool MPDPlayer::getSingle() const
{
    return _mpdStatus->getSingle();
}

void MPDPlayer::setSingle(bool single)
{
    emit mpdSetSingle(single ? 1 : 2);
}

bool MPDPlayer::getShuffle() const
{
    return _mpdStatus->getShuffle();
}

void MPDPlayer::setShuffle(bool shuffle)
{
    emit mpdSetShuffle(shuffle);
}

bool MPDPlayer::getConsume() const
{
    return _mpdStatus->getConsume();
}

void MPDPlayer::setConsume(bool consume)
{
    emit mpdSetConsume(consume);
}

QVariantMap MPDPlayer::getControls()
{
    return _controls;
}

void MPDPlayer::setControls(QVariantMap controls)
{
    if (controls["volume"] != _controls["volume"]) {
        emit mpdSetVolume(controls["volume"].toInt());
    }

    if (controls.contains("refresh")) {
        emit mpdRefreshDatabase();
        controls.remove("refresh");
    }

    if (controls["enabledOutputs"] != _controls["enabledOutputs"]) {
        int diff = (controls["enabledOutputs"].toInt() ^ _controls["enabledOutputs"].toInt());
        for (int i = 0; i < 32; i++) {
            if (diff & (1 << i)) {
                if (controls["enabledOutputs"].toInt() & (1 << i)) {
                    emit enableOutput(i);
                } else {
                    emit disableOutput(i);
                }
            }
        }
    }

    _controls = controls;

    _settings.setValue("controls-mpd", _controls);

    emit controlsChanged();
}

void MPDPlayer::onVolumeChanged(int volume)
{
    _controls["volume"] = volume;

    _settings.setValue("controls-mpd", _controls);

    emit controlsChanged();
}

void MPDPlayer::onOutputsChanged(QList<QObject *> *mpdOutputs)
{
    auto outputs = (QList<MPDOutput*>*) mpdOutputs;

    QStringList outputNames;
    int enabledOutputs = 0;

    MPDOutput *output;
    foreach (output, *outputs) {
        outputNames.append(output->getName());
        if (output->getEnabled()) {
            enabledOutputs |= (1 << output->getID());
        }
        delete output;
    }

    delete mpdOutputs;

    _controls["outputs"] = outputNames;
    _controls["enabledOutputs"] = enabledOutputs;

    _settings.setValue("controls-mpd", _controls);

    emit controlsChanged();
}
