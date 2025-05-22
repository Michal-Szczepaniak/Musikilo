#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include "filemodelinterface.h"
#include "playerinterface.h"
#include "playlistmodelinterface.h"

class PluginInterface
{
public:
    PluginInterface() {};
    virtual ~PluginInterface(){};

    virtual void initialize(QVariantMap settings) = 0;
    virtual void updateConig(QVariantMap settings) = 0;
    virtual PlaylistModelInterface* getPlaylistModel() = 0;
    virtual FileModelInterface* getFileModel() = 0;
    virtual PlayerInterface* getPlayer() = 0;
    virtual void testConfig() = 0;

signals:
    virtual void testSucceeded() = 0;
    virtual void testFailed(QString message) = 0;
};

#endif // PLUGININTERFACE_H
