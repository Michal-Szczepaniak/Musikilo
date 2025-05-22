#ifndef PLAYER_H
#define PLAYER_H

#include "playerinterface.h"
#include "settingsmanager.h"

#include <QMediaPlayer>
#include <QObject>

class Player : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QMediaPlayer::State state READ getState NOTIFY stateChanged)
    Q_PROPERTY(qint64 duration READ getDuration NOTIFY durationChanged)
    Q_PROPERTY(qint64 position READ getPosition WRITE setPosition NOTIFY positionChanged)
public:
    explicit Player(SettingsManager *settingsManager, QObject *parent = nullptr);

    QMediaPlayer::State getState();

    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void stop();

    qint64 getDuration() const;
    qint64 getPosition() const;
    void setPosition(qint64 position);

signals:
    void errorOccured(QString error);
    void pluginChanged();
    void stateChanged();
    void durationChanged();
    void positionChanged();

public slots:
    void onPluginChange();

private:
    SettingsManager *_settingsManager;
    PlayerInterface *_player = nullptr;
    QMetaObject::Connection _errorOccuredSignal;
    QMetaObject::Connection _stateSignal;
    QMetaObject::Connection _durationSignal;
    QMetaObject::Connection _positionSignal;
};

#endif // PLAYER_H
