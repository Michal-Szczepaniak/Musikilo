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

#include "playlistmodel.h"

PlaylistModel::PlaylistModel(SettingsManager *settingsManager, Player *player, QObject *parent) : QAbstractListModel(parent), _settingsManager(settingsManager), _player(player)
{
    connect(settingsManager, &SettingsManager::currentPluginChanged, this, &PlaylistModel::onPluginChange);
}

int PlaylistModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    if (_playlistModel == nullptr) return 0;

    return _playlistModel->rowCount();
}

QVariant PlaylistModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount() || _playlistModel == nullptr) {
        return QVariant();
    }

    return _playlistModel->data(index, role);
}

int PlaylistModel::getCurrentIndex() const
{
    return _currentIndex;
}

void PlaylistModel::setCurrentIndex(int currentIndex)
{
    _currentIndex = currentIndex;

    emit currentIndexChanged(currentIndex);

    if (_playlistModel == nullptr) return;

    _playlistModel->play(currentIndex);
}

void PlaylistModel::reset()
{
    if (_playlistModel == nullptr) return;

    beginResetModel();
    _player->stop();
    _playlistModel->reset();
    setCurrentIndex(-1);
    endResetModel();
}

void PlaylistModel::addSong(QString path)
{
    if (_playlistModel == nullptr) return;

    _playlistModel->addSong(path);
}

void PlaylistModel::playSong(QString path)
{
    if (_playlistModel == nullptr) return;

    reset();

    _playlistModel->playSong(path);
}

void PlaylistModel::nextSong()
{
    if (_playlistModel == nullptr) return;

    _playlistModel->nextSong();
}

void PlaylistModel::prevSong()
{
    if (_playlistModel == nullptr) return;

    _playlistModel->prevSong();
}

void PlaylistModel::remove(int index)
{
    if (_playlistModel == nullptr) return;

    _playlistModel->remove(index);
}

void PlaylistModel::onPluginChange()
{
    auto plugin = _settingsManager->getCurrentPlugin();
    if (plugin != nullptr) {
        beginResetModel();
        if (_playlistModel != nullptr) {
            disconnect(_modelResetSigal);
            disconnect(_errorOccuredSignal);
            disconnect(_currentIndexChangedSignal);
        }

        _playlistModel = plugin->getPlaylistModel();
        _modelResetSigal = connect(_playlistModel, &PlaylistModelInterface::modelReset, this, &PlaylistModel::modelReset);
        _rowsInsertedSigal = connect(_playlistModel, &PlaylistModelInterface::rowsInserted, this, &PlaylistModel::rowsInserted);
        _rowsRemovedSigal = connect(_playlistModel, &PlaylistModelInterface::rowsRemoved, this, &PlaylistModel::rowsRemoved);
        _errorOccuredSignal = connect(_playlistModel, &PlaylistModelInterface::errorOccured, this, &PlaylistModel::errorOccured);
        _currentIndexChangedSignal = connect(_playlistModel, &PlaylistModelInterface::currentIndexChanged, this, &PlaylistModel::onCurrentIndexChanged);
        endResetModel();
    }
}

void PlaylistModel::onCurrentIndexChanged(int index)
{
    _currentIndex = index;

    emit currentIndexChanged(index);
}

QHash<int, QByteArray> PlaylistModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Name] = "name";
    roles[Path] = "path";
    return roles;
}
