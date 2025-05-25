#include "squeezeboxfilemodel.h"

SqueezeBoxFileModel::SqueezeBoxFileModel(jcon::JsonRpcTcpClient *jcon, QObject *parent) : FileModelInterface(parent), _jcon(jcon)
{

}

int SqueezeBoxFileModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 0;
}

QVariant SqueezeBoxFileModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount())
        return QVariant();

    return QVariant();
}

void SqueezeBoxFileModel::getFilesList(QString path)
{
}

QHash<int, QByteArray> SqueezeBoxFileModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Name] = "name";
    roles[Path] = "path";
    roles[isDir] = "isDir";
    return roles;
}
