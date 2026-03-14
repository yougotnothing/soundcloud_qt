#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H

#include <QString>
#include <QObject>
#include <QNetworkAccessManager>

enum TokenType {
    ACCESS,
    REFRESH
};

class AuthService : public QObject {
    Q_OBJECT

    public:
        explicit AuthService(QObject *parent = nullptr);
        void refresh();
        void auth();
        QString accessToken;

    protected:
        QString token;

    signals:
        void authorized();

    private:
        void setToken(QString token, TokenType type);
        QString refreshToken;
        QNetworkAccessManager manager;
};

#endif // AUTHSERVICE_H
