#ifndef MPDPLAYLISTMODEL_H
#define MPDPLAYLISTMODEL_H

#include "mpdplayer.h"

#include <QObject>

#include <src/playlistmodelinterface.h>

#include <mpd/networkaccess.h>

class MPDPlaylistModel : public PlaylistModelInterface
{
    Q_OBJECT
public:
    explicit MPDPlaylistModel(NetworkAccess *mpd, MPDPlayer *player, QObject *parent = nullptr);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    void reset();
    void play(int index);
    void addSong(QString song);
    void playSong(QString song);

signals:
    void mpdPlay(int index);
    void mpdClearPlaylist();
    void mpdPlaySong(QString path);
    void mpdAddSong(QString path);

public slots:
    void onPlaylistReady(QList<MpdTrack*> *playlist);
    void onCurrentIndexChanged(int index);

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    NetworkAccess *_mpd;
    MPDPlayer *_player;
    MPDPlaybackStatus *_mpdStatus;
    QList<MpdTrack*> _entries;
};

#endif // MPDPLAYLISTMODEL_H
