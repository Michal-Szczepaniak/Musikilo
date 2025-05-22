#ifndef PLAYLISTMODELINTERFACE_H
#define PLAYLISTMODELINTERFACE_H

#include <QAbstractItemModel>

class PlaylistModelInterface : public QAbstractListModel
{
    Q_OBJECT
public:
    enum PlaylistRoles {
        Name = Qt::UserRole + 1,
        Path
    };

    PlaylistModelInterface(){};
    virtual ~PlaylistModelInterface(){};

    virtual void reset() = 0;
    virtual void play(int index) = 0;
    virtual void addSong(QString song) = 0;

signals:
    void errorOccured(QString error);
};

#endif // PLAYLISTMODELINTERFACE_H
