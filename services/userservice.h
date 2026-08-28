#ifndef USERSERVICE_H
#define USERSERVICE_H

#include "structs/user.h"
#include <QNetworkAccessManager>
#include <QObject>
#include <QString>

class UserService : QObject {
  Q_OBJECT

public:
  explicit UserService(QObject *parent = nullptr);
  User user;
  void getUser();
  void changeName(QString name);
  void getFeed();

signals:
  void gotUser();
  void gotFeed();
  void changedName();

private:
  QNetworkAccessManager manager;
};

#endif // USERSERVICE_H
