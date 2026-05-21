#ifndef SUBSONICFILEMODEL_H
#define SUBSONICFILEMODEL_H

#include "subsonicmanager.h"

#include <QObject>

#include <src/filemodelinterface.h>

class SubsonicFileModel : public FileModelInterface
{
    Q_OBJECT
public:
    explicit SubsonicFileModel(SubsonicManager *manager, QObject *parent = nullptr);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    void getFilesList(QString path);

protected:
    QHash<int, QByteArray> roleNames() const;

public slots:
    void onGotFileList(QVector<SubsonicManager::File> files);

private:
    SubsonicManager *_manager;
    QVector<SubsonicManager::File> _entries;

    QString getPreviousPath(QString path);
};

#endif // SUBSONICFILEMODEL_H
