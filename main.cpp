#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QJsonObject>
#include <QDebug>
#include "services/trackservice.h"
#include "services/authservice.h"
#include "servers/oauthserver.h"
#include "services/PlaylistsService.h"

int main(int argc, char *argv[])
{
    const QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    TrackService trackService;
    AuthService authService;
    const OAuthServer oAuthServer;
    PlaylistsService playlistsService;

    QObject::connect(&authService, &AuthService::hasNoRefreshToken, [&] {
        authService.startOAuthFlow();
    });

    QObject::connect(&oAuthServer, &OAuthServer::codeRecieved, [&](const QString& code) {
        authService.exchangeCodeForToken(code);
    });

    QObject::connect(&authService, &AuthService::tokenReady, [&]{
        qDebug() << "authService.tokenReady";
        playlistsService.getPlaylistsPreview();
    });

    authService.init();

    engine.rootContext()->setContextProperty("trackService", &trackService);
    engine.rootContext()->setContextProperty("authService", &authService);
    engine.rootContext()->setContextProperty("playlistsService", &playlistsService);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("soundcloud_qt", "Main");

    return app.exec();
}
