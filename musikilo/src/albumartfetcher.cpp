#include "albumartfetcher.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>

#define STR(x) #x
#define STRINGIFY(x) STR(x)

AlbumArtFetcher::AlbumArtFetcher(QObject *parent) : QObject(parent)
{
    connect(&_manager, &QNetworkAccessManager::finished, this, &AlbumArtFetcher::onSongInfo);
}

void AlbumArtFetcher::searchAlbumArt(QString song, QString artist, QString album)
{
    song = song.trimmed();
    artist = artist.trimmed();
    album = album.trimmed();

    if (_cache[0] == song && _cache[1] == artist && _cache[2] == album) {
        return;
    } else {
        _cache[0] = song;
        _cache[1] = artist;
        _cache[2] = album;
    }

    if (song.isEmpty() && artist.isEmpty() && album.isEmpty())
        return;

    QUrl url("https://ws.audioscrobbler.com/2.0/");
    QUrlQuery params;

    params.addQueryItem("api_key", STRINGIFY(LASTFM_API_KEY));
    params.addQueryItem("format", "json");
    params.addQueryItem("autocorrect", "1");

    if (!artist.isEmpty() && !album.isEmpty()) {
        params.addQueryItem("method", "album.getinfo");
        params.addQueryItem("artist", artist);
        params.addQueryItem("album", album);
    } else if (!song.isEmpty() && !artist.isEmpty()) {
        params.addQueryItem("method", "track.getinfo");
        params.addQueryItem("artist", artist);
        params.addQueryItem("track", song);
    } else if (!album.isEmpty()) {
        params.addQueryItem("method", "album.search");
        params.addQueryItem("album", album);
        params.addQueryItem("limit", "1");
    } else if (!song.isEmpty()) {
        params.addQueryItem("method", "track.search");
        params.addQueryItem("track", song);
        params.addQueryItem("limit", "1");
    } else {
        return;
    }

    url.setQuery(params);

    QNetworkRequest request(url);
    _manager.get(request);
}

void AlbumArtFetcher::onSongInfo(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << reply->errorString();
        reply->deleteLater();
        return;
    }

    const QByteArray data = reply->readAll();
    reply->deleteLater();

    const QJsonDocument json = QJsonDocument::fromJson(data);

    if (!json.isObject())
        return;

    const QJsonObject root = json.object();

    if (root.contains("error")) {
        qDebug() << root["error"].toInt() << root["message"].toString();
        return;
    }

    if (root.contains("album")) {
        const QJsonArray images = root["album"].toObject()["image"].toArray();

        for (QJsonValue image : images) {
            const QJsonObject imageObj = image.toObject();

            if (imageObj["size"].toString() == "large") {
                const QString url = imageObj["#text"].toString();

                if (!url.isEmpty()) {
                    emit gotAlbumArt(url);
                    return;
                }
            }
        }

        for (int i = images.size() - 1; i >= 0; --i) {
            const QString url = images[i].toObject()["#text"].toString();

            if (!url.isEmpty()) {
                emit gotAlbumArt(url);
                return;
            }
        }

        return;
    }
}
