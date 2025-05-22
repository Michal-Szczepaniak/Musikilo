#include "player.h"

Player::Player(SettingsManager *settingsManager, QObject *parent) : QObject(parent), _settingsManager(settingsManager)
{
    connect(settingsManager, &SettingsManager::currentPluginChanged, this, &Player::onPluginChange);
}

QMediaPlayer::State Player::getState()
{
    if (_player == nullptr) return QMediaPlayer::State::StoppedState;

    return _player->getState();
}

void Player::play()
{
    if (_player == nullptr) return;

    _player->play();
}

void Player::pause()
{
    if (_player == nullptr) return;

    _player->pause();
}

void Player::stop()
{
    if (_player == nullptr) return;

    _player->stop();
}

qint64 Player::getDuration() const
{
    if (_player == nullptr) return 0;

    return _player->getDuration();
}

qint64 Player::getPosition() const
{
    if (_player == nullptr) return 0;

    return _player->getPosition();
}

void Player::setPosition(qint64 position)
{
    if (_player == nullptr) return;

    _player->setPosition(position);
}

QString Player::getTitle()
{
    if (_player == nullptr) return QString();

    return _player->getTitle();
}

QString Player::getArtist()
{
    if (_player == nullptr) return QString();

    return _player->getArtist();
}

QString Player::getAlbum()
{
    if (_player == nullptr) return QString();

    return _player->getAlbum();
}

QString Player::getBitrate()
{
    if (_player == nullptr) return QString();

    return _player->getBitrate();
}

void Player::onPluginChange()
{
    auto plugin = _settingsManager->getCurrentPlugin();
    if (plugin != nullptr) {
        if (_player != nullptr) {
            disconnect(_errorOccuredSignal);
            disconnect(_stateSignal);
            disconnect(_durationSignal);
            disconnect(_positionSignal);
            disconnect(_metadataSignal);
        }

        _player = plugin->getPlayer();
        _errorOccuredSignal = connect(_player, &PlayerInterface::errorOccured, this, &Player::errorOccured);
        _stateSignal = connect(_player, &PlayerInterface::stateChanged, this, &Player::stateChanged);
        _durationSignal = connect(_player, &PlayerInterface::durationChanged, this, &Player::durationChanged);
        _positionSignal = connect(_player, &PlayerInterface::positionChanged, this, &Player::positionChanged);
        _metadataSignal = connect(_player, &PlayerInterface::metadataChanged, this, &Player::metadataChanged);
    }

    emit pluginChanged();
    emit stateChanged();
    emit durationChanged();
    emit positionChanged();
    emit metadataChanged();
}
