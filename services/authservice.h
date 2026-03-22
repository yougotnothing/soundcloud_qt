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
        void init();
        void startOAuthFlow();
        void exchangeCodeForToken(const QString& code);

        QString accessToken;

    protected:
        QString token;

    signals:
        void authorized();
        void tokenReady(QString token);
        void hasRefreshToken(QString refreshToken);
        void hasNoRefreshToken();

    private:
        void setToken(const QString& token, TokenType type);
        QString refreshToken;
        QNetworkAccessManager manager;
        QString m_accessToken;
};

#endif // AUTHSERVICE_H
