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
    Q_INVOKABLE void add(QString path);

    void addFile(QString path, bool reset);

    void setPlaylistModel(PlaylistModel* playlistModel);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<QWebdavItem> mFilesList;
    PlaylistModel* playlistModel;

    QWebdav w;
    QWebdavDirParser p;
    QWebdavDirParser playlistParser;

    QString getPreviousPath(QWebdavItem item);

signals:
    void gotFilesList();
    void printError(QString errorMsg);

public slots:
    void addFilesToList();
    void addFilesToPlaylist();
    void replySkipRead();
    void playFile(QString path);
};

#endif // WEBDAVMODEL_H
