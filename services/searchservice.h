#ifndef SEARCHSERVICE_H
#define SEARCHSERVICE_H

#include "utils/searchtype.h"
#include <QNetworkAccessManager>
#include <QObject>

class SearchService : public QObject {
  Q_OBJECT

public:
  explicit SearchService(QObject *parent = nullptr);
  Q_INVOKABLE bool dropdownOpened;
  Q_INVOKABLE void setOpened(bool value);
  Q_INVOKABLE QList<QJsonObject> data;
  Q_INVOKABLE QVariantList testData;
  Q_INVOKABLE void setList(QList<QJsonObject> &data);
  Q_INVOKABLE void search(QString query, SearchType &type);

signals:
  void toggleDropdown();
  void listSetted();

private:
  QNetworkAccessManager manager;
};

#endif // SEARCHSERVICE_H
