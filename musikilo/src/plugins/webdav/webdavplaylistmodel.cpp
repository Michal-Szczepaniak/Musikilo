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

WebDavPlaylistModel::WebDavPlaylistModel(QWebdav *webdav, WebDavPlayer *player, QObject *parent) : _webdav(webdav), _player(player)
{
    connect(&_parser, &QWebdavDirParser::finished, this, &WebDavPlaylistModel::addFilesToPlaylist);
    connect(&_parser, &QWebdavDirParser::errorChanged, this, &WebDavPlaylistModel::errorOccured);
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
}

void WebDavPlaylistModel::addSong(QString song)
{
    bool isDir = song.endsWith('/');

    qDebug() << "file " << song << " isdir " << isDir;

    if(isDir) {
        _parser.listDirectory(_webdav, song, 100);
    } else {
        _parser.listItem(_webdav, song);
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

Qt::ItemFlags WebDavPlaylistModel::flags(const QModelIndex &index) const {
    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
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
