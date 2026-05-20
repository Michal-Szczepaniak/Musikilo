#include "kodiplaylistmodel.h"

KodiPlaylistModel::KodiPlaylistModel(KodiManager *manager, KodiPlayer *player, QObject *parent) : PlaylistModelInterface(parent), _manager(manager), _player(player)
{
    connect(manager, &KodiManager::gotStatus, this, &KodiPlaylistModel::onGotStatus);
    connect(manager, &KodiManager::gotPlaylist, this, &KodiPlaylistModel::onGotPlaylist);
}

int KodiPlaylistModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return _playlist.size();
}

QVariant KodiPlaylistModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount())
        return QVariant();

    const PlaylistItem &item = _playlist[index.row()];
    if (role == Name) {
        return item.title;
    } else if (role == Path) {
        return item.index;
    }

    return QVariant();
}

void KodiPlaylistModel::reset()
{
    _manager->clearPlaylist();
}

void KodiPlaylistModel::play(int index)
{
    if (index < 0) return;

    _manager->play(index);
}

void KodiPlaylistModel::addSong(QString song)
{
    _manager->addFile(song);
}

void KodiPlaylistModel::playSong(QString song)
{
    _manager->playFile(song);
}

void KodiPlaylistModel::nextSong()
{
    _manager->nextSong();
}

void KodiPlaylistModel::prevSong()
{
    _manager->prevSong();
}

void KodiPlaylistModel::remove(int index)
{
    _manager->remove(index);
}

QHash<int, QByteArray> KodiPlaylistModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Name] = "name";
    roles[Path] = "path";
    return roles;
}

void KodiPlaylistModel::onGotStatus(Status status)
{
    emit currentIndexChanged(status.position);
}

void KodiPlaylistModel::onGotPlaylist(QVector<PlaylistItem> playlist)
{
    bool isDiff = false;

    if (playlist.size() != _playlist.size()) isDiff = true;

    if (!isDiff) {
        for (int i = 0; i < playlist.size(); i++) {
            if (playlist[i].title != _playlist[i].title) {
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
