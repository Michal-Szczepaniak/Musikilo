#include "playlistmodel.h"

PlaylistModel::PlaylistModel(QObject *parent) : QAbstractListModel(parent)
{
    connect(this, &PlaylistModel::rowsInserted, this, &PlaylistModel::onRowsInserted);
}

void PlaylistModel::reset()
{
    beginResetModel();
    mEntries.clear();
    endResetModel();
}

void PlaylistModel::addFile(QWebdavItem file)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    mEntries << file;
    qDebug() << "PlaylistModel " << file.name();
    endInsertRows();
}

void PlaylistModel::play(QMediaContent content)
{
    mMediaPlayer->setMedia(content);
    mMediaPlayer->play();
}

int PlaylistModel::activeItem()
{
    return mActiveItem;
}

void PlaylistModel::setActiveItem(int activeItem)
{
    mActiveItem = activeItem;
}

void PlaylistModel::setMediaPlayer(QObject *mediaPlayer)
{
    mMediaPlayer = qvariant_cast<QMediaPlayer*>(mediaPlayer->property("mediaObject"));
    connect(mMediaPlayer, &QMediaPlayer::stateChanged, this, &PlaylistModel::onStateChanged);
}

int PlaylistModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mEntries.size();
}

QVariant PlaylistModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount())
        return QVariant();

    const QWebdavItem &file = mEntries[index.row()];
    if (role == Name)
        return file.name();
    else if (role == Path)
        return file.path();

    return QVariant();
}

Qt::ItemFlags PlaylistModel::flags(const QModelIndex &index) const {
    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

QHash<int, QByteArray> PlaylistModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Name] = "name";
    roles[Path] = "path";
    return roles;
}

void PlaylistModel::onRowsInserted(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent);
    Q_UNUSED(first);
    Q_UNUSED(last);

    if (mMediaPlayer->state() == QMediaPlayer::StoppedState) {
        emit playFile(mEntries[0].path());
    }
}

void PlaylistModel::onStateChanged(QMediaPlayer::State state)
{
    switch (state) {
    case QMediaPlayer::StoppedState:
        if (mActiveItem + 1 < rowCount()) {
            mActiveItem++;
            emit playFile(mEntries[mActiveItem].path());
        }
        break;
    }
}
