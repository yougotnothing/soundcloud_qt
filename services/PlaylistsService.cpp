#include "PlaylistsService.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>

#include "config.h"
#include "data/consts.h"

PlaylistsService::PlaylistsService(QObject *parent) : QObject(parent) {}

void PlaylistsService::getPlaylistsPreview() {
    QUrl url(SOUNDCLOUD_URL + "/me/playlists");
    QUrlQuery query;

    query.addQueryItem("show_tracks", "true");
    query.addQueryItem("limit", "50");
    url.setQuery(query);

    QNetworkRequest request(url);

    request.setRawHeader("Authorization", QString("OAuth " + Config::instance().accessToken).toUtf8());

    QNetworkReply *reply = manager.get(request);

    connect(reply, &QNetworkReply::finished, this, [reply, this]() {
        if (reply->error() == QNetworkReply::NoError) {
            const QJsonArray data = QJsonDocument::fromJson(reply->readAll()).array();

            m_playlists.clear();

            qDebug() << data[0].toObject()["tracks"][0].toObject()["artwork_url"].toString();
            for (const auto &obj : data) {
                QVariantMap map;
                map["title"] = obj.toObject()["title"].toString();
                map["urn"] = obj.toObject()["urn"].toString();
                map["artworkUrl"] = obj.toObject()["artwork_url"].toString().length() > 0 ?
                                      obj.toObject()["artwork_url"].toString() :
                                      obj.toObject()["tracks"][0]["artwork_url"].toString();
                m_playlists << map;
            }

            qDebug() << "Playlists:" << m_playlists;

            emit playlistsGot();
        } else {
            qWarning() << "An error occurred:" << reply->errorString();
        }

        reply->deleteLater();
    });
}
