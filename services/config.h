#ifndef CONFIG_H
#define CONFIG_H

#include <QJsonObject>
#include <QFile>
#include <QIoDevice>
#include <QDebug>
#include <QDir>
#include <QSettings>
#include <QStandardPaths>

class Config {
    public:
        static Config& instance() {
            static Config cfg;
            return cfg;
        }

        void setCredentials(QJsonObject data) {
            accessToken = data["access_token"].toString();
            refreshToken = data["refresh_token"].toString();
            expiresAt = QDateTime::currentDateTime().addSecs(3600).toSecsSinceEpoch();
            tokenObtained = QDateTime::currentDateTime().toSecsSinceEpoch();

            settings.setValue("auth/accessToken", accessToken);
            settings.setValue("auth/refreshToken", refreshToken);
            settings.setValue("auth/expiresIn", expiresAt);
            settings.setValue("auth/tokenObtained", tokenObtained);
        }

        void setAccessToken(const QString& token) {
            accessToken = token;
            expiresAt = QDateTime::currentDateTime().addSecs(3600).toSecsSinceEpoch();

            settings.setValue("auth/expiresIn", expiresAt);
            settings.setValue("auth/accessToken", token);
        }

        void setRefreshToken(const QString& token) {
            refreshToken = token;

            settings.setValue("auth/refreshToken", token);
        }

        QString clientId;
        QString clientSecret;
        QString accessToken;
        QString refreshToken;
        qint64 expiresAt;
        qint64 tokenObtained;

    private:
        Config() {
            QFile config(":/config.json");

            if (!config.open(QIODevice::ReadOnly)) {
                qWarning() << "Cannot open config.json resource!";
                return;
            }

            QJsonObject data = QJsonDocument::fromJson(config.readAll()).object();

            clientId = data["CLIENT_ID"].toString();
            clientSecret = data["CLIENT_SECRET"].toString();
            accessToken = settings.value("auth/accessToken").toString();
            refreshToken = settings.value("auth/refreshToken").toString();
            expiresAt = settings.value("auth/expiresIn").toLongLong();
            tokenObtained = settings.value("auth/tokenObtained").toLongLong();
        }

        QSettings settings = QSettings("yougotnothing", "soundcloud_qt");

        Config(const Config&) = delete;
        Config& operator=(const Config&) = delete;
};

#endif // CONFIG_H
