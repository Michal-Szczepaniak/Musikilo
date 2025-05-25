#include "squeezeboxplugin.h"

SqueezeBoxPlugin::SqueezeBoxPlugin(QObject *parent) : PluginInterface(parent), _jcon(parent)
{
    connect(&_jcon, &jcon::JsonRpcTcpClient::socketConnected, [&](){
        if (_testPending) {
            emit testSucceeded();
            _testPending = false;
            _jcon.disconnectFromServer();

            return;
        }

        emit pluginReady();
    });

    connect(&_jcon, &jcon::JsonRpcTcpClient::socketError, [&](QObject* socket, QAbstractSocket::SocketError){
        if (_testPending) {
            emit testFailed(((QAbstractSocket*)socket)->errorString());
            _testPending = false;
            _jcon.disconnectFromServer();
        }
    });
}

void SqueezeBoxPlugin::initialize(QVariantMap settings)
{
    updateConig(settings);

    _fileModel = std::make_unique<SqueezeBoxFileModel>(&_jcon, this);
    _player = std::make_unique<SqueezeBoxPlayer>(&_jcon, this);
    _playlistModel = std::make_unique<SqueezeBoxPlaylistModel>(&_jcon, _player.get(), this);
}

void SqueezeBoxPlugin::updateConig(QVariantMap settings)
{
    _host = settings.value("hostname").toString();
    _port = settings.value("port").toString().toInt();
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

    _jcon.connectToServerAsync(_host, _port);
}

void SqueezeBoxPlugin::activate()
{
    _jcon.connectToServerAsync(_host, _port);
}

void SqueezeBoxPlugin::deactivate()
{
    _jcon.disconnectFromServer();
}
