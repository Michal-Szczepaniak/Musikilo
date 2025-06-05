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

#include "webdavplaylistmodel.h"

WebDavPlaylistModel::WebDavPlaylistModel(QWebdav *webdav, WebDavPlayer *player, QObject *parent) : PlaylistModelInterface(parent), _webdav(webdav), _player(player)
{
    connect(&_parser, &QWebdavDirParser::finished, this, &WebDavPlaylistModel::addFilesToPlaylist);
    connect(&_parser, &QWebdavDirParser::errorChanged, this, &WebDavPlaylistModel::errorOccured);
    connect(&_playParser, &QWebdavDirParser::finished, this, &WebDavPlaylistModel::playFiles);
    connect(&_playParser, &QWebdavDirParser::errorChanged, this, &WebDavPlaylistModel::errorOccured);
    connect(_player, &WebDavPlayer::stateChanged, this, &WebDavPlaylistModel::onStateChanged);
}

void WebDavPlaylistModel::reset()
{
    beginResetModel();
    _entries.clear();
    endResetModel();
}

void WebDavPlaylistModel::play(int index)
{
    if (index < 0 || index >= rowCount()) return;

    QString path = _entries.at(index).path();

    _player->play(path);

    _lastIndex = index;

    emit currentIndexChanged(index);
}

void WebDavPlaylistModel::addSong(QString song)
{
    bool isDir = song.endsWith('/');

    if(isDir) {
        _parser.listDirectory(_webdav, song, 100);
    } else {
        _parser.listItem(_webdav, song);
    }
}

void WebDavPlaylistModel::playSong(QString song)
{
    bool isDir = song.endsWith('/');

    if(isDir) {
        _playParser.listDirectory(_webdav, song, 100);
    } else {
        _playParser.listItem(_webdav, song);
    }
}

void WebDavPlaylistModel::nextSong()
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

void WebDavPlaylistModel::prevSong()
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

void WebDavPlaylistModel::remove(int index)
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

int WebDavPlaylistModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _entries.length();
}

QVariant WebDavPlaylistModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount())
        return QVariant();

    const QWebdavItem &file = _entries[index.row()];
    if (role == Name)
        return file.name();
    else if (role == Path)
        return file.path();

    return QVariant();
}

QHash<int, QByteArray> WebDavPlaylistModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Name] = "name";
    roles[Path] = "path";
    return roles;
}

void WebDavPlaylistModel::addFilesToPlaylist()
{
    QList<QWebdavItem> list = _parser.getList();
    QList<QWebdavItem> playlistFiles{};

    QWebdavItem item;
    foreach(item, list) {
        if (item.mimeType().startsWith("audio")) {
            playlistFiles << item;
        }
    }

    beginInsertRows(QModelIndex(), rowCount(), rowCount()+playlistFiles.count()-1);
    _entries.append(playlistFiles);
    endInsertRows();

}

void WebDavPlaylistModel::playFiles()
{
    QList<QWebdavItem> list = _playParser.getList();
    QList<QWebdavItem> playlistFiles{};

    QWebdavItem item;
    foreach(item, list) {
        if (item.mimeType().startsWith("audio")) {
            playlistFiles << item;
        }
    }

    beginInsertRows(QModelIndex(), rowCount(), rowCount()+playlistFiles.count()-1);
    _entries.append(playlistFiles);
    endInsertRows();

    _lastIndex = 0;
    emit currentIndexChanged(0);

    _player->play(_entries.first().path());
}

void WebDavPlaylistModel::onStateChanged()
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
