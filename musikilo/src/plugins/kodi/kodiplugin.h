#ifndef KODIPLUGIN_H
#define KODIPLUGIN_H

#include "kodifilemodel.h"
#include "kodimanager.h"
#include "kodiplayer.h"
#include "kodiplaylistmodel.h"

#include <QObject>
#include <QSettings>
#include <QTimer>
#include <memory>

#include <src/plugininterface.h>

class KodiPlugin : public PluginInterface
{
    Q_OBJECT
public:
    explicit KodiPlugin(QObject *parent = nullptr);

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
    std::unique_ptr<KodiFileModel> _fileModel;
    std::unique_ptr<KodiPlaylistModel> _playlistModel;
    std::unique_ptr<KodiPlayer> _player;
    KodiManager _manager;
    QTimer _readyTimer;
    QTimer _updateTimer;
    bool _testPending = false;
};

#endif // KODIPLUGIN_H
