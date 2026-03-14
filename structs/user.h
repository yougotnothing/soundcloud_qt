#include <QString>
#include <QList>
#include <QObject>

#ifndef USER_H
#define USER_H

struct UserQuota {
    bool unlimitedUploadQuota;
    int uploadSecondsUsed;
    int uploadSecondsLeft;
};

struct UserSubscriptionProduct {
    QString id;
    QString name;
};

struct UserSubscription {
    UserSubscriptionProduct product;
    bool recurring;
};

struct User {
    QString avatarUrl;
    QString city;
    QString country;
    QString createdAt;
    QString description;
    QString doscogsName;
    QString firstName;
    int followersCount;
    int followingsCount;
    QString fullName;
    QString urn;
    QString kind;
    QString lastModified;
    QString lastName;
    int likesCount;
    QString locale;
    bool online;
    QString permalink;
    QString permalinkUrl;
    QString plan;
    int playlistCount;
    bool primaryEmailConfirmed;
    int privatePlaylistsCount;
    int privateTracksCount;
    int publicFavoritesCount;
    UserQuota quota;
    int repostsCount;
    QList<UserSubscription> subscriptions;
    int trackCount;
    int uploadSecondsLeft;
    QString uri;
    QString username;
    QString website;
    QString websiteTitle;
};

#endif // USER_H
