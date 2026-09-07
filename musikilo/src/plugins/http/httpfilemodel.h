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

#ifndef HTTPHTTPMODEL_H
#define HTTPHTTPMODEL_H

#include <QObject>

#include <QMediaContent>
#include <src/filemodelinterface.h>

class HttpFileModel : public FileModelInterface
{
    Q_OBJECT
public:
    explicit HttpFileModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    void getFilesList(QString path) { Q_UNUSED(path) };

protected:
    QHash<int, QByteArray> roleNames() const;
};

#endif // HTTPHTTPMODEL_H
