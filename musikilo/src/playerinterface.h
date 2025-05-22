#ifndef PLAYERINTERFACE_H
#define PLAYERINTERFACE_H

#include <QObject>
#include <QMediaPlayer>

class PlayerInterface : public QObject
{
    Q_OBJECT
public:
    PlayerInterface(){};
    virtual ~PlayerInterface(){};

    virtual QMediaPlayer::State getState() = 0;

    virtual void play() = 0;
    virtual void pause() = 0;
    virtual void stop() = 0;

    virtual qint64 getDuration() const = 0;
    virtual qint64 getPosition() const = 0;
    virtual void setPosition(qint64 position) = 0;

    using QObject::parent;

signals:
    void errorOccured(QString error);
    void stateChanged();
    void positionChanged();
    void durationChanged();
};

#endif // PLAYERINTERFACE_H
