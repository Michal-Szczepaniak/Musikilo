#include "webdavmodel.h"

WebDavModel::WebDavModel(QObject *parent) : QAbstractListModel(parent)
{
    connect(&p, &QWebdavDirParser::finished, this, &WebDavModel::gotFilesList);
    connect(&p, &QWebdavDirParser::finished, this, &WebDavModel::addFilesToList);
    connect(&p, &QWebdavDirParser::errorChanged, this, &WebDavModel::printError);
    connect(&w, &QWebdav::errorChanged, this, &WebDavModel::printError);

    connect(&playlistParser, &QWebdavDirParser::finished, this, &WebDavModel::addFilesToPlaylist);
    connect(&playlistParser, &QWebdavDirParser::errorChanged, this, &WebDavModel::printError);

}

void WebDavModel::getFilesList(QString path)
{
    p.listDirectory(&w, path);
}

void WebDavModel::play(QString path)
{
    addFile(path);
}

void WebDavModel::addFile(QString path)
{
    bool isDir = path.endsWith('/');

    if(isDir) {
        playlistParser.listDirectory(&w, path);
    } else {
        playlistParser.listItem(&w, path);
    }
}

int WebDavModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return mFilesList.size();
}

QVariant WebDavModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount())
        return QVariant();

    const QWebdavItem &file = mFilesList[index.row()];
    if (role == Name)
        return file.name();
    else if (role == Path)
        return file.path();
    else if (role == isDir)
        return file.isDir();

    return QVariant();
}

Qt::ItemFlags WebDavModel::flags(const QModelIndex &index) const {
    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

QHash<int, QByteArray> WebDavModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Name] = "name";
    roles[Path] = "path";
    roles[isDir] = "isDir";
    return roles;
}

QString WebDavModel::getPreviousPath(QWebdavItem item) // Why is getting parent path so complicated
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

void WebDavModel::addFilesToList()
{
    QList<QWebdavItem> list = p.getList();
    if(list.size() == 0) return;

    mFilesList.clear();

    beginResetModel();

    QString previousPath = this->getPreviousPath(list.first());
    QDateTime date;
    QWebdavItem backItem(previousPath, "..", "", true, date, 0);
    mFilesList<<backItem;

    QWebdavItem item;
    foreach(item, list) {
        qDebug() << item.name();
        mFilesList << item;
    }
    mFilesList<<backItem;

    endResetModel();
}

void WebDavModel::addFilesToPlaylist()
{
    QList<QWebdavItem> list = p.getList();
    if(list.size() == 0) return;

    QWebdavItem item;
    foreach(item, list) {
        qDebug()<<item.mimeType();
        if (item.mimeType().startsWith("audio")) {
            emit gotAudioFile(item);
        }
    }
}

void WebDavModel::replySkipRead()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());
    if (reply==0)
        return;

    QByteArray ba = reply->readAll();
}

void WebDavModel::getMediaContent(QString path)
{
    QNetworkRequest r = w.getRequest(path);
    r.setRawHeader("Authorization", "Basic " + QString(w.username() + ":" + w.password()).toUtf8().toBase64()); // Headers aren't set by lib
    emit gotMediaContent(QMediaContent(r));
}
