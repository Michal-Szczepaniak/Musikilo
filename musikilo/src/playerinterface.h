#ifndef PLAYERINTERFACE_H
#define PLAYERINTERFACE_H

#include <QObject>
#include <QMediaPlayer>

class PlayerInterface : public QObject
{
    Q_OBJECT
public:
    PlayerInterface(QObject *parent = nullptr) : QObject(parent) {};
    virtual ~PlayerInterface(){};

    virtual QMediaPlayer::State getState() = 0;

    virtual void play() = 0;
    virtual void pause() = 0;
    virtual void stop() = 0;

    virtual qint64 getDuration() const = 0;
    virtual qint64 getPosition() const = 0;
    virtual void setPosition(qint64 position) = 0;

    virtual QString getTitle() = 0;
    virtual QString getArtist() = 0;
    virtual QString getAlbum() = 0;
    virtual QString getBitrate() = 0;

    virtual bool getRepeat() const = 0;
    virtual void setRepeat(bool repeat) = 0;
    virtual bool getSingle() const = 0;
    virtual void setSingle(bool single) = 0;
    virtual bool getShuffle() const = 0;
    virtual void setShuffle(bool shuffle) = 0;
    virtual bool getConsume() const = 0;
    virtual void setConsume(bool consume) = 0;

    virtual QVariantMap getControls() = 0;
    virtual void setControls(QVariantMap controls) = 0;

    using QObject::parent;

signals:
    void errorOccured(QString error);
    void stateChanged();
    void positionChanged();
    void durationChanged();
    void metadataChanged();
    void controlsChanged();
    void repeatChanged();
    void singleChanged();
    void shuffleChanged();
    void consumeChanged();
};

#endif // PLAYERINTERFACE_H
