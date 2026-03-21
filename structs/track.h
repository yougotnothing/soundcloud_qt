#ifndef TRACK_H
#define TRACK_H

#include <QString>
#include <optional>

struct Track {
    std::optional<QString> description;
    QString artworkUrl;
    QString createdAt;
    QString downloadUrl;
    QString genre;
    QString urn;
    QString kind;
    QString streamUrl;
    QString title;
    QString uri;
    std::optional<int> bpm;
    int commentCount;
    int duration;
    int favoritingsCount;
    int playbackCount;
    int releaseDay;
    int releaseMonth;
    int releaseYear;
    int repostsCount;
    int userPlaybackCount;
    int id;
    bool commentable;
    bool downloadable;
    bool streamable;
    bool userFavorite;
};

#endif
