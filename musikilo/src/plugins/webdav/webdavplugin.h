#ifndef WEBDAVPLUGIN_H
#define WEBDAVPLUGIN_H

#include "webdavfilemodel.h"
#include "webdavplayer.h"
#include "webdavplaylistmodel.h"

#include <QObject>
#include <memory>
#include <qwebdav.h>
#include <src/plugininterface.h>

class WebDavPlugin : public PluginInterface
{
    Q_OBJECT
public:
    explicit WebDavPlugin(QObject *parent = nullptr);

    void initialize(QVariantMap settings);
    void updateConig(QVariantMap settings);
    PlaylistModelInterface* getPlaylistModel();
    FileModelInterface* getFileModel();
    PlayerInterface* getPlayer();
    void testConfig();

    void activate();
    void deactivate();

private slots:
    void checkFinished();

private:
    std::unique_ptr<WebDavFileModel> _fileModel;
    std::unique_ptr<WebDavPlaylistModel> _playlistModel;
    std::unique_ptr<WebDavPlayer> _player;
    QWebdav _webdav;
    QTimer _readyTimer;
};

#endif // WEBDAVPLUGIN_H
