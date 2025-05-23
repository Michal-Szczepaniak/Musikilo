#include "webdavplugin.h"

WebDavPlugin::WebDavPlugin(QObject *parent) : PluginInterface(parent)
{
    _readyTimer.setInterval(10);
    _readyTimer.setSingleShot(true);
    connect(&_readyTimer, &QTimer::timeout, [&](){ emit pluginReady(); });
}

void WebDavPlugin::initialize(QVariantMap settings)
{
    updateConig(settings);

    _fileModel = std::make_unique<WebDavFileModel>(&_webdav, this);
    _player = std::make_unique<WebDavPlayer>(&_webdav, this);
    _playlistModel = std::make_unique<WebDavPlaylistModel>(&_webdav, _player.get(), this);
}

void WebDavPlugin::updateConig(QVariantMap settings)
{
    int connectionType = settings.value("connectionType").toInt();
    QString hostname = settings.value("hostname").toString();
    QString rootPath = settings.value("rootPath").toString();
    QString username = settings.value("username").toString();
    QString password = settings.value("password").toString();
    int port = settings.value("port").toString().toInt();

    qDebug() << "Nextcloud settings: ";
    qDebug() << "Connection type: " << connectionType;
    qDebug() << "Hostname: " << hostname;
    qDebug() << "Root path: " << rootPath;
    qDebug() << "Username: " << username;
    qDebug() << "Port: " << port;

    _webdav.setConnectionSettings(connectionType == 1 ? QWebdav::HTTPS : QWebdav::HTTP, hostname, rootPath, username, password, port);
}

PlaylistModelInterface *WebDavPlugin::getPlaylistModel()
{
    return _playlistModel.get();
}

FileModelInterface *WebDavPlugin::getFileModel()
{
    return _fileModel.get();
}

PlayerInterface *WebDavPlugin::getPlayer()
{
    return _player.get();
}

void WebDavPlugin::testConfig()
{
    QNetworkReply *reply = _webdav.list("/", 100);
    connect(reply, &QNetworkReply::finished, this, &WebDavPlugin::checkFinished);
}

void WebDavPlugin::activate()
{
    _readyTimer.start();
}

void WebDavPlugin::deactivate()
{}

void WebDavPlugin::checkFinished()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());

    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "WebDavPlugin test error: " << reply->errorString();
        emit testFailed(reply->errorString());
    } else {
        emit testSucceeded();
    }
}
