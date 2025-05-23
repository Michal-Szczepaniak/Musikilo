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

bool Player::getRepeat() const
{
    if (_player == nullptr) return false;

    return _player->getRepeat();
}

void Player::setRepeat(bool repeat)
{
    if (_player == nullptr) return;

    return _player->setRepeat(repeat);
}

bool Player::getSingle() const
{
    if (_player == nullptr) return false;

    return _player->getSingle();
}

void Player::setSingle(bool single)
{
    if (_player == nullptr) return;

    return _player->setSingle(single);
}

bool Player::getShuffle() const
{
    if (_player == nullptr) return false;

    return _player->getShuffle();
}

void Player::setShuffle(bool shuffle)
{
    if (_player == nullptr) return;

    return _player->setShuffle(shuffle);
}

bool Player::getConsume() const
{
    if (_player == nullptr) return false;

    return _player->getConsume();
}

void Player::setConsume(bool consume)
{
    if (_player == nullptr) return;

    return _player->setConsume(consume);
}

QVariantMap Player::getPluginControls() const
{
    if (_player == nullptr) return QVariantMap();

    return _player->getControls();
}

void Player::setPluginControls(QVariantMap controls)
{
    if (_player == nullptr) return;

    _player->setControls(controls);
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
            disconnect(_controlsSignal);
            disconnect(_repeatSignal);
            disconnect(_singleSignal);
            disconnect(_shuffleSignal);
            disconnect(_consumeSignal);
        }

        _player = plugin->getPlayer();
        _errorOccuredSignal = connect(_player, &PlayerInterface::errorOccured, this, &Player::errorOccured);
        _stateSignal = connect(_player, &PlayerInterface::stateChanged, this, &Player::stateChanged);
        _durationSignal = connect(_player, &PlayerInterface::durationChanged, this, &Player::durationChanged);
        _positionSignal = connect(_player, &PlayerInterface::positionChanged, this, &Player::positionChanged);
        _metadataSignal = connect(_player, &PlayerInterface::metadataChanged, this, &Player::metadataChanged);
        _controlsSignal = connect(_player, &PlayerInterface::controlsChanged, this, &Player::pluginControlsChanged);
        _repeatSignal = connect(_player, &PlayerInterface::repeatChanged, this, &Player::repeatChanged);
        _singleSignal = connect(_player, &PlayerInterface::singleChanged, this, &Player::singleChanged);
        _shuffleSignal = connect(_player, &PlayerInterface::shuffleChanged, this, &Player::shuffleChanged);
        _consumeSignal = connect(_player, &PlayerInterface::consumeChanged, this, &Player::consumeChanged);
    }

    emit pluginChanged();
    emit stateChanged();
    emit durationChanged();
    emit positionChanged();
    emit metadataChanged();
    emit pluginControlsChanged();
    emit repeatChanged();
    emit singleChanged();
    emit shuffleChanged();
    emit consumeChanged();
}
