#ifndef SQUEEZEBOXFILEMODEL_H
#define SQUEEZEBOXFILEMODEL_H

#include "squeezeboxmanager.h"

#include <QObject>

#include <src/filemodelinterface.h>

class SqueezeBoxFileModel : public FileModelInterface
{
    Q_OBJECT
public:
    explicit SqueezeBoxFileModel(SqueezeBoxManager *manager, QObject *parent = nullptr);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    void getFilesList(QString path);

protected:
    QHash<int, QByteArray> roleNames() const;

public slots:
    void onGotMusicFolder(QVector<SqueezeBoxManager::File> folder);
    void onGotTopMusicFolder(SqueezeBoxManager::File folder);

private:
    SqueezeBoxManager *_manager;
    QVector<SqueezeBoxManager::File> _entries;
    QString _topPath{};

    QString getPreviousPath(SqueezeBoxManager::File file);
};

#endif // SQUEEZEBOXFILEMODEL_H
