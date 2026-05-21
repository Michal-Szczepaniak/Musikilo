#include "subsonicplugin.h"

SubsonicPlugin::SubsonicPlugin(QObject *parent) : PluginInterface(parent), _manager(parent)
{
    _readyTimer.setInterval(10);
    _readyTimer.setSingleShot(true);
    connect(&_readyTimer, &QTimer::timeout, [&](){ emit pluginReady(); });

    connect(&_manager, &SubsonicManager::gotPing, [&](){
        if (_testPending) {
            emit testSucceeded();
            _testPending = false;

            return;
        }

        emit pluginReady();
    });

    connect(&_manager, &SubsonicManager::errorOccured, [&](QString message){
        if (_testPending) {
            emit testFailed(message);
            _testPending = false;
        }
    });
}

void SubsonicPlugin::initialize(QVariantMap settings)
{
    updateConig(settings);

    _fileModel = std::make_unique<SubsonicFileModel>(&_manager, this);
    _player = std::make_unique<SubsonicPlayer>(&_manager, this);
    _playlistModel = std::make_unique<SubsonicPlaylistModel>(&_manager, _player.get(), this);
}

void SubsonicPlugin::updateConig(QVariantMap settings)
{
    QString host = settings.value("hostname").toString();
    int port = settings.value("port").toString().toInt();
    QString username = settings.value("username").toString();
    QString password = settings.value("password").toString();
    int connectionType = settings.value("connectionType").toInt();

    _manager.setConnectionSettings(host, port, username, password, connectionType);
}

PlaylistModelInterface *SubsonicPlugin::getPlaylistModel()
{
    return _playlistModel.get();
}

FileModelInterface *SubsonicPlugin::getFileModel()
{
    return _fileModel.get();
}

PlayerInterface *SubsonicPlugin::getPlayer()
{
    return _player.get();
}

void SubsonicPlugin::testConfig()
{
    _testPending = true;

    _manager.ping();
}

void SubsonicPlugin::activate()
{
    _manager.getFilesList("/");
}

void SubsonicPlugin::deactivate()
{
}
