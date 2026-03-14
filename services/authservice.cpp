#include "authservice.h"
#include "config.h"
#include "data/consts.h"
#include <QUrlQuery>
#include <QNetworkReply>

AuthService::AuthService(QObject *parent) : QObject(parent) {
    auto &config = Config::instance();

    this->setToken(Config::instance().accessToken, TokenType::ACCESS);
    this->setToken(Config::instance().refreshToken, TokenType::REFRESH);
}

void AuthService::auth() {
    QNetworkRequest request(QUrl(SOUNDCLOUD_URL + "/oauth2/token"));
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");

    QUrlQuery query;
    query.addQueryItem("client_id", Config::instance().clientId);
    query.addQueryItem("client_secret", Config::instance().clientSecret);
    query.addQueryItem("grant_type", "client_credentials");

    QByteArray data = query.toString(QUrl::FullyEncoded).toUtf8();

    QNetworkReply *reply = manager.post(request, data);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QJsonObject tokens = QJsonDocument::fromJson(response).object();

            Config::instance().setCredentials(tokens);

            setToken(tokens["access_token"].toString(), TokenType::ACCESS);
            setToken(tokens["refresh_token"].toString(), TokenType::REFRESH);

            qDebug() << response;
        } else qWarning() << reply->errorString();

        reply->deleteLater();
    });
}

void AuthService::refresh() {
    QNetworkRequest request(QUrl(SOUNDCLOUD_URL + "/oauth2/token"));
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");

    QUrlQuery query;
    query.addQueryItem("client_id", Config::instance().clientId);
    query.addQueryItem("client_secret", Config::instance().clientSecret);
    query.addQueryItem("grant_type", "refresh_token");
    query.addQueryItem("refresh_token", this->refreshToken);

    QByteArray data = query.toString(QUrl::FullyEncoded).toUtf8();
    QNetworkReply *reply = manager.post(request, data);

    connect(reply, &QNetworkReply::finished, this, [reply, this]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QJsonObject object = QJsonDocument::fromJson(response).object();

            Config::instance().setCredentials(object);

            setToken(object["access_token"].toString(), TokenType::ACCESS);
            setToken(object["refresh_token"].toString(), TokenType::REFRESH);

            qDebug() << response;
        } else qWarning() << reply->readAll();
    });
}

void AuthService::setToken(QString token, TokenType type) {
    if (type == TokenType::ACCESS) this->accessToken = token;
    else this->refreshToken = token;
}
