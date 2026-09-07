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

#ifndef FILEPLAYLISTMODEL_H
#define FILEPLAYLISTMODEL_H

#include <QObject>

#include <src/playlistmodel.h>
#include <src/playlistmodelinterface.h>
#include "filefilemodel.h"
#include "fileplayer.h"

class FilePlaylistModel : public PlaylistModelInterface
{
    using File = FileFileModel::File;

    Q_OBJECT
public:

    explicit FilePlaylistModel(FilePlayer *player, FileFileModel *fileModel, QObject *parent = nullptr);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    void reset();
    void play(int index);
    void addSong(QString song);
    void playSong(QString song);
    void nextSong();
    void prevSong();
    void remove(int index);

protected:
    QHash<int, QByteArray> roleNames() const;

public slots:
    void onStateChanged();

private:
    FilePlayer *_player;
    FileFileModel *_fileModel;
    QList<File> _entries;
    int _lastIndex = -1;
};

#endif // FILEPLAYLISTMODEL_H
