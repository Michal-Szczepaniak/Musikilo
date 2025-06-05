#include "squeezeboxplaylistmodel.h"

SqueezeBoxPlaylistModel::SqueezeBoxPlaylistModel(SqueezeBoxManager *manager, SqueezeBoxPlayer *player, QObject *parent) : PlaylistModelInterface(parent), _manager(manager), _player(player)
{
    connect(manager, &SqueezeBoxManager::gotStatus, this, &SqueezeBoxPlaylistModel::onGotStatus);
    connect(manager, &SqueezeBoxManager::gotPlaylist, this, &SqueezeBoxPlaylistModel::onGotPlaylist);
}

int SqueezeBoxPlaylistModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return _playlist.size();
}

QVariant SqueezeBoxPlaylistModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount())
        return QVariant();

    const Song &song = _playlist[index.row()];
    if (role == Name)
        return (song.trackNr > 0 ? (QString::number(song.trackNr).rightJustified(2, '0') + ". ") : "") + song.title + " - " + song.artist;
    else if (role == Path)
        return song.url;

    return QVariant();
}

void SqueezeBoxPlaylistModel::reset()
{
    _manager->clear();
}

void SqueezeBoxPlaylistModel::play(int index)
{
    _manager->play(index);
}

void SqueezeBoxPlaylistModel::addSong(QString song)
{
    _manager->addFile(song);
}

void SqueezeBoxPlaylistModel::playSong(QString song)
{
    _manager->playFile(song);
}

void SqueezeBoxPlaylistModel::nextSong()
{
    _manager->nextSong();
}

void SqueezeBoxPlaylistModel::prevSong()
{
    _manager->prevSong();
}

void SqueezeBoxPlaylistModel::remove(int index)
{
    _manager->remove(index);
}

QHash<int, QByteArray> SqueezeBoxPlaylistModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Name] = "name";
    roles[Path] = "path";
    return roles;
}

void SqueezeBoxPlaylistModel::onGotStatus(Status status)
{
    emit currentIndexChanged(status.currentIndex);
}

void SqueezeBoxPlaylistModel::onGotPlaylist(QVector<Song> playlist)
{
    bool isDiff = false;

    if (playlist.size() != _playlist.size()) isDiff = true;

    if (!isDiff) {
        for (int i = 0; i < playlist.size(); i++) {
            if (playlist[i].id != _playlist[i].id) {
                isDiff = true;
                break;
            }
        }
    }

    if (!isDiff) return;

    beginResetModel();

    _playlist.clear();
    _playlist = playlist;

    endResetModel();
}
