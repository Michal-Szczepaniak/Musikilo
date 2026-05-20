#ifndef KODIPLAYER_H
#define KODIPLAYER_H

#include "kodimanager.h"

#include <QObject>

#include <src/playerinterface.h>

class KodiPlayer : public PlayerInterface
{
    using Status = KodiManager::Status;

    Q_OBJECT
public:
    explicit KodiPlayer(KodiManager *manager, QObject *parent = nullptr);

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

signals:

private:
    KodiManager *_manager;
    int _activePlaylist = -1;
    Status _status{};
    QStringList _syncPlayers{};
};

#endif // KODIPLAYER_H
