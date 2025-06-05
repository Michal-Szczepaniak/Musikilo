#ifndef SQUEEZEBOXPLAYER_H
#define SQUEEZEBOXPLAYER_H

#include "squeezeboxmanager.h"

#include <QObject>

#include <src/playerinterface.h>

using Status = SqueezeBoxManager::Status;

class SqueezeBoxPlayer : public PlayerInterface
{
    Q_OBJECT
public:
    explicit SqueezeBoxPlayer(SqueezeBoxManager *manager, QObject *parent = nullptr);

    QMediaPlayer::State getState();

    void play();
    void pause();
    void stop();

    qint64 getDuration() const;
    qint64 getPosition() const;
    void setPosition(qint64 position);

    QString getTitle();
    QString getArtist();
    QString getAlbum();
    QString getBitrate();

    bool getRepeat() const;
    void setRepeat(bool repeat);
    bool getSingle() const;
    void setSingle(bool single);
    bool getShuffle() const;
    void setShuffle(bool shuffle);
    bool getConsume() const;
    void setConsume(bool consume);

    QVariantMap getControls();
    void setControls(QVariantMap controls);

public slots:
    void onGotStatus(Status status);
    void onGotPlayers(QVector<SqueezeBoxManager::Player> players);
    void onGotSyncPlayers(QStringList players);

signals:

private:
    SqueezeBoxManager *_manager;
    QVector<SqueezeBoxManager::Player> _players{};
    QHash<QString, int> _playerIds{};
    int _activePlayer = -1;
    Status _status{};
    QStringList _syncPlayers{};
};

#endif // SQUEEZEBOXPLAYER_H
