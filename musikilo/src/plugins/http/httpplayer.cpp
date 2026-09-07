#include "httpplayer.h"

#include <QFileInfo>

HttpPlayer::HttpPlayer(QObject *parent) : PlayerInterface(parent)
{
    connect(&_mediaPlayer, &QMediaPlayer::stateChanged, this, &PlayerInterface::stateChanged);
    connect(&_mediaPlayer, &QMediaPlayer::durationChanged, this, &PlayerInterface::durationChanged);
    connect(&_mediaPlayer, &QMediaPlayer::positionChanged, this, &PlayerInterface::positionChanged);
    connect(&_mediaPlayer, static_cast<void (QMediaPlayer::*)()>(&QMediaPlayer::metaDataChanged), this, &PlayerInterface::metadataChanged);
}

QMediaPlayer::State HttpPlayer::getState()
{
    return _mediaPlayer.state();
}

void HttpPlayer::setPath(QString path)
{
    _path = path;

    _mediaPlayer.setMedia(QUrl(path));
}

void HttpPlayer::play()
{
    _mediaPlayer.play();
}

void HttpPlayer::pause()
{
    _mediaPlayer.pause();
}

void HttpPlayer::stop()
{
    _mediaPlayer.stop();
}

qint64 HttpPlayer::getDuration() const
{
    return _mediaPlayer.duration();
}

qint64 HttpPlayer::getPosition() const
{
    return _mediaPlayer.position();
}

void HttpPlayer::setPosition(qint64 position)
{
    Q_UNUSED(position)

    return;
}

QString HttpPlayer::getTitle()
{
    if (_mediaPlayer.metaData("Title").toString().isEmpty() && !_mediaPlayer.currentMedia().isNull()) {
        QFileInfo path = _mediaPlayer.currentMedia().canonicalUrl().toString();
        return path.baseName();
    }

    return _mediaPlayer.metaData("Title").toString();
}

QString HttpPlayer::getArtist()
{
    if (_mediaPlayer.availableMetaData().contains("AlbumArtist"))
        return _mediaPlayer.metaData("AlbumArtist").toString();
    else if (_mediaPlayer.availableMetaData().contains("ContributingArtist"))
        return _mediaPlayer.metaData("ContributingArtist").toString();

    return QString();
}

QString HttpPlayer::getAlbum()
{
    return _mediaPlayer.metaData("AlbumTitle").toString();
}

QString HttpPlayer::getBitrate()
{
    return QString::number(_mediaPlayer.metaData("AudioBitRate").toInt() / 1000);
}

bool HttpPlayer::getRepeat() const
{
    return false;
}

void HttpPlayer::setRepeat(bool repeat)
{
    Q_UNUSED(repeat)
}

bool HttpPlayer::getSingle() const
{
    return false;
}

void HttpPlayer::setSingle(bool single)
{
    Q_UNUSED(single)
}

bool HttpPlayer::getShuffle() const
{
    return false;
}

void HttpPlayer::setShuffle(bool shuffle)
{
    Q_UNUSED(shuffle)
}

bool HttpPlayer::getConsume() const
{
    return false;
}

void HttpPlayer::setConsume(bool consume)
{
    Q_UNUSED(consume)
}

QVariantMap HttpPlayer::getControls()
{
    return QVariantMap();
}

void HttpPlayer::setControls(QVariantMap controls)
{
    Q_UNUSED(controls)
}
