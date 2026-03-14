#ifndef USERSERVICE_H
#define USERSERVICE_H

#include <QString>
#include <QObject>
#include "structs/user.h"
#include <QNetworkAccessManager>

class UserService : QObject {
    Q_OBJECT

    public:
        explicit UserService(QObject *parent = nullptr);
        User user;
        void getUser();
        void changeName(QString name);

    signals:
        void gotUser();
        void changedName();

    private:
        QNetworkAccessManager manager;
};

#endif // USERSERVICE_H
