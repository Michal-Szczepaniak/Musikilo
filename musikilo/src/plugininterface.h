#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include "filemodelinterface.h"
#include "playerinterface.h"
#include "playlistmodelinterface.h"

class PluginInterface : public QObject
{
    Q_OBJECT
public:
    PluginInterface(QObject *parent = nullptr) : QObject(parent) {};
    virtual ~PluginInterface(){};

    virtual void initialize(QVariantMap settings) = 0;
    virtual void updateConig(QVariantMap settings) = 0;
    virtual PlaylistModelInterface* getPlaylistModel() = 0;
    virtual FileModelInterface* getFileModel() = 0;
    virtual PlayerInterface* getPlayer() = 0;
    virtual void testConfig() = 0;

    virtual void activate() = 0;
    virtual void deactivate() = 0;

signals:
    void testSucceeded();
    void testFailed(QString message);
    void pluginReady();
};

#endif // PLUGININTERFACE_H
