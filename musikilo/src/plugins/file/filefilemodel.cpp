/*
    Copyright (C) 2019 Michał Szczepaniak

    This file is part of Musikilo.

    Musikilo is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Musikilo is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Musikilo. If not, see <http://www.gnu.org/licenses/>.
*/

#include "filefilemodel.h"

#include <QDir>
#include <QDebug>
#include <QDirIterator>

FileFileModel::FileFileModel(QObject *parent) : FileModelInterface(parent)
{}

void FileFileModel::getFilesList(QString path)
{
    QDir dir(_path + path);
    qDebug() << "Path: " << path;

    QStringList nameFilters;
    nameFilters << "*.mp3"
            << "*.flac"
            << "*.ogg"
            << "*.wav"
            << "*.opus"
            << "*.m4a";

    QDir::Filters filters = QDir::Dirs | QDir::AllDirs | QDir::Files | QDir::NoSymLinks | QDir::NoDot;
    if (path == "/") {
        filters |= QDir::NoDotDot;
    }
    QFileInfoList files = dir.entryInfoList(nameFilters, filters);
    std::sort(files.begin(), files.end(),
        [](const QFileInfo &a, const QFileInfo &b)
        {
            if (a.isDir() != b.isDir())
                return a.isDir();

            return QString::localeAwareCompare(a.fileName(), b.fileName()) < 0;
        }
    );


    beginResetModel();
    _filesList.clear();

    QString previousPath = "";
    if (!files.isEmpty()) {
        previousPath = files.first().absoluteFilePath();
    }
    previousPath.replace(_path, "");
    previousPath = previousPath.isEmpty() ? previousPath + "/" : previousPath;

    File previousDirectory = {
        .name = "..",
        .path = previousPath,
        .type = Folder,
    };

    if (path != "/") {
        _filesList.append(previousDirectory);
        files.pop_front();
    }

    for (const QFileInfo &fileInfo : files) {
        if (fileInfo.isDir()) {
            _filesList.append({
                .name = fileInfo.fileName(),
                .path = fileInfo.absoluteFilePath().replace(_path, "") + "/",
                .type = Folder,
            });
        }
    }

    for (const QFileInfo &fileInfo : files) {
        if (!fileInfo.isDir()) {
            _filesList.append({
                .name = fileInfo.fileName(),
                .path = fileInfo.absoluteFilePath().replace(_path, ""),
                .type = Track,
                });
        }
    }

    if (path != "/") {
        _filesList.append(previousDirectory);
    }

    endResetModel();
}

QList<FileFileModel::File> FileFileModel::getFiles(QString path)
{
    QList<File> files;
    QStringList nameFilters;
    nameFilters << "*.mp3"
                << "*.flac"
                << "*.ogg"
                << "*.wav"
                << "*.opus"
                << "*.m4a";

    QDirIterator it(_path + path, nameFilters, QDir::Files, QDirIterator::Subdirectories);

    while (it.hasNext()) {
        it.next();
        qDebug() << "Add file: " << it.fileName() << " " << it.path();
        files.append({
            .name = it.fileName(),
            .path = it.fileInfo().absoluteFilePath(),
            .type = Track,
        });
    }

    std::sort(files.begin(), files.end(),
        [](const File &a, const File &b)
        {
            if ((a.type == FileType::Folder) != (b.type == FileType::Folder))
                return a.type == FileType::Folder;

            return QString::localeAwareCompare(a.name, b.name) < 0;
        }
    );

    return files;
}

FileFileModel::File FileFileModel::getFile(QString path)
{
    for (File file : _filesList) {
        if (file.path == path) {
            file.path = _path + path;
            return file;
        }
    }

    return {
        .name = "Idk man some bug, you should probably report this",
        .path = path,
        .type = Track,
    };
}


void FileFileModel::setPath(QString path)
{
    _path = path;

    getFilesList("/");
}

int FileFileModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return _filesList.size();
}

QVariant FileFileModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount())
        return QVariant();

    const File &file = _filesList[index.row()];
    if (role == Name)
        return file.name;
    else if (role == Path)
        return file.path;
    else if (role == isDir)
        return file.type == FileType::Folder;

    return QVariant();
}

QHash<int, QByteArray> FileFileModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Name] = "name";
    roles[Path] = "path";
    roles[isDir] = "isDir";
    return roles;
}

QString FileFileModel::getPreviousPath(QString path)
{
    if (path.endsWith('/')) {
        path = path.left(path.length()-1);
    }
    path = path.left(path.lastIndexOf(QChar('/')));

    path = path.left(path.lastIndexOf(QChar('/')));

    path += "/";

    return path;
}
