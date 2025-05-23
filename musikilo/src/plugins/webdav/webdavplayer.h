#ifndef WEBDAVPLAYER_H
#define WEBDAVPLAYER_H

#include <src/playerinterface.h>

#include <QMediaPlayer>
#include <qwebdav.h>

class WebDavPlayer : public PlayerInterface
{
    Q_OBJECT
public:
    WebDavPlayer(QWebdav *webdav, QObject *parent = nullptr);

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

private:
    QWebdav *_webdav;
    QMediaPlayer _mediaPlayer;
    QSettings _settings;
};

#endif // WEBDAVPLAYER_H
