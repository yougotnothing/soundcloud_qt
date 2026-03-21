#include "oauthserver.h"
#include <QTcpSocket>
#include <QTcpServer>
#include <QObject>
#include <QUrlQuery>
#include <QDebug>

OAuthServer::OAuthServer(QObject *parent) : QObject(parent) {
    if (!server.listen(QHostAddress::LocalHost, 1212)) {
        qWarning() << "Cannot start server!";
        return;
    }

    connect(&server, &QTcpServer::newConnection, this, [this]() {
        QTcpSocket* socket = server.nextPendingConnection();

        connect(socket, &QTcpSocket::readyRead, [this, socket]() {
            QByteArray request = socket->readAll();
            QString reqStr(request);

            qDebug() << "Request:" << reqStr;

            if (reqStr.startsWith("GET /callback?")) {
                QString query = reqStr.split(" ")[1].mid(strlen("/callback?"));
                QUrlQuery q(query);
                code = q.queryItemValue("code");
                qDebug() << "OAuth Code recieved:" << code;

                emit codeRecieved(code);

                QByteArray response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
                                      "<html><body>You can close this window now.</body></html>";

                socket->write(response);
                socket->flush();
                socket->disconnectFromHost();
            }
        });
    });

    qDebug() << "Server listening at http://localhost:1212/callback";
}
