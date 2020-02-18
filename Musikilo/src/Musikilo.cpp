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
#include "webdavmodel.h"
#include "playlistmodel.h"
#include "simplecrypt.h"
#include <Mpris>
#include <MprisPlayer>
#include <MprisManager>
#include <qofonoextmodemmanager.h>

int main(int argc, char *argv[])
{
    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    QSharedPointer<QQuickView> view(SailfishApp::createView());

    WebDavModel webdavmodel;
    PlaylistModel playlistmodel;
    SimpleCrypt simpleCrypt;

    qmlRegisterType<QOfonoExtModemManager>("com.verdanditeam.ofono", 1, 0, "OfonoModemManager");
    qmlRegisterSingletonType<Mpris>("com.verdanditeam.mpris", 1, 0, "Mpris", Mpris::api_factory);
    qmlRegisterType<MprisPlayer>("com.verdanditeam.mpris", 1, 0, "MprisPlayer");
    qmlRegisterType<MprisManager>("com.verdanditeam.mpris", 1, 0, "MprisManager");

    QObject::connect(&webdavmodel, &WebDavModel::gotMediaContent, &playlistmodel, &PlaylistModel::play);
    QObject::connect(&webdavmodel, &WebDavModel::gotAudioFile, &playlistmodel, &PlaylistModel::addFile);
    QObject::connect(&playlistmodel, &PlaylistModel::playFile, &webdavmodel, &WebDavModel::getMediaContent);

    view->rootContext()->setContextProperty("webdavmodel", &webdavmodel);
    view->rootContext()->setContextProperty("playlistmodel", &playlistmodel);
    view->rootContext()->setContextProperty("simpleCrypt", &simpleCrypt);

    view->setSource(SailfishApp::pathTo("qml/Musikilo.qml"));
    view->show();

    int ret = app->exec();
    return ret;
}
