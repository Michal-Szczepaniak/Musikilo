#ifndef SUBSONICPLUGIN_H
#define SUBSONICPLUGIN_H

#include "subsonicfilemodel.h"
#include "subsonicmanager.h"
#include "subsonicplayer.h"
#include "subsonicplaylistmodel.h"

#include <QObject>
#include <QSettings>
#include <QTimer>
#include <memory>

#include <src/plugininterface.h>

class SubsonicPlugin : public PluginInterface
{
    Q_OBJECT
public:
    explicit SubsonicPlugin(QObject *parent = nullptr);

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
    std::unique_ptr<SubsonicFileModel> _fileModel;
    std::unique_ptr<SubsonicPlaylistModel> _playlistModel;
    std::unique_ptr<SubsonicPlayer> _player;
    SubsonicManager _manager;
    QTimer _readyTimer;
    bool _testPending = false;
};

#endif // SUBSONICPLUGIN_H
