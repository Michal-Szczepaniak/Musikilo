#ifndef KODIPLAYLISTMODEL_H
#define KODIPLAYLISTMODEL_H

#include "kodiplayer.h"

#include <QObject>

#include <src/playlistmodelinterface.h>

class KodiPlaylistModel : public PlaylistModelInterface
{
    using PlaylistItem = KodiManager::PlaylistItem;
    using Status = KodiManager::Status;

    Q_OBJECT
public:
    explicit KodiPlaylistModel(KodiManager *manager, KodiPlayer *player, QObject *parent = nullptr);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    void reset();
    void play(int index);
    void addSong(QString song);
    void playSong(QString song);
    void nextSong();
    void prevSong();
    void remove(int index);

protected:
    QHash<int, QByteArray> roleNames() const;

signals:

public slots:
    void onGotStatus(Status status);
    void onGotPlaylist(QVector<PlaylistItem> playlist);

private:
    KodiManager *_manager;
    KodiPlayer *_player;
    QVector<PlaylistItem> _playlist{};
};

#endif // KODIPLAYLISTMODEL_H
