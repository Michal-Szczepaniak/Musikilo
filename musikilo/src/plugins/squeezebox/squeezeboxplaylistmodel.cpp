#include "squeezeboxplaylistmodel.h"

SqueezeBoxPlaylistModel::SqueezeBoxPlaylistModel(jcon::JsonRpcTcpClient *jcon, SqueezeBoxPlayer *player, QObject *parent) : PlaylistModelInterface(parent), _jcon(jcon), _player(player)
{

}

int SqueezeBoxPlaylistModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 0;
}

QVariant SqueezeBoxPlaylistModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount())
        return QVariant();

    return QVariant();
}

void SqueezeBoxPlaylistModel::reset()
{

}

void SqueezeBoxPlaylistModel::play(int index)
{

}

void SqueezeBoxPlaylistModel::addSong(QString song)
{

}

void SqueezeBoxPlaylistModel::playSong(QString song)
{

}

void SqueezeBoxPlaylistModel::nextSong()
{

}

void SqueezeBoxPlaylistModel::prevSong()
{

}

QHash<int, QByteArray> SqueezeBoxPlaylistModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Name] = "name";
    roles[Path] = "path";
    return roles;
}
