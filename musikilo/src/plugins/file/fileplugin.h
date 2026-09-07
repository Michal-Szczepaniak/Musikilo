#ifndef FILEPLUGIN_H
#define FILEPLUGIN_H

#include "filefilemodel.h"
#include "fileplayer.h"
#include "fileplaylistmodel.h"

#include <QObject>
#include <memory>
#include <qwebdav.h>
#include <src/plugininterface.h>

class FilePlugin : public PluginInterface
{
    Q_OBJECT
public:
    explicit FilePlugin(QObject *parent = nullptr);

    void initialize(QVariantMap settings);
    void updateConig(QVariantMap settings);
    PlaylistModelInterface* getPlaylistModel();
    FileModelInterface* getFileModel();
    PlayerInterface* getPlayer();
    void testConfig();

    void activate();
    void deactivate();

private:
    std::unique_ptr<FileFileModel> _fileModel;
    std::unique_ptr<FilePlaylistModel> _playlistModel;
    std::unique_ptr<FilePlayer> _player;
    QString _path{};
};

#endif // FILEPLUGIN_H
