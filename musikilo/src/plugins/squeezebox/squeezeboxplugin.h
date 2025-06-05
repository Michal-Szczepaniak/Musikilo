#ifndef SQUEEZEBOXPLUGIN_H
#define SQUEEZEBOXPLUGIN_H

#include "squeezeboxfilemodel.h"
#include "squeezeboxmanager.h"
#include "squeezeboxplayer.h"
#include "squeezeboxplaylistmodel.h"

#include <QObject>
#include <QSettings>
#include <QTimer>
#include <memory>

#include <src/plugininterface.h>

class SqueezeBoxPlugin : public PluginInterface
{
    Q_OBJECT
public:
    explicit SqueezeBoxPlugin(QObject *parent = nullptr);

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
    std::unique_ptr<SqueezeBoxFileModel> _fileModel;
    std::unique_ptr<SqueezeBoxPlaylistModel> _playlistModel;
    std::unique_ptr<SqueezeBoxPlayer> _player;
    SqueezeBoxManager _manager;
    QTimer _readyTimer;
    QTimer _updateTimer;
    bool _testPending = false;
};

#endif // SQUEEZEBOXPLUGIN_H
