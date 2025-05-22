#ifndef FILEMODELINTERFACE_H
#define FILEMODELINTERFACE_H

#include <QAbstractListModel>

class FileModelInterface : public QAbstractListModel
{
    Q_OBJECT
public:
    enum FilesListRoles {
        Name = Qt::UserRole + 1,
        Path,
        isDir
    };

    FileModelInterface(){};
    virtual ~FileModelInterface(){};

    virtual void getFilesList(QString path) = 0;

signals:
    void errorOccured(QString error);
};

#endif // FILEMODELINTERFACE_H
