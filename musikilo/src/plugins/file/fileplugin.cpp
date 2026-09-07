#include "fileplugin.h"

FilePlugin::FilePlugin(QObject *parent) : PluginInterface(parent)
{
}

void FilePlugin::initialize(QVariantMap settings)
{
    _fileModel = std::make_unique<FileFileModel>(this);
    _player = std::make_unique<FilePlayer>(this);
    _playlistModel = std::make_unique<FilePlaylistModel>(_player.get(), _fileModel.get(), this);

    updateConig(settings);
}

void FilePlugin::updateConig(QVariantMap settings)
{
    QString path = settings.value("path").toString();

    if (_path != path) {
        _path = path;
        _fileModel->setPath(path);
    }
}

PlaylistModelInterface *FilePlugin::getPlaylistModel()
{
    return _playlistModel.get();
}

FileModelInterface *FilePlugin::getFileModel()
{
    return _fileModel.get();
}

PlayerInterface *FilePlugin::getPlayer()
{
    return _player.get();
}

void FilePlugin::testConfig()
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

void FilePlugin::activate()
{}

void FilePlugin::deactivate()
{}
