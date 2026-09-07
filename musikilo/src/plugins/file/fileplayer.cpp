#include "fileplayer.h"

#include <QFileInfo>

FilePlayer::FilePlayer(QObject *parent) : PlayerInterface(parent)
{
    connect(&_mediaPlayer, &QMediaPlayer::stateChanged, this, &PlayerInterface::stateChanged);
    connect(&_mediaPlayer, &QMediaPlayer::durationChanged, this, &PlayerInterface::durationChanged);
    connect(&_mediaPlayer, &QMediaPlayer::positionChanged, this, &PlayerInterface::positionChanged);
    connect(&_mediaPlayer, static_cast<void (QMediaPlayer::*)()>(&QMediaPlayer::metaDataChanged), this, &PlayerInterface::metadataChanged);
}

QMediaPlayer::State FilePlayer::getState()
{
    return _mediaPlayer.state();
}

void FilePlayer::play(QString path)
{
    _mediaPlayer.stop();
    _mediaPlayer.setMedia(QUrl::fromLocalFile(path));
    _mediaPlayer.play();
}

void FilePlayer::play()
{
    _mediaPlayer.play();
}

void FilePlayer::pause()
{
    _mediaPlayer.pause();
}

void FilePlayer::stop()
{
    _mediaPlayer.stop();
}

qint64 FilePlayer::getDuration() const
{
    return _mediaPlayer.duration();
}

qint64 FilePlayer::getPosition() const
{
    return _mediaPlayer.position();
}

void FilePlayer::setPosition(qint64 position)
{
    _mediaPlayer.setPosition(position);
}

QString FilePlayer::getTitle()
{
    if (_mediaPlayer.metaData("Title").toString().isEmpty() && !_mediaPlayer.currentMedia().isNull()) {
        QFileInfo path = _mediaPlayer.currentMedia().canonicalUrl().toString();
        return path.baseName();
    }

    return _mediaPlayer.metaData("Title").toString();
}

QString FilePlayer::getArtist()
{
    if (_mediaPlayer.availableMetaData().contains("AlbumArtist"))
        return _mediaPlayer.metaData("AlbumArtist").toString();
    else if (_mediaPlayer.availableMetaData().contains("ContributingArtist"))
        return _mediaPlayer.metaData("ContributingArtist").toString();

    return QString();
}

QString FilePlayer::getAlbum()
{
    return _mediaPlayer.metaData("AlbumTitle").toString();
}

QString FilePlayer::getBitrate()
{
    return QString::number(_mediaPlayer.metaData("AudioBitRate").toInt() / 1000);
}

bool FilePlayer::getRepeat() const
{
    return _settings.value("nc-repeat", false).toBool();
}

void FilePlayer::setRepeat(bool repeat)
{
    _settings.setValue("nc-repeat", repeat);

    emit repeatChanged();
}

bool FilePlayer::getSingle() const
{
    return _settings.value("nc-single", false).toBool();
}

void FilePlayer::setSingle(bool single)
{
    _settings.setValue("nc-single", single);

    emit singleChanged();
}

bool FilePlayer::getShuffle() const
{
    return _settings.value("nc-shuffle", false).toBool();
}

void FilePlayer::setShuffle(bool shuffle)
{
    _settings.setValue("nc-shuffle", shuffle);

    emit shuffleChanged();
}

bool FilePlayer::getConsume() const
{
    return _settings.value("nc-consume", false).toBool();
}

void FilePlayer::setConsume(bool consume)
{
    _settings.setValue("nc-consume", consume);

    emit consumeChanged();
}

QVariantMap FilePlayer::getControls()
{
    return QVariantMap();
}

void FilePlayer::setControls(QVariantMap controls)
{
    Q_UNUSED(controls)
}
