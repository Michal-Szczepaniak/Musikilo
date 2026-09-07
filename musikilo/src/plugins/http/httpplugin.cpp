#include "httpplugin.h"

HttpPlugin::HttpPlugin(QObject *parent) : PluginInterface(parent)
{
    connect(&_manager, &QNetworkAccessManager::finished, this, [&](QNetworkReply *reply){
        if (reply->error() != QNetworkReply::NoError) {
            emit testFailed(reply->errorString());
            return;
        }

        emit testSucceeded();
    });
}

void HttpPlugin::initialize(QVariantMap settings)
{
    _fileModel = std::make_unique<HttpFileModel>(this);
    _player = std::make_unique<HttpPlayer>(this);
    _playlistModel = std::make_unique<HttpPlaylistModel>(_player.get(), this);

    updateConig(settings);
}

void HttpPlugin::updateConig(QVariantMap settings)
{
    QString path = settings.value("path").toString();

    if (_path != path) {
        _path = path;
        _player->setPath(path);
    }
}

PlaylistModelInterface *HttpPlugin::getPlaylistModel()
{
    return _playlistModel.get();
}

FileModelInterface *HttpPlugin::getFileModel()
{
    return _fileModel.get();
}

PlayerInterface *HttpPlugin::getPlayer()
{
    return _player.get();
}

void HttpPlugin::testConfig()
{
    QFileInfo path(_path);

    if (path.exists() && path.isDir()) {
        emit testSucceeded();
    } else {
        if (!path.exists()) {
            emit testFailed("Path directory does not exist");
        } else if (!path.isDir()) {
            emit testFailed("Path directory is not a directory");
        }

        emit testFailed("Go play lottery because I've no clue how you got here");
    }
}

void HttpPlugin::activate()
{
    _player->play();
}

void HttpPlugin::deactivate()
{
    _player->stop();
}
