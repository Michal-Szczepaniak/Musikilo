#ifndef SQUEEZEBOXPLAYER_H
#define SQUEEZEBOXPLAYER_H

#include <QObject>

#include <jcon/json_rpc_tcp_client.h>

#include <src/playerinterface.h>

class SqueezeBoxPlayer : public PlayerInterface
{
    Q_OBJECT
public:
    explicit SqueezeBoxPlayer(jcon::JsonRpcTcpClient *jcon, QObject *parent = nullptr);

    QMediaPlayer::State getState();

    void play(QString path);
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

signals:

private:
    jcon::JsonRpcTcpClient *_jcon;
};

#endif // SQUEEZEBOXPLAYER_H
