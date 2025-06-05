#ifndef MPDPLUGIN_H
#define MPDPLUGIN_H

#include "mpdfilemodel.h"
#include "mpdplaylistmodel.h"

#include <src/plugininterface.h>
#include <mpd/networkaccess.h>
#include <memory>

class MPDPlugin : public PluginInterface
{
    Q_OBJECT
public:
    explicit MPDPlugin(QObject *parent = nullptr);

    void initialize(QVariantMap settings);
    void updateConig(QVariantMap settings);
    PlaylistModelInterface* getPlaylistModel();
    FileModelInterface* getFileModel();
    PlayerInterface* getPlayer();
    void testConfig();

    void activate();
    void deactivate();

signals:
    void connectToHost();
    void disconnectFromHost();

private slots:

private:
    std::unique_ptr<MPDFileModel> _fileModel;
    std::unique_ptr<MPDPlaylistModel> _playlistModel;
    std::unique_ptr<MPDPlayer> _player;
    QThread *_mpdThread;
    NetworkAccess _mpd;
    bool _testPending;
    QTimer _timeoutTimer;
};

#endif // MPDPLUGIN_H
