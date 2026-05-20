#include "kodiplugin.h"

KodiPlugin::KodiPlugin(QObject *parent) : PluginInterface(parent), _manager(parent)
{
    _readyTimer.setInterval(10);
    _readyTimer.setSingleShot(true);
    connect(&_readyTimer, &QTimer::timeout, [&](){ emit pluginReady(); });

    connect(&_manager, &KodiManager::gotPing, [&](){
        if (_testPending) {
            emit testSucceeded();
            _testPending = false;

            return;
        }

        emit pluginReady();
    });

    connect(&_manager, &KodiManager::errorOccured, [&](QString message){
        if (_testPending) {
            emit testFailed(message);
            _testPending = false;
        }

        _updateTimer.stop();
    });

    _updateTimer.setInterval(1000);
    connect(&_updateTimer, &QTimer::timeout, &_manager, &KodiManager::update);
}

void KodiPlugin::initialize(QVariantMap settings)
{
    updateConig(settings);

    _fileModel = std::make_unique<KodiFileModel>(&_manager, this);
    _player = std::make_unique<KodiPlayer>(&_manager, this);
    _playlistModel = std::make_unique<KodiPlaylistModel>(&_manager, _player.get(), this);
}

void KodiPlugin::updateConig(QVariantMap settings)
{
    QString host = settings.value("hostname").toString();
    int port = settings.value("port").toString().toInt();

    _manager.setConnectionSettings(host, port);
}

PlaylistModelInterface *KodiPlugin::getPlaylistModel()
{
    return _playlistModel.get();
}

FileModelInterface *KodiPlugin::getFileModel()
{
    return _fileModel.get();
}

PlayerInterface *KodiPlugin::getPlayer()
{
    return _player.get();
}

void KodiPlugin::testConfig()
{
    _testPending = true;

    _manager.ping();
}

void KodiPlugin::activate()
{
    _manager.getSources();
    _updateTimer.start();
}

void KodiPlugin::deactivate()
{
    _updateTimer.stop();
}
