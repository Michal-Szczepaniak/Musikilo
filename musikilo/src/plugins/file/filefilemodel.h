/*
    Copyright (C) 2019 Michał Szczepaniak

    This file is part of Musikilo.

    Musikilo is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Musikilo is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Musikilo. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef FILEFILEMODEL_H
#define FILEFILEMODEL_H

#include <QObject>

#include <QMediaContent>
#include <src/filemodelinterface.h>

class FileFileModel : public FileModelInterface
{
    Q_OBJECT
public:
    explicit FileFileModel(QObject *parent = nullptr);

    enum FileType {
        Folder,
        Track,
        Unknown,
    };

    struct File {
        QString name;
        QString path;
        FileType type;
    };

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    void getFilesList(QString path);
    QList<File> getFiles(QString path);
    File getFile(QString path);
    void setPath(QString path);

protected:
    QHash<int, QByteArray> roleNames() const;

signals:
    void gotFilesList();

private:
    QString _path;
    QList<File> _filesList;

    QString getPreviousPath(QString path);
};

#endif // FILEFILEMODEL_H
