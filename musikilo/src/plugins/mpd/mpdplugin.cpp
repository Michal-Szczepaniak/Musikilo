#include "mpdplugin.h"

MPDPlugin::MPDPlugin(QObject *parent) : PluginInterface(parent), _mpdThread(new QThread(this)), _testPending(false)
{
    _mpd.moveToThread(_mpdThread);
    _mpdThread->start();
    _mpd.setUpdateInterval(1000);

    connect(&_mpd, &NetworkAccess::connectionEstablished, [&](){
        if (_testPending) {
            emit testSucceeded();
            _testPending = false;
            emit disconnectFromHost();
        }

        emit pluginReady();
    });
    connect(&_mpd, &NetworkAccess::connectionError, [&](QString message){
        if (_testPending) {
            emit testFailed(message);
            _testPending = false;
            emit disconnectFromHost();
        }
    });

    connect(this, &MPDPlugin::connectToHost, &_mpd, static_cast<void (NetworkAccess::*)()>(&NetworkAccess::connectToHost));
    connect(this, &MPDPlugin::disconnectFromHost, &_mpd, &NetworkAccess::disconnectFromServer);

//    connect(&_mpd, &NetworkAccess::)
}

void MPDPlugin::initialize(QVariantMap settings)
{
    updateConig(settings);

    _fileModel = std::make_unique<MPDFileModel>(&_mpd, this);
    _player = std::make_unique<MPDPlayer>(&_mpd, this);
    _playlistModel = std::make_unique<MPDPlaylistModel>(&_mpd, _player.get(), this);
}

void MPDPlugin::updateConig(QVariantMap settings)
{
    QString hostname = settings.value("hostname").toString();
    QString password = settings.value("password").toString();
    int port = settings.value("port").toString().toInt();

    qDebug() << "Nextcloud settings: ";
    qDebug() << "Hostname: " << hostname;
    qDebug() << "Port: " << port;

    _mpd.setConnectParameters(hostname, port, password);
}

PlaylistModelInterface *MPDPlugin::getPlaylistModel()
{
    return _playlistModel.get();
}

FileModelInterface *MPDPlugin::getFileModel()
{
    return _fileModel.get();
}

PlayerInterface *MPDPlugin::getPlayer()
{
    return _player.get();
}

void MPDPlugin::testConfig()
{
    _testPending = true;
    emit connectToHost();
}

void MPDPlugin::activate()
{
    emit connectToHost();
}

void MPDPlugin::deactivate()
{
    emit disconnectFromHost();
}
