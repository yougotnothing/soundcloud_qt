#include "userservice.h"
#include "structs/user.h"
#include "data/consts.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QObject>

UserService::UserService(QObject *parent) : QObject(parent) {}

void UserService::getUser() {
    QNetworkRequest request(QUrl(SOUNDCLOUD_URL + "/me"));
    QNetworkReply *reply = manager.get(request);

    connect(reply, &QNetworkReply::finished, [this, reply]() {
        QByteArray data = reply->readAll();

        emit gotUser();

        reply->deleteLater();
    });
}
