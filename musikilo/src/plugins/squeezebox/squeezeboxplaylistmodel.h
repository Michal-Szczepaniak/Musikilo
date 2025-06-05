#ifndef SQUEEZEBOXPLAYLISTMODEL_H
#define SQUEEZEBOXPLAYLISTMODEL_H

#include "squeezeboxplayer.h"

#include <QObject>

#include <src/playlistmodelinterface.h>

using Song = SqueezeBoxManager::Song;

class SqueezeBoxPlaylistModel : public PlaylistModelInterface
{
    Q_OBJECT
public:
    explicit SqueezeBoxPlaylistModel(SqueezeBoxManager *manager, SqueezeBoxPlayer *player, QObject *parent = nullptr);

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
    void onGotPlaylist(QVector<Song> playlist);

private:
    SqueezeBoxManager *_manager;
    SqueezeBoxPlayer *_player;
    QVector<Song> _playlist{};
};

#endif // SQUEEZEBOXPLAYLISTMODEL_H
