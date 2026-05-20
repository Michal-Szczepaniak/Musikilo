#include "tauonfilemodel.h"

TauonFileModel::TauonFileModel(TauonManager *manager, QObject *parent) : FileModelInterface(parent), _manager(manager)
{
    connect(_manager, &TauonManager::gotTracklist, this, &TauonFileModel::onGotTracklist);
}

int TauonFileModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return _entries.size();
}

QVariant TauonFileModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount())
        return QVariant();

    const TauonManager::File &file = _entries[index.row()];
    if (role == Name)
        return file.name;
    else if (role == Path)
        return file.path;
    else if (role == isDir)
        return file.type == TauonManager::Folder;

    return QVariant();
}

void TauonFileModel::getFilesList(QString path)
{
    QVector<TauonManager::File> folder = _manager->getFilesList(path);

    beginResetModel();
    _entries.clear();

    qDebug() << "Previous: " << getPreviousPath(folder.first().path);

    TauonManager::File previousDirectory = {
        .name = "..",
        .path = getPreviousPath(folder.first().path),
        .id = 0,
        .type = TauonManager::Folder,
    };

    _entries.append(previousDirectory);

    TauonManager::File item;
    foreach(item, folder) {
        if (item.type == TauonManager::Folder) {
            _entries << item;
        }
    }

    foreach(item, folder) {
        if (item.type == TauonManager::Track) {
            _entries << item;
        }
    }

    _entries.append(previousDirectory);

    endResetModel();
}

QHash<int, QByteArray> TauonFileModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Name] = "name";
    roles[Path] = "path";
    roles[isDir] = "isDir";
    return roles;
}

void TauonFileModel::onGotTracklist(QVector<TauonManager::Song> list)
{
    Q_UNUSED(list)

    getFilesList("/");
}

QString TauonFileModel::getPreviousPath(QString path)
{
    if (path.endsWith('/')) {
        path = path.left(path.length()-1);
    }

    path = path.left(path.lastIndexOf(QChar('/')));
    path = path.left(path.lastIndexOf(QChar('/')));

    path = path == "" ? "/" : path;

    return path;
}
