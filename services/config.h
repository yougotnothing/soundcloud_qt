#ifndef CONFIG_H
#define CONFIG_H

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QIoDevice>
#include <QJsonObject>
#include <QSettings>
#include <QStandardPaths>
#include <QtCore/qtypes.h>

class Config {
public:
  static Config &instance() {
    static Config cfg;
    return cfg;
  }

  void setCredentials(QJsonObject data) {
    setAccessToken(data["access_token"].toString());
    setRefreshToken(data["refresh_token"].toString());
    setExpiresAt(QDateTime::currentDateTime().addSecs(3600).toSecsSinceEpoch());

    tokenObtained = QDateTime::currentDateTime().toSecsSinceEpoch();
    settings.setValue("auth/tokenObtained", tokenObtained);
  }

  void setAccessToken(const QString &token) {
    accessToken = token;

    settings.setValue("auth/accessToken", token);
  }

  void setExpiresAt(const qint64 &dateTime) {
    expiresAt = dateTime;

    settings.setValue("auth/expiresIn", dateTime);
  }

  void setRefreshToken(const QString &token) {
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

  Config(const Config &) = delete;
  Config &operator=(const Config &) = delete;
};

#endif // CONFIG_H
