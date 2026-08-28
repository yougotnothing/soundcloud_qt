#include "PlaylistsService.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>

#include "config.h"
#include "data/consts.h"
#include "structs/Playlist.h"

PlaylistsService::PlaylistsService(QObject *parent) : QObject(parent) {}

void PlaylistsService::getPlaylistsPreview() {
  QUrl url(SOUNDCLOUD_URL + "/me/playlists");
  QUrlQuery query;

  query.addQueryItem("show_tracks", "false");
  query.addQueryItem("limit", "50");
  url.setQuery(query);

  QNetworkRequest request(url);

  request.setRawHeader(
      "Authorization",
      QString("OAuth " + Config::instance().accessToken).toUtf8());

  QNetworkReply *reply = manager.get(request);

  connect(reply, &QNetworkReply::finished, this, [reply, this]() {
    if (reply->error() == QNetworkReply::NoError) {
      const QByteArray data = reply->readAll();

      m_playlists.clear();

      this->parsePlaylists(data, m_playlists);

      emit playlistsGot();

    } else {
      qWarning() << "An error occurred:" << reply->errorString();
    }

    reply->deleteLater();
  });
}

void PlaylistsService::parsePlaylists(QByteArray data,
                                      QVariantList &playlists) {
  const QJsonArray parsedData = QJsonDocument::fromJson(data).array();

  for (const auto &obj : parsedData) {
    QVariantMap map;
    map["title"] = obj.toObject()["title"].toString();
    map["urn"] = obj.toObject()["urn"].toString();
    map["artworkUrl"] =
        obj.toObject()["artwork_url"].toString().length() > 0
            ? obj.toObject()["artwork_url"].toString()
            : obj.toObject()["tracks"][0]["artwork_url"].toString();
    playlists << map;
  }
}

void PlaylistsService::getPlaylistData(const QJsonObject &data) {
  m_urn = data["urn"].toString();
  m_artworkUrl = data["artwork_url"].toString().length() > 0
                     ? data["artwork_url"].toString()
                     : data["tracks"][0]["artwork_url"].toString();
  ;
  m_title = data["title"].toString();

  emit currentPlaylistChanged();
}

void PlaylistsService::parseCurrentPlaylistSongs(
    const QJsonObject &playlistData, QVariantList &playlist) {
  for (const auto &data : playlistData["tracks"].toArray()) {
    const QJsonObject &trackData = data.toObject();
    QVariantMap track;

    track["id"] = trackData["id"].toInteger();
    track["artworkUrl"] = trackData["artwork_url"].toString();
    track["uri"] = trackData["uri"].toString();
    track["createdAt"] = trackData["created_at"].toString();
    trackData["bpm"].isNull() ? track["bpm"] = QVariant()
                              : track["bpm"] = trackData["bpm"].toInt();
    track["commentCount"] = trackData["comment_count"].toInt();
    track["commentable"] = trackData["commentable"].toBool();
    track["favoritingsCount"] = trackData["favoritings_count"].toInt();
    track["playbackCount"] = trackData["playback_count"].toInt();
    track["releaseDay"] = trackData["release_day"].toInt();
    track["releaseMonth"] = trackData["release_month"].toInt();
    track["title"] = trackData["title"].toString();
    track["labelName"] = trackData["label_name"].toString();
    playlist << track;
  }
}

void PlaylistsService::getPlaylistSongs(const QString &urn) {
  QUrl url(SOUNDCLOUD_URL + "/playlists" + "/" + urn);
  QNetworkRequest request(url);

  request.setRawHeader(
      "Authorization",
      QString("OAuth " + Config::instance().accessToken).toUtf8());

  QNetworkReply *reply = manager.get(request);

  connect(reply, &QNetworkReply::finished, this, [reply, this]() {
    if (reply->error() == QNetworkReply::NoError) {
      QByteArray response = reply->readAll();
      QJsonObject playlistData = QJsonDocument::fromJson(response).object();

      getPlaylistData(playlistData);
      parseCurrentPlaylistSongs(playlistData, m_currentPlaylistSongs);

      qDebug() << "Playlist songs" << playlistData["tracks"];

      emit currentPlaylistChanged();

      reply->deleteLater();
    } else {
      qWarning() << reply->errorString();
    }
  });
}
