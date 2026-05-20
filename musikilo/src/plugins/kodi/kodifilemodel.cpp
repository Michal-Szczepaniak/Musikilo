#include "kodifilemodel.h"

KodiFileModel::KodiFileModel(KodiManager *manager, QObject *parent) : FileModelInterface(parent), _manager(manager)
{
    connect(_manager, &KodiManager::gotFileList, this, &KodiFileModel::onGotFileList);
    connect(_manager, &KodiManager::gotSources, this, &KodiFileModel::onGotSources);
}

int KodiFileModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return _entries.size();
}

QVariant KodiFileModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount())
        return QVariant();

    const KodiManager::File &file = _entries[index.row()];
    if (role == Name)
        return file.name;
    else if (role == Path)
        return file.path;
    else if (role == isDir)
        return file.type == KodiManager::Folder;

    return QVariant();
}

void KodiFileModel::getFilesList(QString path)
{
    _manager->getFilesList(path);
}

QHash<int, QByteArray> KodiFileModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Name] = "name";
    roles[Path] = "path";
    roles[isDir] = "isDir";
    return roles;
}

void KodiFileModel::onGotFileList(QVector<KodiManager::File> files)
{
    beginResetModel();
    _entries.clear();

    QString previousPath = "";
    if (!files.isEmpty()) {
        previousPath = getPreviousPath(files.first().path);
    }

    KodiManager::File previousDirectory = {
        .name = "..",
        .path = previousPath,
        .type = KodiManager::Folder,
    };

    _entries.append(previousDirectory);

    KodiManager::File item;
    foreach(item, files) {
        if (item.type == KodiManager::Folder) {
            _entries << item;
        }
    }

    foreach(item, files) {
        if (item.type == KodiManager::Track) {
            _entries << item;
        }
    }

    _entries.append(previousDirectory);

    endResetModel();
}

void KodiFileModel::onGotSources(QVector<KodiManager::File> sources)
{
    beginResetModel();
    _entries.clear();

    KodiManager::File item;
    foreach(item, sources) {
        if (item.type == KodiManager::Folder) {
            _entries << item;
        }
    }

    foreach(item, sources) {
        if (item.type == KodiManager::Track) {
            _entries << item;
        }
    }

    endResetModel();

    _sources = sources;
}

QString KodiFileModel::getPreviousPath(QString path)
{
    if (path.endsWith('/')) {
        path = path.left(path.length()-1);
    }

    path = path.left(path.lastIndexOf(QChar('/')));

    for (KodiManager::File source : _sources) {
        if (source.path == (path + "/")) {
            return "/";
        }
    }

    path = path.left(path.lastIndexOf(QChar('/')));

    path = path == "" ? "/" : path;

    return path;
}
