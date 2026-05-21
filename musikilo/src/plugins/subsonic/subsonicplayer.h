#ifndef SUBSONICPLAYER_H
#define SUBSONICPLAYER_H

#include "subsonicmanager.h"

#include <QObject>
#include <QSettings>

#include <src/playerinterface.h>

class SubsonicPlayer : public PlayerInterface
{
    using PlaylistItem = SubsonicManager::PlaylistItem;

    Q_OBJECT
public:
    explicit SubsonicPlayer(SubsonicManager *manager, QObject *parent = nullptr);

    QMediaPlayer::State getState();

    void play(QString id);
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
    void gotSong(PlaylistItem song);

private:
    SubsonicManager *_manager;
    QMediaPlayer _mediaPlayer;
    QSettings _settings;
    PlaylistItem _song;
};

#endif // SUBSONICPLAYER_H
