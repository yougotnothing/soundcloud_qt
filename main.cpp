#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include "services/trackservice.h"
#include "services/authservice.h"
#include "services/config.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    TrackService trackService;
    AuthService authService;

    if (QDateTime::currentDateTimeUtc() > Config::instance().tokenObtained.addSecs(Config::instance().expiresAt)) {
        authService.auth();
    }


    engine.rootContext()->setContextProperty("trackService", &trackService);
    engine.rootContext()->setContextProperty("authService", &authService);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("soundcloud_qt", "Main");

    return app.exec();
}
