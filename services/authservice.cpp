#include "authservice.h"
#include "config.h"
#include "data/consts.h"
#include <QUrlQuery>
#include <QNetworkReply>
#include <QDesktopServices>

AuthService::AuthService(QObject *parent) : QObject(parent) {
    this->setToken(Config::instance().accessToken, TokenType::ACCESS);
    this->setToken(Config::instance().refreshToken, TokenType::REFRESH);

    qDebug() << "accessToken:" << this->accessToken;
    qDebug() << "refreshToken:" << this->refreshToken;
}

void AuthService::auth() {
    QNetworkRequest request(QUrl(SOUNDCLOUD_URL + "/oauth2/token"));
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");

    QUrlQuery query;
    query.addQueryItem("client_id", Config::instance().clientId);
    query.addQueryItem("client_secret", Config::instance().clientSecret);
    query.addQueryItem("grant_type", "client_credentials");

    const QByteArray data = query.toString(QUrl::FullyEncoded).toUtf8();

    QNetworkReply *reply = manager.post(request, data);

    qDebug() << request.headers();
    qDebug() << data;

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            const QByteArray response = reply->readAll();
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

    const QByteArray data = query.toString(QUrl::FullyEncoded).toUtf8();
    QNetworkReply *reply = manager.post(request, data);

    connect(reply, &QNetworkReply::finished, this, [reply, this]() {
        if (reply->error() == QNetworkReply::NoError) {
            const QByteArray response = reply->readAll();
            QJsonObject object = QJsonDocument::fromJson(response).object();

            Config::instance().setCredentials(object);

            qDebug() << Config::instance().accessToken;

            setToken(object["access_token"].toString(), TokenType::ACCESS);
            setToken(object["refresh_token"].toString(), TokenType::REFRESH);

            emit tokenReady(object["access_token"].toString());
            emit hasRefreshToken(object["refresh_token"].toString());

            qDebug() << response;
        } else qWarning() << reply->readAll();
    });
}

void AuthService::setToken(const QString& token, const TokenType type) {
    if (type == TokenType::ACCESS) {
        Config::instance().setAccessToken(token);
        this->accessToken = token;
    }else {
        Config::instance().setRefreshToken(token);
        this->refreshToken = token;
        emit hasRefreshToken(token);
    }
}


void AuthService::startOAuthFlow() {
    QUrl authUrl("https://soundcloud.com/connect");
    QUrlQuery query;
    query.addQueryItem("client_id", Config::instance().clientId);
    query.addQueryItem("redirect_uri", "http://localhost:1212/callback");
    query.addQueryItem("response_type", "code");
    query.addQueryItem("scope", "non-expiring");

    authUrl.setQuery(query);
    QDesktopServices::openUrl(authUrl);
}

void AuthService::exchangeCodeForToken(const QString& code) {
    const QUrl tokenUrl("https://api.soundcloud.com/oauth2/token");
    QUrlQuery body;

    body.addQueryItem("client_id", Config::instance().clientId);
    body.addQueryItem("client_secret", Config::instance().clientSecret);
    body.addQueryItem("grant_type", "authorization_code");
    body.addQueryItem("redirect_uri", "http://localhost:1212/callback");
    body.addQueryItem("code", code);

    const QByteArray postData = body.toString(QUrl::FullyEncoded).toUtf8();
    QNetworkRequest request(tokenUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QNetworkReply *reply = manager.post(request, postData);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if(reply->error() == QNetworkReply::NoError) {
            const QJsonObject obj = QJsonDocument::fromJson(reply->readAll()).object();

            Config::instance().setCredentials(obj);

            emit tokenReady(accessToken);
            emit hasRefreshToken(refreshToken);
        } else {
            qWarning() << "OAuth error:" << reply->errorString();
        }
        reply->deleteLater();
    });
}

void AuthService::init() {
    if (Config::instance().accessToken.data() && Config::instance().expiresAt > Config::instance().tokenObtained) {
        emit tokenReady(this->accessToken);
    } else this->refresh();

    if (this->refreshToken.isEmpty()) {
        emit hasNoRefreshToken();
    } else {
        emit hasRefreshToken(this->refreshToken);
    }
}
