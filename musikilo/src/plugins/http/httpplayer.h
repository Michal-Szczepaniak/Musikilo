#ifndef HTTPPLAYER_H
#define HTTPPLAYER_H

#include <src/playerinterface.h>

#include <QMediaPlayer>
#include <QSettings>

class HttpPlayer : public PlayerInterface
{
    Q_OBJECT
public:
    HttpPlayer(QObject *parent = nullptr);

    QMediaPlayer::State getState();

    void setPath(QString path);

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

private:
    QMediaPlayer _mediaPlayer;
    QString _path;
};

#endif // HTTPPLAYER_H
