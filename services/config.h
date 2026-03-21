#ifndef CONFIG_H
#define CONFIG_H

#include <QJsonObject>
#include <QFile>
#include <QIoDevice>
#include <QDebug>
#include <QDir>
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
        expiresAt = data["expires_in"].toInt();
        tokenObtained = QDateTime::currentDateTimeUtc();

        QFile file(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/config.json");

        if(file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            QString jsonString = QString(R"({
                "CLIENT_ID": "%1",
                "CLIENT_SECRET": "%2",
                "ACCESS_TOKEN": "%3",
                "REFRESH_TOKEN": "%4",
                "EXPIRES_IN": %5,
                "TOKEN_OBTAINED": "%6"
            })")
            .arg(clientId,
                 clientSecret,
                 accessToken,
                 refreshToken,
                 QString::number(expiresAt),
                 tokenObtained.toString(Qt::ISODate)
            );

            file.write(jsonString.toUtf8());
            file.close();
        }
    }

    void setAccessToken(QString token) {
        accessToken = token;

        QFile file(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/config.json");

        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            QString jsonString = QString(R"({
                "CLIENT_ID": "%1",
                "CLIENT_SECRET": "%2",
                "ACCESS_TOKEN": "%3",
                "REFRESH_TOKEN": "%4",
                "EXPIRES_IN": %5,
                "TOKEN_OBTAINED": "%6"
            })")
            .arg(clientId,
                 clientSecret,
                 token,
                 refreshToken,
                 QString::number(expiresAt),
                 tokenObtained.toString(Qt::ISODate)
            );

            file.write(jsonString.toUtf8());
            file.close();
        }
    }

    QString clientId;
    QString clientSecret;
    QString accessToken;
    QString refreshToken;
    int expiresAt;
    QDateTime tokenObtained;

private:
    Config() {
        QString dirPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
        QString filePath = dirPath + "/config.json";

        if (!QDir().exists(dirPath)) {
            qDebug() << "creaded dir";
            QDir().mkpath(dirPath);
        }
        if (!QFile::exists(filePath)) {
            qDebug() << "created file";
            QFile::copy(":/config.json", filePath);
        }

        QFile file(filePath);
        QFile config(":/config.json");

        if (!file.open(QIODevice::ReadOnly) || !config.open(QIODevice::ReadOnly)) {
            qWarning() << "Cannot open config.json resource!";
            return;
        }

        QJsonObject object = QJsonDocument::fromJson(file.readAll()).object();
        QJsonObject clientData = QJsonDocument::fromJson(config.readAll()).object();

        clientId = clientData["CLIENT_ID"].toString();
        clientSecret = clientData["CLIENT_SECRET"].toString();
        accessToken = clientData["ACCESS_TOKEN"].toString();
        refreshToken = object["REFRESH_TOKEN"].toString();
        expiresAt = object["EXPIRES_IN"].toInt();
        tokenObtained = QDateTime::fromString(object["TOKEN_OBTAINED"].toString(), Qt::ISODate);
    }

    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;
};

#endif // CONFIG_H
