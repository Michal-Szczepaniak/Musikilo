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

#include "fileplaylistmodel.h"

FilePlaylistModel::FilePlaylistModel(FilePlayer *player, FileFileModel *fileModel, QObject *parent) : PlaylistModelInterface(parent), _player(player), _fileModel(fileModel)
{
    connect(_player, &FilePlayer::stateChanged, this, &FilePlaylistModel::onStateChanged);
}

void FilePlaylistModel::reset()
{
    beginResetModel();
    _entries.clear();
    endResetModel();
}

void FilePlaylistModel::play(int index)
{
    if (index < 0 || index >= rowCount()) return;

    QString path = _entries.at(index).path;

    _player->play(path);

    _lastIndex = index;

    emit currentIndexChanged(index);
}

void FilePlaylistModel::addSong(QString song)
{
    bool isDir = song.endsWith('/');
    qDebug() << "Add path: " << song;

    if(isDir) {
        auto files = _fileModel->getFiles(song);

        beginInsertRows(QModelIndex(), rowCount(), rowCount()+files.count()-1);
        _entries.append(files);
        endInsertRows();
    } else {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        _entries.append(_fileModel->getFile(song));
        endInsertRows();
    }
}

void FilePlaylistModel::playSong(QString song)
{
    bool isDir = song.endsWith('/');
    qDebug() << "Play path: " << song;

    _entries.clear();
    if(isDir) {
        auto files = _fileModel->getFiles(song);

        beginResetModel();
        _entries.append(files);
        endResetModel();
    } else {
        beginResetModel();
        _entries.append(_fileModel->getFile(song));
        endResetModel();
    }

    _lastIndex = 0;
    emit currentIndexChanged(0);

    _player->play(_entries.first().path);
}

void FilePlaylistModel::nextSong()
{
    if (_player->getConsume()) {
        beginRemoveRows(QModelIndex(), _lastIndex, _lastIndex);
        _entries.removeAt(_lastIndex);
        endRemoveRows();
    }

    if (_player->getShuffle()) {
        play(qrand() % rowCount());
    } else if (_player->getConsume() && _lastIndex < rowCount()) {
        play(_lastIndex);
    } else if (_lastIndex+1 < rowCount()) {
        play(_lastIndex+1);
    } else if (_player->getRepeat()) {
        play(0);
    }
}

void FilePlaylistModel::prevSong()
{
    if (_player->getConsume()) {
        beginRemoveRows(QModelIndex(), _lastIndex, _lastIndex);
        _entries.removeAt(_lastIndex);
        endRemoveRows();
    }

    if (_player->getShuffle()) {
        play(qrand() % rowCount());
    } else if (_lastIndex - 1 >= 0) {
        play(_lastIndex - 1);
    }
}

void FilePlaylistModel::remove(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    _entries.removeAt(index);
    endRemoveRows();

    if (_lastIndex == index) {
        if (_lastIndex < rowCount()) {
            play(_lastIndex);
        } else {
            _player->stop();
        }
    } else if (_lastIndex > index) {
        _lastIndex--;

        emit currentIndexChanged(_lastIndex);
    }
}

int FilePlaylistModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _entries.length();
}

QVariant FilePlaylistModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount())
        return QVariant();

    const File &file = _entries[index.row()];
    if (role == Name)
        return file.name;
    else if (role == Path)
        return file.path;

    return QVariant();
}

QHash<int, QByteArray> FilePlaylistModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Name] = "name";
    roles[Path] = "path";
    return roles;
}

void FilePlaylistModel::onStateChanged()
{
    if ((_player->getState() == QMediaPlayer::StoppedState) && (_player->getPosition() >= _player->getDuration()-10) && (_player->getDuration() > 0)) {
        if (_player->getConsume()) {
            beginRemoveRows(QModelIndex(), _lastIndex, _lastIndex);
            _entries.removeAt(_lastIndex);
            endRemoveRows();
        } else if (_player->getSingle()) {
            _player->play();
            return;
        }

        if (_player->getShuffle()) {
            play(qrand() % rowCount());
        } else if (_player->getConsume() && _lastIndex < rowCount()) {
            play(_lastIndex);
        } else if (_lastIndex+1 < rowCount()) {
            play(_lastIndex+1);
        } else if (_player->getRepeat()) {
            play(0);
        }
    }
}
