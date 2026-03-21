#ifndef OAUTHSERVER_H
#define OAUTHSERVER_H

#include <QObject>
#include <QTcpServer>

class OAuthServer : public QObject {
    Q_OBJECT

    public:
        QString code;
        QTcpServer server;
        explicit OAuthServer(QObject *parent = nullptr);
    signals:
        void codeRecieved(QString code);
};

#endif // OAUTHSERVER_H
