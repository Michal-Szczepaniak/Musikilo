#include "squeezeboxplayer.h"

SqueezeBoxPlayer::SqueezeBoxPlayer(jcon::JsonRpcTcpClient *jcon, QObject *parent) : PlayerInterface(parent), _jcon(jcon)
{

}

QMediaPlayer::State SqueezeBoxPlayer::getState()
{

}

void SqueezeBoxPlayer::play(QString path)
{

}

void SqueezeBoxPlayer::play()
{

}

void SqueezeBoxPlayer::pause()
{

}

void SqueezeBoxPlayer::stop()
{

}

qint64 SqueezeBoxPlayer::getDuration() const
{
    return 0;
}

qint64 SqueezeBoxPlayer::getPosition() const
{
    return 0;
}

void SqueezeBoxPlayer::setPosition(qint64 position)
{

}

QString SqueezeBoxPlayer::getTitle()
{
    return QString();
}

QString SqueezeBoxPlayer::getArtist()
{
    return QString();
}

QString SqueezeBoxPlayer::getAlbum()
{
    return QString();
}

QString SqueezeBoxPlayer::getBitrate()
{
    return QString();
}

bool SqueezeBoxPlayer::getRepeat() const
{
    return false;
}

void SqueezeBoxPlayer::setRepeat(bool repeat)
{

}

bool SqueezeBoxPlayer::getSingle() const
{
    return false;
}

void SqueezeBoxPlayer::setSingle(bool single)
{

}

bool SqueezeBoxPlayer::getShuffle() const
{
    return false;
}

void SqueezeBoxPlayer::setShuffle(bool shuffle)
{

}

bool SqueezeBoxPlayer::getConsume() const
{
    return false;
}

void SqueezeBoxPlayer::setConsume(bool consume)
{

}

QVariantMap SqueezeBoxPlayer::getControls()
{
    return QVariantMap();
}

void SqueezeBoxPlayer::setControls(QVariantMap controls)
{

}
