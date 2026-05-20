#include "tauonplugin.h"

TauonPlugin::TauonPlugin(QObject *parent) : PluginInterface(parent), _manager(parent)
{
    _readyTimer.setInterval(10);
    _readyTimer.setSingleShot(true);
    connect(&_readyTimer, &QTimer::timeout, [&](){ emit pluginReady(); });

    connect(&_manager, &TauonManager::gotVersion, [&](){
        if (_testPending) {
            emit testSucceeded();
            _testPending = false;

            return;
        }

        emit pluginReady();
    });

    connect(&_manager, &TauonManager::errorOccured, [&](QString message){
        if (_testPending) {
            emit testFailed(message);
            _testPending = false;
        }

        _updateTimer.stop();
    });

    _updateTimer.setInterval(1000);
    connect(&_updateTimer, &QTimer::timeout, &_manager, &TauonManager::update);
}

void TauonPlugin::initialize(QVariantMap settings)
{
    updateConig(settings);

    _fileModel = std::make_unique<TauonFileModel>(&_manager, this);
    _player = std::make_unique<TauonPlayer>(&_manager, this);
    _playlistModel = std::make_unique<TauonPlaylistModel>(&_manager, _player.get(), this);
}

void TauonPlugin::updateConig(QVariantMap settings)
{
    QString host = settings.value("hostname").toString();
    int port = settings.value("port").toString().toInt();

    _manager.setConnectionSettings(host, port);
}

PlaylistModelInterface *TauonPlugin::getPlaylistModel()
{
    return _playlistModel.get();
}

FileModelInterface *TauonPlugin::getFileModel()
{
    return _fileModel.get();
}

PlayerInterface *TauonPlugin::getPlayer()
{
    return _player.get();
}

void TauonPlugin::testConfig()
{
    _testPending = true;

    _manager.getVersion();
}

void TauonPlugin::activate()
{
    _manager.getPlaylists();
    _updateTimer.start();
}

void TauonPlugin::deactivate()
{
    _updateTimer.stop();
}
