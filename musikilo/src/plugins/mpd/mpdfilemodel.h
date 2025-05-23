#ifndef MPDFILEMODEL_H
#define MPDFILEMODEL_H

#include <QObject>

#include <src/filemodelinterface.h>

#include <mpd/networkaccess.h>

class MPDFileModel : public FileModelInterface
{
    Q_OBJECT
public:
    explicit MPDFileModel(NetworkAccess *mpd, QObject *parent = nullptr);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    void getFilesList(QString path);

protected:
    QHash<int, QByteArray> roleNames() const;

signals:
    void getMPDFilesList(QString path);

public slots:
    void onFilesReady(QList<QObject*> *filesList);

private:
    NetworkAccess *_mpd;
    QList<MpdFileEntry*> _filesList;

    QString getPreviousPath(MpdFileEntry *file);
};

#endif // MPDFILEMODEL_H
