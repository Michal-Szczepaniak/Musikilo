#include "mpdplaylistmodel.h"

MPDPlaylistModel::MPDPlaylistModel(NetworkAccess *mpd, MPDPlayer *player , QObject *parent) : PlaylistModelInterface(parent), _mpd(mpd), _player(player), _mpdStatus(_mpd->getMPDPlaybackStatus())
{
    connect(_mpd, &NetworkAccess::currentPlaylistReady, this, &MPDPlaylistModel::onPlaylistReady);
    connect(_mpdStatus, &MPDPlaybackStatus::idChanged, this, &PlaylistModelInterface::currentIndexChanged);
    connect(this, &MPDPlaylistModel::mpdPlay, _mpd, &NetworkAccess::playTrackByNumber);
    connect(this, &MPDPlaylistModel::mpdClearPlaylist, _mpd, &NetworkAccess::clearPlaylist);
    connect(this, &MPDPlaylistModel::mpdAddSong, _mpd, &NetworkAccess::addTrackToPlaylist);
    connect(this, &MPDPlaylistModel::mpdPlaySong, _mpd, &NetworkAccess::playTrack);
    connect(_mpd, &NetworkAccess::connectionEstablished, [&](){
        emit currentIndexChanged(_mpdStatus->getID());
    });
}

int MPDPlaylistModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _entries.length();
}

QVariant MPDPlaylistModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount())
        return QVariant();

    const MpdTrack *file = _entries[index.row()];
    if (role == Name)
        return file->getFileName();
    else if (role == Path)
        return file->getFileUri();

    return QVariant();
}

void MPDPlaylistModel::reset()
{
    emit mpdClearPlaylist();
}

void MPDPlaylistModel::play(int index)
{
    emit currentIndexChanged(_mpdStatus->getID());

    if (index < 0 || index >= rowCount()) {
        return;
    }

    emit mpdPlay(index);
}

void MPDPlaylistModel::addSong(QString song)
{
    if (song.startsWith('/')) song = song.mid(1);
    emit mpdAddSong(song);
}

void MPDPlaylistModel::playSong(QString song)
{
    if (song.startsWith('/')) song = song.mid(1);
    emit mpdPlaySong(song);
}

void MPDPlaylistModel::onPlaylistReady(QList<MpdTrack*> *playlist)
{
    beginResetModel();

    MpdTrack *track;
    foreach(track, _entries) {
        delete track;
    }
    _entries.clear();

    _entries.append(*playlist);
    endResetModel();

    delete playlist;
}

void MPDPlaylistModel::onCurrentIndexChanged(int index)
{
    emit currentIndexChanged(index - 1);
}

QHash<int, QByteArray> MPDPlaylistModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Name] = "name";
    roles[Path] = "path";
    return roles;
}
