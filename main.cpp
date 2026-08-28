#include "servers/oauthserver.h"
#include "services/PlaylistsService.h"
#include "services/authservice.h"
#include "services/config.h"
#include "services/searchservice.h"
#include "services/trackservice.h"
#include "services/userservice.h"
#include <QDebug>
#include <QGuiApplication>
#include <QJsonObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtCore/qlogging.h>
#include <QtQml/qqml.h>

int main(int argc, char *argv[]) {
  const QGuiApplication app(argc, argv);
  QQmlApplicationEngine engine;
  TrackService trackService;
  AuthService authService;
  const OAuthServer oAuthServer;
  PlaylistsService playlistsService;
  SearchService searchService;
  UserService userService;

  QObject::connect(&authService, &AuthService::hasNoRefreshToken, [&] {
    qDebug() << "Token Obtained" << Config::instance().tokenObtained;
    qDebug() << "Expires At" << Config::instance().expiresAt;
    qDebug() << "Time Now" << QDateTime::currentDateTime().toSecsSinceEpoch();

    authService.startOAuthFlow();
  });

  QObject::connect(
      &oAuthServer, &OAuthServer::codeRecieved,
      [&](const QString &code) { authService.exchangeCodeForToken(code); });

  QObject::connect(&authService, &AuthService::tokenReady, [&] {
    qDebug() << "Token Obtained" << Config::instance().tokenObtained;
    qDebug() << "Expires At" << Config::instance().expiresAt;
    qDebug() << "Time Now" << QDateTime::currentDateTime().toSecsSinceEpoch();

    qDebug() << "authService.tokenReady";
    playlistsService.getPlaylistsPreview();
    userService.getFeed();
  });

  authService.init();

  engine.rootContext()->setContextProperty("trackService", &trackService);
  engine.rootContext()->setContextProperty("authService", &authService);
  engine.rootContext()->setContextProperty("searchService", &searchService);
  engine.rootContext()->setContextProperty("playlistsService",
                                           &playlistsService);

  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
      []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
  engine.loadFromModule("soundcloud_qt", "Main");

  return app.exec();
}
