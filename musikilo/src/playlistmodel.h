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

#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QObject>
#include <QMediaPlayer>

#include <qwebdav.h>
#include <qwebdavitem.h>

class PlaylistModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit PlaylistModel(QObject *parent = nullptr);

    Q_PROPERTY(int activeItem READ activeItem WRITE setActiveItem NOTIFY activeItemChanged)

    enum EntriesRoles {
        Name = Qt::UserRole + 1,
        Path
    };

    Q_INVOKABLE void reset();

    int activeItem();
    void setActiveItem(int activeItem);

    Q_INVOKABLE void resume();
    Q_INVOKABLE void pause();

    Q_INVOKABLE void remove(int position);

    Q_INVOKABLE void setMediaPlayer(QObject* mediaPlayer);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<QWebdavItem> mEntries;
    QMediaPlayer* mMediaPlayer;
    int mActiveItem = 0;

signals:
    void playFile(QString path);
    void activeItemChanged(int activeItem);

public slots:
    void onRowsInserted();

    void play(QMediaContent content);
    void addFile(QWebdavItem file);
};

#endif // PLAYLISTMODEL_H
