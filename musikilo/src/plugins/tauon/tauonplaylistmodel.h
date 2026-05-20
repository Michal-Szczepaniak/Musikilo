#ifndef TAUONPLAYLISTMODEL_H
#define TAUONPLAYLISTMODEL_H

#include "tauonplayer.h"

#include <QObject>

#include <src/playlistmodelinterface.h>

class TauonPlaylistModel : public PlaylistModelInterface
{
    using Song = TauonManager::Song;
    using Status = TauonManager::Status;

    Q_OBJECT
public:
    explicit TauonPlaylistModel(TauonManager *manager, TauonPlayer *player, QObject *parent = nullptr);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    void reset() {};
    void play(int index);
    void addSong(QString song);
    void playSong(QString song);
    void nextSong();
    void prevSong();
    void remove(int) {};

protected:
    QHash<int, QByteArray> roleNames() const;

signals:

public slots:
    void onGotStatus(Status status);
    void onGotPlaylist(QVector<Song> playlist);

private:
    TauonManager *_manager;
    TauonPlayer *_player;
    QVector<Song> _playlist{};
};

#endif // TAUONPLAYLISTMODEL_H
