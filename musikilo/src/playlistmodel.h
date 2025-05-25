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
#include <src/playlistmodelinterface.h>
#include <src/settingsmanager.h>
#include <src/player.h>

class PlaylistModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int currentIndex READ getCurrentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
public:
    enum PlaylistRoles {
        Name = Qt::UserRole + 1,
        Path
    };

    explicit PlaylistModel(SettingsManager *settingsManager, Player *player, QObject *parent = nullptr);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    int getCurrentIndex() const;
    void setCurrentIndex(int currentIndex);

    Q_INVOKABLE void addSong(QString path);
    Q_INVOKABLE void playSong(QString path);
    Q_INVOKABLE void nextSong();
    Q_INVOKABLE void prevSong();

protected:
    QHash<int, QByteArray> roleNames() const;

signals:
    void errorOccured(QString error);
    void currentIndexChanged(int currentIndex);

public slots:
    void onPluginChange();
    void onCurrentIndexChanged(int index);

private:
    SettingsManager *_settingsManager;
    PlaylistModelInterface *_playlistModel = nullptr;
    Player *_player = nullptr;
    QMetaObject::Connection _modelResetSigal;
    QMetaObject::Connection _rowsInsertedSigal;
    QMetaObject::Connection _rowsRemovedSigal;
    QMetaObject::Connection _errorOccuredSignal;
    QMetaObject::Connection _currentIndexChangedSignal;
    int _currentIndex = -1;

    void reset();
};

#endif // PLAYLISTMODEL_H
