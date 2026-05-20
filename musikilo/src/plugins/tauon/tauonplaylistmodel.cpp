#include "tauonplaylistmodel.h"

TauonPlaylistModel::TauonPlaylistModel(TauonManager *manager, TauonPlayer *player, QObject *parent) : PlaylistModelInterface(parent), _manager(manager), _player(player)
{
    connect(manager, &TauonManager::gotStatus, this, &TauonPlaylistModel::onGotStatus);
    connect(manager, &TauonManager::gotTracklist, this, &TauonPlaylistModel::onGotPlaylist);
}

int TauonPlaylistModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return _playlist.size();
}

QVariant TauonPlaylistModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount())
        return QVariant();

    const Song &song = _playlist[index.row()];
    if (role == Name) {
        QString trackNumber = song.trackNumber;
        if (!trackNumber.isEmpty()) {
            trackNumber = trackNumber.contains("/") ? trackNumber.left(trackNumber.indexOf("/")) : trackNumber;

            trackNumber = trackNumber.rightJustified(2, '0') + ". ";
        }

        QString title = song.title + " - " + song.artist;
        if (song.title.isEmpty() && song.artist.isEmpty()) {
            title = QUrl(song.path).fileName();
        }

        return trackNumber + title;
    } else if (role == Path) {
        return song.path;
    }

    return QVariant();
}

void TauonPlaylistModel::play(int index)
{
    if (index < 0) return;

    _manager->play(index);
}

void TauonPlaylistModel::addSong(QString song)
{
    _manager->playPath(song);
}

void TauonPlaylistModel::playSong(QString song)
{
    _manager->playPath(song);
}

void TauonPlaylistModel::nextSong()
{
    _manager->nextSong();
}

void TauonPlaylistModel::prevSong()
{
    _manager->prevSong();
}

QHash<int, QByteArray> TauonPlaylistModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Name] = "name";
    roles[Path] = "path";
    return roles;
}

void TauonPlaylistModel::onGotStatus(Status status)
{
    emit currentIndexChanged(status.position);
}

void TauonPlaylistModel::onGotPlaylist(QVector<Song> playlist)
{
    beginResetModel();

    _playlist.clear();
    _playlist = playlist;

    endResetModel();
}
