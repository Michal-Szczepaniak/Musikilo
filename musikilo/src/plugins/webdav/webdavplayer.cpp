#include "webdavplayer.h"

WebDavPlayer::WebDavPlayer(QWebdav *webdav, QObject *parent) : PlayerInterface(parent), _webdav(webdav)
{
    connect(&_mediaPlayer, &QMediaPlayer::stateChanged, this, &PlayerInterface::stateChanged);
    connect(&_mediaPlayer, &QMediaPlayer::durationChanged, this, &PlayerInterface::durationChanged);
    connect(&_mediaPlayer, &QMediaPlayer::positionChanged, this, &PlayerInterface::positionChanged);
    connect(&_mediaPlayer, static_cast<void (QMediaPlayer::*)()>(&QMediaPlayer::metaDataChanged), this, &PlayerInterface::metadataChanged);
}

QMediaPlayer::State WebDavPlayer::getState()
{
    return _mediaPlayer.state();
}

void WebDavPlayer::play(QString path)
{
    QNetworkRequest r = _webdav->getRequest(path);
    r.setRawHeader("Authorization", "Basic " + QString(_webdav->username() + ":" + _webdav->password()).toUtf8().toBase64());

    _mediaPlayer.stop();
    _mediaPlayer.setMedia(r);
    _mediaPlayer.play();
}

void WebDavPlayer::play()
{
    _mediaPlayer.play();
}

void WebDavPlayer::pause()
{
    _mediaPlayer.pause();
}

void WebDavPlayer::stop()
{
    _mediaPlayer.stop();
}

qint64 WebDavPlayer::getDuration() const
{
    return _mediaPlayer.duration();
}

qint64 WebDavPlayer::getPosition() const
{
    return _mediaPlayer.position();
}

void WebDavPlayer::setPosition(qint64 position)
{
    _mediaPlayer.setPosition(position);
}

QString WebDavPlayer::getTitle()
{
    return _mediaPlayer.metaData("Title").toString();
}

QString WebDavPlayer::getArtist()
{
    if (_mediaPlayer.availableMetaData().contains("AlbumArtist"))
        return _mediaPlayer.metaData("AlbumArtist").toString();
    else if (_mediaPlayer.availableMetaData().contains("ContributingArtist"))
        return _mediaPlayer.metaData("ContributingArtist").toString();

    return QString();
}

QString WebDavPlayer::getAlbum()
{
    return _mediaPlayer.metaData("AlbumTitle").toString();
}

QString WebDavPlayer::getBitrate()
{
    return QString::number(_mediaPlayer.metaData("AudioBitRate").toInt() / 1000);
}

bool WebDavPlayer::getRepeat() const
{
    return _settings.value("nc-repeat", false).toBool();
}

void WebDavPlayer::setRepeat(bool repeat)
{
    _settings.setValue("nc-repeat", repeat);

    emit repeatChanged();
}

bool WebDavPlayer::getSingle() const
{
    return _settings.value("nc-single", false).toBool();
}

void WebDavPlayer::setSingle(bool single)
{
    _settings.setValue("nc-single", single);

    emit singleChanged();
}

bool WebDavPlayer::getShuffle() const
{
    return _settings.value("nc-shuffle", false).toBool();
}

void WebDavPlayer::setShuffle(bool shuffle)
{
    _settings.setValue("nc-shuffle", shuffle);

    emit shuffleChanged();
}

bool WebDavPlayer::getConsume() const
{
    return _settings.value("nc-consume", false).toBool();
}

void WebDavPlayer::setConsume(bool consume)
{
    _settings.setValue("nc-consume", consume);

    emit consumeChanged();
}

QVariantMap WebDavPlayer::getControls()
{
    return QVariantMap();
}

void WebDavPlayer::setControls(QVariantMap controls)
{
    Q_UNUSED(controls)
}
