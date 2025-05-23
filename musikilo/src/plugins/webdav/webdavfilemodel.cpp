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

#include "webdavfilemodel.h"

WebDavFileModel::WebDavFileModel(QWebdav *webdav, QObject *parent) : FileModelInterface(parent), _webdav(webdav)
{
    connect(&_parser, &QWebdavDirParser::finished, this, &WebDavFileModel::gotFilesList);
    connect(&_parser, &QWebdavDirParser::finished, this, &WebDavFileModel::addFilesToList);
    connect(&_parser, &QWebdavDirParser::errorChanged, this, &WebDavFileModel::errorOccured);
    connect(_webdav, &QWebdav::errorChanged, this, &WebDavFileModel::errorOccured);
}

void WebDavFileModel::getFilesList(QString path)
{
    _parser.listDirectory(_webdav, path, 1);
}

int WebDavFileModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return _filesList.size();
}

QVariant WebDavFileModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount())
        return QVariant();

    const QWebdavItem &file = _filesList[index.row()];
    if (role == Name)
        return file.name();
    else if (role == Path)
        return file.path();
    else if (role == isDir)
        return file.isDir();

    return QVariant();
}

QHash<int, QByteArray> WebDavFileModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Name] = "name";
    roles[Path] = "path";
    roles[isDir] = "isDir";
    return roles;
}

QString WebDavFileModel::getPreviousPath(QWebdavItem item)
{
    QString path  = item.path();
    if (path.endsWith('/')) {
        path = path.left(path.length()-1);
    }
    path = path.left(path.lastIndexOf(QChar('/')));

    path = path.left(path.lastIndexOf(QChar('/')));

    path += "/";

    return path;
}

void WebDavFileModel::addFilesToList()
{
    QList<QWebdavItem> list = _parser.getList();
    if(list.size() == 0) return;

    _filesList.clear();

    beginResetModel();

    QString previousPath = getPreviousPath(list.first());
    QDateTime date;
    QWebdavItem backItem(previousPath, "..", "", true, date, 0);
    _filesList<<backItem;

    QWebdavItem item;
    foreach(item, list) {
        if (item.isDir()) {
            _filesList << item;
        }
    }

    foreach(item, list) {
        if (item.mimeType().startsWith("audio")) {
            _filesList << item;
        }
    }

    _filesList<<backItem;

    endResetModel();
}

void WebDavFileModel::replySkipRead()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());
    if (reply==0)
        return;

    QByteArray ba = reply->readAll();
}

void WebDavFileModel::getMediaContent(QString path)
{
    QNetworkRequest r = _webdav->getRequest(path);
    r.setRawHeader("Authorization", "Basic " + QString(_webdav->username() + ":" + _webdav->password()).toUtf8().toBase64());
    emit gotMediaContent(QMediaContent(r));
}
