#ifndef ALBUMARTFETCHER_H
#define ALBUMARTFETCHER_H

#include <QNetworkAccessManager>
#include <QObject>

class AlbumArtFetcher : public QObject
{
    Q_OBJECT
public:
    explicit AlbumArtFetcher(QObject *parent = nullptr);

    Q_INVOKABLE void searchAlbumArt(QString song, QString artist, QString album);

signals:
    void gotAlbumArt(QString url);

private slots:
    void onSongInfo(QNetworkReply *reply);

private:
    QNetworkAccessManager _manager;
    QStringList _cache{"", "", ""};
};

#endif // ALBUMARTFETCHER_H
