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
    Q_PROPERTY(QString title READ getTitle NOTIFY metadataChanged)
    Q_PROPERTY(QString artist READ getArtist NOTIFY metadataChanged)
    Q_PROPERTY(QString album READ getAlbum NOTIFY metadataChanged)
    Q_PROPERTY(QString bitrate READ getBitrate NOTIFY metadataChanged)
    Q_PROPERTY(bool repeat READ getRepeat WRITE setRepeat NOTIFY repeatChanged)
    Q_PROPERTY(bool single READ getSingle WRITE setSingle NOTIFY singleChanged)
    Q_PROPERTY(bool shuffle READ getShuffle WRITE setShuffle NOTIFY shuffleChanged)
    Q_PROPERTY(bool consume READ getConsume WRITE setConsume NOTIFY consumeChanged)
    Q_PROPERTY(QVariantMap pluginControls READ getPluginControls WRITE setPluginControls NOTIFY pluginControlsChanged)
public:
    explicit Player(SettingsManager *settingsManager, QObject *parent = nullptr);

    QMediaPlayer::State getState();

    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void stop();

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

    QVariantMap getPluginControls() const;
    void setPluginControls(QVariantMap controls);

signals:
    void errorOccured(QString error);
    void pluginChanged();
    void stateChanged();
    void durationChanged();
    void positionChanged();
    void metadataChanged();
    void pluginControlsChanged();
    void repeatChanged();
    void singleChanged();
    void shuffleChanged();
    void consumeChanged();

public slots:
    void onPluginChange();

private:
    SettingsManager *_settingsManager;
    PlayerInterface *_player = nullptr;
    QMetaObject::Connection _errorOccuredSignal;
    QMetaObject::Connection _stateSignal;
    QMetaObject::Connection _durationSignal;
    QMetaObject::Connection _positionSignal;
    QMetaObject::Connection _metadataSignal;
    QMetaObject::Connection _controlsSignal;
    QMetaObject::Connection _repeatSignal;
    QMetaObject::Connection _singleSignal;
    QMetaObject::Connection _shuffleSignal;
    QMetaObject::Connection _consumeSignal;
};

#endif // PLAYER_H
