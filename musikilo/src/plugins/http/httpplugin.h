#ifndef HTTPPLUGIN_H
#define HTTPPLUGIN_H

#include "httpfilemodel.h"
#include "httpplayer.h"
#include "httpplaylistmodel.h"

#include <QObject>
#include <memory>
#include <qwebdav.h>
#include <src/plugininterface.h>

class HttpPlugin : public PluginInterface
{
    Q_OBJECT
public:
    explicit HttpPlugin(QObject *parent = nullptr);

    void initialize(QVariantMap settings);
    void updateConig(QVariantMap settings);
    PlaylistModelInterface* getPlaylistModel();
    FileModelInterface* getFileModel();
    PlayerInterface* getPlayer();
    void testConfig();

    void activate();
    void deactivate();

private:
    std::unique_ptr<HttpFileModel> _fileModel;
    std::unique_ptr<HttpPlaylistModel> _playlistModel;
    std::unique_ptr<HttpPlayer> _player;
    QString _path{};
    QNetworkAccessManager _manager;
};

#endif // HTTPPLUGIN_H
