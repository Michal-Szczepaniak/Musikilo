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

#ifndef WEBDAVFILEMODEL_H
#define WEBDAVFILEMODEL_H

#include <QObject>

#include <QMediaContent>
#include <qwebdav.h>
#include <qwebdavdirparser.h>
#include <qwebdavitem.h>
#include <src/filemodelinterface.h>

class WebDavFileModel : public FileModelInterface
{
    Q_OBJECT
public:
    explicit WebDavFileModel(QWebdav *webdav, QObject *parent = nullptr);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    void getFilesList(QString path);

protected:
    QHash<int, QByteArray> roleNames() const;

signals:
    void gotFilesList();
    void gotAudioFile(QWebdavItem item);
    void gotMediaContent(QMediaContent content);

public slots:
    void addFilesToList();
    void addFilesToPlaylist();
    void replySkipRead();
    void getMediaContent(QString path);

private:
    QList<QWebdavItem> _filesList;

    QWebdav* _webdav;
    QWebdavDirParser _parser;

    QString getPreviousPath(QWebdavItem item);
};

#endif // WEBDAVFILEMODEL_H
