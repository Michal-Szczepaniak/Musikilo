#include "subsonicfilemodel.h"

SubsonicFileModel::SubsonicFileModel(SubsonicManager *manager, QObject *parent) : FileModelInterface(parent), _manager(manager)
{
    connect(_manager, &SubsonicManager::gotFileList, this, &SubsonicFileModel::onGotFileList);
}

int SubsonicFileModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return _entries.size();
}

QVariant SubsonicFileModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount())
        return QVariant();

    const SubsonicManager::File &file = _entries[index.row()];
    if (role == Name)
        return file.name;
    else if (role == Path)
        return file.path;
    else if (role == isDir)
        return file.type == SubsonicManager::Folder;

    return QVariant();
}

void SubsonicFileModel::getFilesList(QString path)
{
    _manager->getFilesList(path);
}

QHash<int, QByteArray> SubsonicFileModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Name] = "name";
    roles[Path] = "path";
    roles[isDir] = "isDir";
    return roles;
}

void SubsonicFileModel::onGotFileList(QVector<SubsonicManager::File> files)
{
    beginResetModel();
    _entries.clear();

    SubsonicManager::File item;
    foreach(item, files) {
        _entries << item;
    }

    endResetModel();
}
