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

PlaylistModel::PlaylistModel(QObject *parent) : QAbstractListModel(parent)
{
    connect(this, &PlaylistModel::rowsInserted, this, &PlaylistModel::onRowsInserted);
}

void PlaylistModel::reset()
{
    mMediaPlayer->stop();
    mMediaPlayer->setMedia(nullptr);
    beginResetModel();
    mEntries.clear();
    endResetModel();
    setActiveItem(0);
}

void PlaylistModel::addFile(QWebdavItem file)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    mEntries << file;
    qDebug() << "PlaylistModel " << file.name();
    endInsertRows();
}

void PlaylistModel::play(QMediaContent content)
{
    mMediaPlayer->setMedia(content);
    mMediaPlayer->play();
}

int PlaylistModel::activeItem()
{
    return mActiveItem;
}

void PlaylistModel::setActiveItem(int activeItem)
{
    mActiveItem = activeItem;

    if (rowCount() > 0 && activeItem < rowCount() && activeItem >= 0) {
        mMediaPlayer->stop();

        emit playFile(mEntries[mActiveItem].path());
    }

    emit activeItemChanged(mActiveItem);
}

void PlaylistModel::resume()
{
    mMediaPlayer->play();
}

void PlaylistModel::pause()
{
    mMediaPlayer->pause();
}

void PlaylistModel::remove(int position)
{
    if (position == mActiveItem)
        mMediaPlayer->stop();
    setActiveItem(-1);
    beginRemoveRows(QModelIndex(), position, position);
    mEntries.removeAt(position);
    endRemoveRows();

    if (rowCount() == 0)
        mMediaPlayer->setMedia(nullptr);
}

void PlaylistModel::setMediaPlayer(QObject *mediaPlayer)
{
    mMediaPlayer = qvariant_cast<QMediaPlayer*>(mediaPlayer->property("mediaObject"));
}

int PlaylistModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mEntries.length();
}

QVariant PlaylistModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount())
        return QVariant();

    const QWebdavItem &file = mEntries[index.row()];
    if (role == Name)
        return file.name();
    else if (role == Path)
        return file.path();

    return QVariant();
}

Qt::ItemFlags PlaylistModel::flags(const QModelIndex &index) const {
    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

QHash<int, QByteArray> PlaylistModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Name] = "name";
    roles[Path] = "path";
    return roles;
}

void PlaylistModel::onRowsInserted()
{
    if (mMediaPlayer->state() == QMediaPlayer::StoppedState && rowCount() > 0) {
        emit playFile(mEntries[0].path());
    }
}
