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

#include <QtQuick>

#include <sailfishapp.h>
#include "filemodel.h"
#include "player.h"
#include "playlistmodel.h"
#include "settingsmanager.h"
#include "simplecrypt.h"

int main(int argc, char *argv[])
{
    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    QSharedPointer<QQuickView> view(SailfishApp::createView());

    qsrand(QDateTime::currentMSecsSinceEpoch() / 1000);

    SettingsManager settingsManager;
    FileModel fileModel(&settingsManager);
    Player player(&settingsManager);
    PlaylistModel playlistModel(&settingsManager, &player);
    SimpleCrypt simpleCrypt;

    view->rootContext()->setContextProperty("settingsManager", &settingsManager);
    view->rootContext()->setContextProperty("fileModel", &fileModel);
    view->rootContext()->setContextProperty("playlistModel", &playlistModel);
    view->rootContext()->setContextProperty("player", &player);
    view->rootContext()->setContextProperty("simpleCrypt", &simpleCrypt);

    view->setSource(SailfishApp::pathTo("qml/Musikilo.qml"));
    view->show();

    int ret = app->exec();
    return ret;
}
