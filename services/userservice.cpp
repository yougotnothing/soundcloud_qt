#include "userservice.h"
#include "data/consts.h"
#include "services/config.h"
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QUrl>

UserService::UserService(QObject *parent) : QObject(parent) {}

void UserService::getUser() {
  const QNetworkRequest request(QUrl(SOUNDCLOUD_URL + "/me"));
  QNetworkReply *reply = manager.get(request);

  connect(reply, &QNetworkReply::finished, [this, reply]() {
    const QByteArray data = reply->readAll();

    qDebug() << "User data:" << data;

    emit gotUser();

    reply->deleteLater();
  });
}

void UserService::getFeed() {
  QNetworkRequest request(QUrl(SOUNDCLOUD_URL + "/me/feed?limit=3"));

  request.setRawHeader(
      "Authorization",
      QString("OAuth " + Config::instance().accessToken).toUtf8());

  QNetworkReply *reply = manager.get(request);

  connect(reply, &QNetworkReply::finished, [this, reply]() {
    const QByteArray data = reply->readAll();

    qDebug() << "Got feed:" << data;

    emit gotFeed();

    reply->deleteLater();
  });
}
