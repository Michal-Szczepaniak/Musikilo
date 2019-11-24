#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <sailfishapp.h>
#include "webdavmodel.h"
#include "playlistmodel.h"

int main(int argc, char *argv[])
{
    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    QSharedPointer<QQuickView> view(SailfishApp::createView());

    WebDavModel webdavmodel;
    PlaylistModel playlistmodel;

    QObject::connect(&webdavmodel, &WebDavModel::gotMediaContent, &playlistmodel, &PlaylistModel::play);
    QObject::connect(&webdavmodel, &WebDavModel::gotAudioFile, &playlistmodel, &PlaylistModel::addFile);
    QObject::connect(&playlistmodel, &PlaylistModel::playFile, &webdavmodel, &WebDavModel::getMediaContent);

    view->rootContext()->setContextProperty("webdavmodel", &webdavmodel);
    view->rootContext()->setContextProperty("playlistmodel", &playlistmodel);

    view->setSource(SailfishApp::pathTo("qml/Musikilo.qml"));
    view->show();

    int ret = app->exec();
    return ret;
}
