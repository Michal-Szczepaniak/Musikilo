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

#include "subsonicplaylistmodel.h"

SubsonicPlaylistModel::SubsonicPlaylistModel(SubsonicManager *manager, SubsonicPlayer *player, QObject *parent) : PlaylistModelInterface(parent), _manager(manager), _player(player)
{
    connect(_player, &SubsonicPlayer::stateChanged, this, &SubsonicPlaylistModel::onStateChanged);
    connect(_manager, &SubsonicManager::gotTrackList, this, &SubsonicPlaylistModel::onGotTrackList);
}

void SubsonicPlaylistModel::reset()
{
    beginResetModel();
    _entries.clear();
    endResetModel();
}

void SubsonicPlaylistModel::play(int index)
{
    if (index < 0 || index >= rowCount()) return;

    QString id = _entries.at(index).id;

    _player->play(id);

    _lastIndex = index;

    emit currentIndexChanged(index);
}

void SubsonicPlaylistModel::addSong(QString song)
{
    _manager->listDirectory(song);
}

void SubsonicPlaylistModel::playSong(QString song)
{
    beginResetModel();
    _entries.clear();
    endResetModel();
    _manager->listDirectory(song);
}

void SubsonicPlaylistModel::nextSong()
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

void SubsonicPlaylistModel::prevSong()
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

void SubsonicPlaylistModel::remove(int index)
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

int SubsonicPlaylistModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _entries.length();
}

QVariant SubsonicPlaylistModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount())
        return QVariant();

    const PlaylistItem &item = _entries[index.row()];
    if (role == Name) {
        QString title = !item.artist.isEmpty() ? item.title + " - " + item.artist : item.title;

        return (item.track > 0 ? (item.track.rightJustified(2, '0') + ". ") : "") + title;
    } else if (role == Path) {
        return item.id;
    }

    return QVariant();
}

QHash<int, QByteArray> SubsonicPlaylistModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Name] = "name";
    roles[Path] = "path";
    return roles;
}

void SubsonicPlaylistModel::onGotTrackList(QList<PlaylistItem> list)
{
    bool play = rowCount() == 0;

    beginInsertRows(QModelIndex(), rowCount(), rowCount()+list.count()-1);
    _entries.append(list);
    endInsertRows();

    if (play && rowCount() > 0) {
        _lastIndex = 0;
        emit currentIndexChanged(0);

        _player->play(_entries.first().id);
    }
}

void SubsonicPlaylistModel::onStateChanged()
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
