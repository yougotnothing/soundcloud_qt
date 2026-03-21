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

        void startOAuthFlow();
        void exchangeCodeForToken(QString code);

        QString accessToken;

    protected:
        QString token;

    signals:
        void authorized();
        void tokenReady(QString token);

    private:
        void setToken(QString token, TokenType type);
        QString refreshToken;
        QNetworkAccessManager manager;
        QString m_accessToken;
};

#endif // AUTHSERVICE_H
