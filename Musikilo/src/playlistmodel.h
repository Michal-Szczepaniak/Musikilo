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

    Q_INVOKABLE void setMediaPlayer(QObject* mediaPlayer);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<QWebdavItem> mEntries;
    QMediaPlayer* mMediaPlayer;
    int mActiveItem;

signals:
    void playFile(QString path);
    void activeItemChanged(int item);

public slots:
    void onRowsInserted(const QModelIndex &parent, int first, int last);
    void onStateChanged(QMediaPlayer::State state);

    void play(QMediaContent content);
    void addFile(QWebdavItem file);
};

#endif // PLAYLISTMODEL_H
