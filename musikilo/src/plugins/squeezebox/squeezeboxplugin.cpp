#include "squeezeboxplugin.h"

SqueezeBoxPlugin::SqueezeBoxPlugin(QObject *parent) : PluginInterface(parent), _manager(parent)
{
    _readyTimer.setInterval(10);
    _readyTimer.setSingleShot(true);
    connect(&_readyTimer, &QTimer::timeout, [&](){ emit pluginReady(); });

    connect(&_manager, &SqueezeBoxManager::gotVersion, [&](){
        if (_testPending) {
            emit testSucceeded();
            _testPending = false;

            return;
        }

        emit pluginReady();
    });

    connect(&_manager, &SqueezeBoxManager::errorOccured, [&](QString message){
        if (_testPending) {
            emit testFailed(message);
            _testPending = false;
        }

        _updateTimer.stop();
    });

    _updateTimer.setInterval(1000);
    connect(&_updateTimer, &QTimer::timeout, &_manager, &SqueezeBoxManager::update);
}

void SqueezeBoxPlugin::initialize(QVariantMap settings)
{
    updateConig(settings);

    _fileModel = std::make_unique<SqueezeBoxFileModel>(&_manager, this);
    _player = std::make_unique<SqueezeBoxPlayer>(&_manager, this);
    _playlistModel = std::make_unique<SqueezeBoxPlaylistModel>(&_manager, _player.get(), this);
}

void SqueezeBoxPlugin::updateConig(QVariantMap settings)
{
    QString host = settings.value("hostname").toString();
    int port = settings.value("port").toString().toInt();

    _manager.setConnectionSettings(host, port);
}

PlaylistModelInterface *SqueezeBoxPlugin::getPlaylistModel()
{
    return _playlistModel.get();
}

FileModelInterface *SqueezeBoxPlugin::getFileModel()
{
    return _fileModel.get();
}

PlayerInterface *SqueezeBoxPlugin::getPlayer()
{
    return _player.get();
}

void SqueezeBoxPlugin::testConfig()
{
    _testPending = true;

    _manager.getVersion();
}

void SqueezeBoxPlugin::activate()
{
    _manager.getVersion();
    _updateTimer.start();
}

void SqueezeBoxPlugin::deactivate()
{
    _updateTimer.stop();
}
