#ifndef MPDPLAYER_H
#define MPDPLAYER_H

#include <QObject>

#include <src/playerinterface.h>

#include <mpd/networkaccess.h>

class MPDPlayer : public PlayerInterface
{
    Q_OBJECT
public:
    explicit MPDPlayer(NetworkAccess *_mpd, QObject *parent = nullptr);

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

signals:
    void playTrack(QString path);
    void mpdPause();
    void mpdStop();
    void seek(int position);
    void mpdSetVolume(int volume);
    void mpdSetRepeat(bool repeat);
    void mpdSetSingle(quint8 single);
    void mpdSetShuffle(bool shuffle);
    void mpdSetConsume(bool consume);
    void enableOutput(int id);
    void disableOutput(int id);
    void mpdRefreshDatabase();

public slots:
    void onVolumeChanged(int volume);
    void onOutputsChanged(QList<QObject*> *mpdOutputs);

private:
    NetworkAccess *_mpd;
    MPDPlaybackStatus *_mpdStatus;
    QVariantMap _controls;
    QSettings _settings;
};

#endif // MPDPLAYER_H
