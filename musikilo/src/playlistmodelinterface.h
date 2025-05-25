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

    PlaylistModelInterface(QObject *parent = nullptr) : QAbstractListModel(parent) {};
    virtual ~PlaylistModelInterface(){};

    virtual void reset() = 0;
    virtual void play(int index) = 0;
    virtual void addSong(QString song) = 0;
    virtual void playSong(QString song) = 0;
    virtual void nextSong() = 0;
    virtual void prevSong() = 0;

signals:
    void errorOccured(QString error);
    void currentIndexChanged(int currentIndex);
};

#endif // PLAYLISTMODELINTERFACE_H
