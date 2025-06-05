#include "squeezeboxfilemodel.h"

SqueezeBoxFileModel::SqueezeBoxFileModel(SqueezeBoxManager *manager, QObject *parent) : FileModelInterface(parent), _manager(manager)
{
    connect(_manager, &SqueezeBoxManager::gotMusicFolder, this, &SqueezeBoxFileModel::onGotMusicFolder);
    connect(_manager, &SqueezeBoxManager::gotTopMusicFolder, this, &SqueezeBoxFileModel::onGotTopMusicFolder);
}

int SqueezeBoxFileModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return _entries.size();
}

QVariant SqueezeBoxFileModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount())
        return QVariant();

    const SqueezeBoxManager::File &file = _entries[index.row()];
    if (role == Name)
        return file.name;
    else if (role == Path)
        return file.url;
    else if (role == isDir)
        return file.type == SqueezeBoxManager::Folder;

    return QVariant();
}

void SqueezeBoxFileModel::getFilesList(QString path)
{
    if (path == "/" && _topPath.isEmpty()) {
        _manager->getMusicFolder();
    } else {
        _manager->getFilesList(path);
    }
}

QHash<int, QByteArray> SqueezeBoxFileModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Name] = "name";
    roles[Path] = "path";
    roles[isDir] = "isDir";
    return roles;
}

void SqueezeBoxFileModel::onGotMusicFolder(QVector<SqueezeBoxManager::File> folder)
{
    beginResetModel();
    _entries.clear();

    SqueezeBoxManager::File previousDirectory = {
        .name = "..",
        .id = 0,
        .type = SqueezeBoxManager::Folder,
        .url = getPreviousPath(folder.first()),
    };

    _entries.append(previousDirectory);

    SqueezeBoxManager::File item;
    foreach(item, folder) {
        if (item.type == SqueezeBoxManager::Folder) {
            _entries << item;
        }
    }

    foreach(item, folder) {
        if (item.type == SqueezeBoxManager::Track) {
            _entries << item;
        }
    }

    _entries.append(previousDirectory);

    endResetModel();
}

void SqueezeBoxFileModel::onGotTopMusicFolder(SqueezeBoxManager::File folder)
{
    _topPath = folder.url;

    getFilesList("/");
}

QString SqueezeBoxFileModel::getPreviousPath(SqueezeBoxManager::File file)
{
    QString path = file.url;
    if (path.endsWith('/')) {
        path = path.left(path.length()-1);
    }
    path = path.left(path.lastIndexOf(QChar('/')));

    if (path != _topPath) {
        path = path.left(path.lastIndexOf(QChar('/')));
    }

    path += "/";

    return path;
}
