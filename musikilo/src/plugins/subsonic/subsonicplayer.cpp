#include "subsonicplayer.h"

#include <QFileInfo>

SubsonicPlayer::SubsonicPlayer(SubsonicManager *manager, QObject *parent) : PlayerInterface(parent), _manager(manager)
{
    connect(manager, &SubsonicManager::errorOccured, this, &PlayerInterface::errorOccured);
    connect(&_mediaPlayer, &QMediaPlayer::stateChanged, this, &PlayerInterface::stateChanged);
    connect(&_mediaPlayer, &QMediaPlayer::durationChanged, this, &PlayerInterface::durationChanged);
    connect(&_mediaPlayer, &QMediaPlayer::positionChanged, this, &PlayerInterface::positionChanged);
    connect(&_mediaPlayer, static_cast<void (QMediaPlayer::*)()>(&QMediaPlayer::metaDataChanged), this, &PlayerInterface::metadataChanged);
    connect(manager, &SubsonicManager::gotSong, this, &SubsonicPlayer::gotSong);
}

QMediaPlayer::State SubsonicPlayer::getState()
{
    return _mediaPlayer.state();
}

void SubsonicPlayer::play(QString id)
{
    QNetworkRequest r = _manager->getStreamRequest(id);
    _manager->getSong(id);

    _mediaPlayer.stop();
    _mediaPlayer.setMedia(r);
    _mediaPlayer.play();
}

void SubsonicPlayer::play()
{
    _mediaPlayer.play();
}

void SubsonicPlayer::pause()
{
    _mediaPlayer.pause();
}

void SubsonicPlayer::stop()
{
    _mediaPlayer.stop();
}

qint64 SubsonicPlayer::getDuration() const
{
    return _mediaPlayer.duration();
}

qint64 SubsonicPlayer::getPosition() const
{
    return _mediaPlayer.position();
}

void SubsonicPlayer::setPosition(qint64 position)
{
    _mediaPlayer.setPosition(position);
}

QString SubsonicPlayer::getTitle()
{
    if (_mediaPlayer.metaData("Title").toString().isEmpty() && !_mediaPlayer.currentMedia().isNull()) {
        return _song.title;
    }

    return _mediaPlayer.metaData("Title").toString();
}

QString SubsonicPlayer::getArtist()
{
    if (_mediaPlayer.availableMetaData().contains("AlbumArtist"))
        return _mediaPlayer.metaData("AlbumArtist").toString();
    else if (_mediaPlayer.availableMetaData().contains("ContributingArtist"))
        return _mediaPlayer.metaData("ContributingArtist").toString();
    else
        return _song.artist;

    return QString();
}

QString SubsonicPlayer::getAlbum()
{
    if (_mediaPlayer.availableMetaData().contains("AlbumTitle"))
        return _mediaPlayer.metaData("AlbumTitle").toString();
    else
        return _song.album;
}

QString SubsonicPlayer::getBitrate()
{
    return QString::number(_mediaPlayer.metaData("AudioBitRate").toInt() / 1000);
}

bool SubsonicPlayer::getRepeat() const
{
    return _settings.value("ss-repeat", false).toBool();
}

void SubsonicPlayer::setRepeat(bool repeat)
{
    _settings.setValue("ss-repeat", repeat);

    emit repeatChanged();
}

bool SubsonicPlayer::getSingle() const
{
    return _settings.value("ss-single", false).toBool();
}

void SubsonicPlayer::setSingle(bool single)
{
    _settings.setValue("ss-single", single);

    emit singleChanged();
}

bool SubsonicPlayer::getShuffle() const
{
    return _settings.value("ss-shuffle", false).toBool();
}

void SubsonicPlayer::setShuffle(bool shuffle)
{
    _settings.setValue("ss-shuffle", shuffle);

    emit shuffleChanged();
}

bool SubsonicPlayer::getConsume() const
{
    return _settings.value("ss-consume", false).toBool();
}

void SubsonicPlayer::setConsume(bool consume)
{
    _settings.setValue("ss-consume", consume);

    emit consumeChanged();
}

QVariantMap SubsonicPlayer::getControls()
{
    return QVariantMap();
}

void SubsonicPlayer::setControls(QVariantMap controls)
{
    Q_UNUSED(controls)
}

void SubsonicPlayer::gotSong(PlaylistItem song)
{
    _song = song;

    emit metadataChanged();
}
