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

#include "httpplaylistmodel.h"

HttpPlaylistModel::HttpPlaylistModel(HttpPlayer *player, QObject *parent) : PlaylistModelInterface(parent), _player(player)
{}

void HttpPlaylistModel::reset()
{
}

void HttpPlaylistModel::play(int index)
{
    Q_UNUSED(index)

    _player->play();
}

void HttpPlaylistModel::addSong(QString song)
{
    Q_UNUSED(song)
}

void HttpPlaylistModel::playSong(QString song)
{
    Q_UNUSED(song)
}

void HttpPlaylistModel::nextSong()
{
    _player->play();
}

void HttpPlaylistModel::prevSong()
{
    _player->play();
}

void HttpPlaylistModel::remove(int index)
{
    Q_UNUSED(index)
}

int HttpPlaylistModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant HttpPlaylistModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(index)

    if (role == Name)
        return tr("Internet Radio Stream");

    return QVariant();
}

QHash<int, QByteArray> HttpPlaylistModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Name] = "name";
    roles[Path] = "path";
    return roles;
}
