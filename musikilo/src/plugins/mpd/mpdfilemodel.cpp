#include "mpdfilemodel.h"

MPDFileModel::MPDFileModel(NetworkAccess *mpd, QObject *parent) : FileModelInterface(parent), _mpd(mpd)
{
    connect(_mpd, &NetworkAccess::filesReady, this, &MPDFileModel::onFilesReady);
    connect(this, &MPDFileModel::getMPDFilesList, _mpd, &NetworkAccess::getDirectory);
}

int MPDFileModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _filesList.length();
}

QVariant MPDFileModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount())
        return QVariant();

    const MpdFileEntry *file = _filesList[index.row()];
    if (role == Name) {
        return file->getName();
    } else if (role == Path) {
        if (file->getName() == "..") {
            return file->getPrePath();
        } else {
            return (file->getPrePath() == "/" ? "" : file->getPrePath() + "/") + file->getName();
        }
    } else if (role == isDir) {
        return file->isDirectory();
    }

    return QVariant();
}

void MPDFileModel::getFilesList(QString path)
{
    emit getMPDFilesList(path);
}

QHash<int, QByteArray> MPDFileModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Name] = "name";
    roles[Path] = "path";
    roles[isDir] = "isDir";
    return roles;
}

void MPDFileModel::onFilesReady(QList<QObject *> *filesList)
{
    if (filesList->length() == 0) return;

    auto files = (QList<MpdFileEntry*>*)filesList;

    MpdFileEntry *file;
    foreach (file, _filesList) {
        delete file;
    }

    _filesList.clear();

    beginResetModel();

    QString previousPath = getPreviousPath(files->first());
    MpdFileEntry *backDirectoryStart = new MpdFileEntry(previousPath, "..", MpdFileEntry::MpdFileType_Directory);
    MpdFileEntry *backDirectoryEnd = new MpdFileEntry(previousPath, "..", MpdFileEntry::MpdFileType_Directory);

    _filesList.append(backDirectoryStart);

    foreach (file, *files) {
        if (file->isDirectory()) {
            _filesList.append(file);
        }
    }

    foreach (file, *files) {
        if (file->isFile()) {
            _filesList.append(file);
        }
    }

    _filesList.append(backDirectoryEnd);

    endResetModel();
    delete filesList;
}

QString MPDFileModel::getPreviousPath(MpdFileEntry *file)
{

    QString path = file->getPrePath();

    int lastIndex = path.lastIndexOf(QChar('/'));

    if (lastIndex <= 0) return "/";

    path = path.left(lastIndex);

    return path;
}
