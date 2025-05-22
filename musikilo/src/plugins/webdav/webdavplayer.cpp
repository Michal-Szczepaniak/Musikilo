#include "webdavplayer.h"

WebDavPlayer::WebDavPlayer(QWebdav *webdav, QObject *parent) : _webdav(webdav)
{
    connect(&_mediaPlayer, &QMediaPlayer::stateChanged, this, &PlayerInterface::stateChanged);
    connect(&_mediaPlayer, &QMediaPlayer::durationChanged, this, &PlayerInterface::durationChanged);
    connect(&_mediaPlayer, &QMediaPlayer::positionChanged, this, &PlayerInterface::positionChanged);
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
