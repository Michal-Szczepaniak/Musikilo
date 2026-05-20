#ifndef TAUONFILEMODEL_H
#define TAUONFILEMODEL_H

#include "tauonmanager.h"

#include <QObject>

#include <src/filemodelinterface.h>

class TauonFileModel : public FileModelInterface
{
    Q_OBJECT
public:
    explicit TauonFileModel(TauonManager *manager, QObject *parent = nullptr);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    void getFilesList(QString path);

protected:
    QHash<int, QByteArray> roleNames() const;

public slots:
    void onGotTracklist(QVector<TauonManager::Song> list);

private:
    TauonManager *_manager;
    QVector<TauonManager::File> _entries;

    QString getPreviousPath(QString path);
};

#endif // TAUONFILEMODEL_H
