#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include "services/trackservice.h"
#include "services/authservice.h"
#include "servers/oauthserver.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    TrackService trackService;
    AuthService authService;
    OAuthServer oAuthServer;

    authService.startOAuthFlow();

    engine.rootContext()->setContextProperty("trackService", &trackService);
    engine.rootContext()->setContextProperty("authService", &authService);

    QObject::connect(&oAuthServer, &OAuthServer::codeRecieved, [&](QString code) {
        authService.exchangeCodeForToken(code);
    });

    QObject::connect(&authService, &AuthService::tokenReady, [&](QString token){
        qDebug() << "Token is ready, now we can play tracks!";
    });

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("soundcloud_qt", "Main");

    return app.exec();
}
