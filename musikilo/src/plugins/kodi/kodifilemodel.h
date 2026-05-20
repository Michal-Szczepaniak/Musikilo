#ifndef KODIFILEMODEL_H
#define KODIFILEMODEL_H

#include "kodimanager.h"

#include <QObject>

#include <src/filemodelinterface.h>

class KodiFileModel : public FileModelInterface
{
    Q_OBJECT
public:
    explicit KodiFileModel(KodiManager *manager, QObject *parent = nullptr);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    void getFilesList(QString path);

protected:
    QHash<int, QByteArray> roleNames() const;

public slots:
    void onGotFileList(QVector<KodiManager::File> files);
    void onGotSources(QVector<KodiManager::File> sources);

private:
    KodiManager *_manager;
    QVector<KodiManager::File> _entries;
    QVector<KodiManager::File> _sources;

    QString getPreviousPath(QString path);
};

#endif // KODIFILEMODEL_H
