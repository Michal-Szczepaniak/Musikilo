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
