#ifndef SQUEEZEBOXFILEMODEL_H
#define SQUEEZEBOXFILEMODEL_H

#include <QObject>

#include <jcon/json_rpc_tcp_client.h>

#include <src/filemodelinterface.h>

class SqueezeBoxFileModel : public FileModelInterface
{
    Q_OBJECT
public:
    explicit SqueezeBoxFileModel(jcon::JsonRpcTcpClient *jcon, QObject *parent = nullptr);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    void getFilesList(QString path);

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    jcon::JsonRpcTcpClient *_jcon;
};

#endif // SQUEEZEBOXFILEMODEL_H
