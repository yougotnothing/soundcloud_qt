#include "authservice.h"
#include "config.h"
#include "data/consts.h"
#include <QUrlQuery>
#include <QNetworkReply>
#include <QDesktopServices>

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

    qDebug() << request.headers();
    qDebug() << data;

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QJsonObject tokens = QJsonDocument::fromJson(response).object();

            Config::instance().setCredentials(tokens);

            qDebug() << "Config Access Token:" << Config::instance().accessToken;

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

            qDebug() << Config::instance().accessToken;

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


void AuthService::startOAuthFlow() {
    QUrl authUrl("https://soundcloud.com/connect");
    QUrlQuery query;
    query.addQueryItem("client_id", Config::instance().clientId);
    query.addQueryItem("redirect_uri", "http://localhost:1212/callback"); // слушаем локальный сервер
    query.addQueryItem("response_type", "code");
    query.addQueryItem("scope", "non-expiring"); // обязательно для полного стрима

    authUrl.setQuery(query);
    QDesktopServices::openUrl(authUrl); // открываем браузер
}

void AuthService::exchangeCodeForToken(QString code) {
    QUrl tokenUrl("https://api.soundcloud.com/oauth2/token");
    QUrlQuery body;
    body.addQueryItem("client_id", Config::instance().clientId);
    body.addQueryItem("client_secret", Config::instance().clientSecret);
    body.addQueryItem("grant_type", "authorization_code");
    body.addQueryItem("redirect_uri", "http://localhost:1212/callback");
    body.addQueryItem("code", code);

    QByteArray postData = body.toString(QUrl::FullyEncoded).toUtf8();
    QNetworkRequest request(tokenUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QNetworkReply *reply = manager.post(request, postData);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if(reply->error() == QNetworkReply::NoError) {
            QJsonObject obj = QJsonDocument::fromJson(reply->readAll()).object();
            accessToken = obj["access_token"].toString();

            Config::instance().setAccessToken(obj["access_token"].toString());

            emit tokenReady(accessToken);

            qDebug() << "Got full access token:" << Config::instance().accessToken;
        } else {
            qWarning() << "OAuth error:" << reply->errorString();
        }
        reply->deleteLater();
    });
}
