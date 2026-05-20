#ifndef TAUONPLAYER_H
#define TAUONPLAYER_H

#include "tauonmanager.h"

#include <QObject>

#include <src/playerinterface.h>

class TauonPlayer : public PlayerInterface
{
    using Status = TauonManager::Status;

    Q_OBJECT
public:
    explicit TauonPlayer(TauonManager *manager, QObject *parent = nullptr);

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
    bool getSingle() const { return false; };
    void setSingle(bool) {};
    bool getShuffle() const;
    void setShuffle(bool shuffle);
    bool getConsume() const;
    void setConsume(bool consume);

    QVariantMap getControls();
    void setControls(QVariantMap controls);

public slots:
    void onGotStatus(Status status);
    void onGotPlaylists(QVector<TauonManager::Playlist> playlists);

signals:

private:
    TauonManager *_manager;
    QVector<TauonManager::Playlist> _playlists{};
    int _activePlaylist = -1;
    Status _status{};
    QStringList _syncPlayers{};
};

#endif // TAUONPLAYER_H
