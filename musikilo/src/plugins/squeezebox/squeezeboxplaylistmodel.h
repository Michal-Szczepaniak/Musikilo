#ifndef SQUEEZEBOXPLAYLISTMODEL_H
#define SQUEEZEBOXPLAYLISTMODEL_H

#include "squeezeboxplayer.h"

#include <QObject>

#include <jcon/json_rpc_tcp_client.h>

#include <src/playlistmodelinterface.h>

class SqueezeBoxPlaylistModel : public PlaylistModelInterface
{
    Q_OBJECT
public:
    explicit SqueezeBoxPlaylistModel(jcon::JsonRpcTcpClient *jcon, SqueezeBoxPlayer *player, QObject *parent = nullptr);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    void reset();
    void play(int index);
    void addSong(QString song);
    void playSong(QString song);
    void nextSong();
    void prevSong();

protected:
    QHash<int, QByteArray> roleNames() const;

signals:

private:
    jcon::JsonRpcTcpClient *_jcon;
    SqueezeBoxPlayer *_player;
};

#endif // SQUEEZEBOXPLAYLISTMODEL_H
