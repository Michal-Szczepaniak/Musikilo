#ifndef TAUONPLUGIN_H
#define TAUONPLUGIN_H

#include "tauonfilemodel.h"
#include "tauonmanager.h"
#include "tauonplayer.h"
#include "tauonplaylistmodel.h"

#include <QObject>
#include <QSettings>
#include <QTimer>
#include <memory>

#include <src/plugininterface.h>

class TauonPlugin : public PluginInterface
{
    Q_OBJECT
public:
    explicit TauonPlugin(QObject *parent = nullptr);

    void initialize(QVariantMap settings);
    void updateConig(QVariantMap settings);
    PlaylistModelInterface* getPlaylistModel();
    FileModelInterface* getFileModel();
    PlayerInterface* getPlayer();
    void testConfig();

    void activate();
    void deactivate();
signals:

private:
    std::unique_ptr<TauonFileModel> _fileModel;
    std::unique_ptr<TauonPlaylistModel> _playlistModel;
    std::unique_ptr<TauonPlayer> _player;
    TauonManager _manager;
    QTimer _readyTimer;
    QTimer _updateTimer;
    bool _testPending = false;
};

#endif // TAUONPLUGIN_H
