#include "userservice.h"
#include "structs/user.h"
#include "data/consts.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QObject>

UserService::UserService(QObject *parent) : QObject(parent) {}

void UserService::getUser() {
    const QNetworkRequest request(QUrl(SOUNDCLOUD_URL + "/me"));
    QNetworkReply *reply = manager.get(request);

    connect(reply, &QNetworkReply::finished, [this, reply]() {
        const QByteArray data = reply->readAll();

        qDebug() << "User data:" << data;

        emit gotUser();

        reply->deleteLater();
    });
}
