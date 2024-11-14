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

#ifndef WEBDAVMODEL_H
#define WEBDAVMODEL_H

#include <QObject>
#include "playlistmodel.h"

#include <qwebdav.h>
#include <qwebdavdirparser.h>
#include <qwebdavitem.h>

class WebDavModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit WebDavModel(QObject *parent = nullptr);

    enum FilesListRoles {
        Name = Qt::UserRole + 1,
        Path,
        isDir
    };

    Q_INVOKABLE void getFilesList(QString path);

    Q_INVOKABLE void play(QString path);

    Q_INVOKABLE void setConnectionSettings( const int connectionType,
                                                 const QString &hostname,
                                                 const QString &rootPath = "/",
                                                 const QString &username = "",
                                                 const QString &password = "",
                                                 int port = 0);

    void addFile(QString path);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<QWebdavItem> mFilesList;

    QWebdav w;
    QWebdavDirParser p;
    QWebdavDirParser playlistParser;

    QString getPreviousPath(QWebdavItem item);

signals:
    void gotFilesList();
    void gotAudioFile(QWebdavItem item);
    void printError(QString errorMsg);
    void gotMediaContent(QMediaContent content);

public slots:
    void addFilesToList();
    void addFilesToPlaylist();
    void replySkipRead();
    void getMediaContent(QString path);
};

#endif // WEBDAVMODEL_H
