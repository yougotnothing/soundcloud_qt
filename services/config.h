#ifndef CONFIG_H
#define CONFIG_H

#include <QJsonObject>
#include <QFile>
#include <QIoDevice>
#include <QDebug>

class Config {
public:
    static Config& instance() {
        static Config cfg;
        return cfg;
    }

    void setCredentials(QJsonObject data) {
        QFile file(":/config.json");

        if(file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            QString jsonString = QString(R"({
                "CLIENT_ID": "%1",
                "CLIENT_SECRET": "%2",
                "ACCESS_TOKEN": "%3",
                "REFRESH_TOKEN": "%4",
                "EXPIRES_IN": %5,
                "TOKEN_OBTAINED": %6
            })")
            .arg(clientId,
                 clientSecret,
                 data["access_token"].toString(),
                 data["refresh_token"].toString(),
                 QString::number(data["expires_in"].toInt()),
                 QDateTime::currentDateTimeUtc().toString()
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
        QFile file(":/config.json");

        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "Cannot open config.json resource!";
            return;
        }

        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonObject object = doc.object();

        clientId = object["CLIENT_ID"].toString();
        clientSecret = object["CLIENT_SECRET"].toString();
        accessToken = object["ACCESS_TOKEN"].toString();
        refreshToken = object["REFRESH_TOKEN"].toString();
        expiresAt = object["EXPIRES_AT"].toInt();
        tokenObtained = QDateTime::fromString(object["TOKEN_OBTAINED"].toString(), Qt::ISODate);
    }

    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;
};

#endif // CONFIG_H
