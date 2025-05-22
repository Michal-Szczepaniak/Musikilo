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

#ifndef FILEMODEL_H
#define FILEMODEL_H

#include <QObject>
#include "playlistmodel.h"

#include <qwebdav.h>
#include <qwebdavdirparser.h>
#include <qwebdavitem.h>

class FileModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit FileModel(SettingsManager *settingsManager, QObject *parent = nullptr);

    enum FilesListRoles {
        Name = Qt::UserRole + 1,
        Path,
        IsDir
    };

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    Q_INVOKABLE void getFilesList(QString path);

signals:
    void errorOccured(QString error);
    void pluginChanged();

public slots:
    void onPluginChange();
    void onModelReset();

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    SettingsManager *_settingsManager;
    FileModelInterface *_fileModel = nullptr;
    QMetaObject::Connection _modelResetSigal;
    QMetaObject::Connection _errorOccuredSignal;
};

#endif // FILEMODEL_H
