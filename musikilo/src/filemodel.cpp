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

#include "filemodel.h"

FileModel::FileModel(SettingsManager *settingsManager, QObject *parent) : QAbstractListModel(parent), _settingsManager(settingsManager)
{
    connect(settingsManager, &SettingsManager::currentPluginChanged, this, &FileModel::onPluginChange);
}

int FileModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    if (_fileModel == nullptr) return 0;

    return _fileModel->rowCount();
}

QVariant FileModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount() || _fileModel == nullptr) {
        return QVariant();
    }

    return _fileModel->data(index, role);
}

void FileModel::getFilesList(QString path)
{
    if (_fileModel == nullptr) return;

    _fileModel->getFilesList(path);
}

void FileModel::onPluginChange()
{
    auto plugin = _settingsManager->getCurrentPlugin();
    if (plugin != nullptr) {
        beginResetModel();
        if (_fileModel != nullptr) {
            disconnect(_modelResetSigal);
            disconnect(_errorOccuredSignal);
        }

        _fileModel = plugin->getFileModel();
        _modelResetSigal = connect(_fileModel, &FileModelInterface::modelReset, this, &FileModel::onModelReset);
        _errorOccuredSignal = connect(_fileModel, &FileModelInterface::errorOccured, this, &FileModel::errorOccured);
        endResetModel();
    }

    emit pluginChanged();
}

void FileModel::onModelReset()
{
    beginResetModel();
    endResetModel();
}

QHash<int, QByteArray> FileModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Name] = "name";
    roles[Path] = "path";
    roles[IsDir] = "isDir";
    return roles;
}
