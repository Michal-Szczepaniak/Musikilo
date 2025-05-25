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

#ifndef WEBDAVPLAYLISTMODEL_H
#define WEBDAVPLAYLISTMODEL_H

#include "webdavplayer.h"

#include <QObject>

#include <qwebdav.h>
#include <qwebdavdirparser.h>
#include <qwebdavitem.h>

#include <src/playlistmodel.h>
#include <src/playlistmodelinterface.h>

class WebDavPlaylistModel : public PlaylistModelInterface
{
    Q_OBJECT
public:

    explicit WebDavPlaylistModel(QWebdav *webdav, WebDavPlayer *player, QObject *parent = nullptr);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    void reset();
    void play(int index);
    void addSong(QString song);
    void playSong(QString song);
    void nextSong();
    void prevSong();

protected:
    QHash<int, QByteArray> roleNames() const;

public slots:
    void addFilesToPlaylist();
    void playFiles();
    void onStateChanged();

private:
    QWebdav* _webdav;
    WebDavPlayer *_player;
    QList<QWebdavItem> _entries;
    QWebdavDirParser _parser, _playParser;
    int _lastIndex = -1;
};

#endif // WEBDAVPLAYLISTMODEL_H
