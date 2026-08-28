#include "services/searchservice.h"
#include "data/consts.h"
#include "services/PlaylistsService.h"
#include "services/config.h"
#include "utils/searchtype.h"
#include <QtCore/qstringview.h>
#include <QtCore/qurl.h>
#include <QtCore/qurlquery.h>
#include <QtNetwork/qnetworkreply.h>

SearchService::SearchService(QObject *parent) : QObject(parent) {}

void SearchService::search(QString query, SearchType &type) {
  QString endpoint = type == SearchType::PLAYLIST ? "/playlists"
                     : type == SearchType::TRACK  ? "/tracks"
                                                  : "/users";
  QUrl url(SOUNDCLOUD_URL + endpoint);
  QUrlQuery params;

  params.addQueryItem("q", query);
  params.addQueryItem("limit", "5");

  url.setQuery(params);

  QNetworkRequest request(url);

  request.setRawHeader(
      "Authorization",
      QString("OAuth" + Config::instance().accessToken).toUtf8());
  QNetworkReply *reply = manager.get(request);

  connect(reply, &QNetworkReply::finished, this, [reply, this, type]() {
    const QByteArray data = reply->readAll();

    if (reply->error() == QNetworkReply::NoError) {
      if (type == SearchType::PLAYLIST) {
        PlaylistsService::parsePlaylists(data, testData);
      }
    }

    reply->deleteLater();
  });
}

void SearchService::setOpened(bool value) { dropdownOpened = value; }

void SearchService::setList(QList<QJsonObject> &data) { this->data = data; }
