#include "trackservice.h"
#include "data/consts.h"
#include "structs/track.h"
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "authservice.h"

TrackService::TrackService(QObject *parent) : QObject(parent), authService(this) {}

Track TrackService::parseTrack(QByteArray data) {
    Track track;
    QJsonDocument parsedData = QJsonDocument::fromJson(data);
    QJsonArray object = parsedData.array();
    QJsonObject trackData = object[0].toObject();

    qDebug() << object[0];

    if (object.isEmpty()) {
        qWarning() << "No tracks found";
        return track;
    }

    track.artworkUrl = trackData["artwork_url"].toString();
    track.uri = trackData["uri"].toString();
    track.createdAt = trackData["created_at"].toString();
    trackData["bpm"].isNull() ? track.bpm = std::nullopt : track.bpm = trackData["bpm"].toInt();
    track.commentCount = trackData["comment_count"].toInt();
    track.commentable = trackData["commentable"].toBool();
    track.favoritingsCount = trackData["favoritings_count"].toInt();
    track.playbackCount = trackData["playback_count"].toInt();
    track.releaseDay = trackData["release_day"].toInt();
    track.releaseMonth = trackData["release_month"].toInt();

    resolveStream(trackData["stream_url"].toString());

    emit trackSetted(this->track);

    return track;
}

void TrackService::setTrack(const Track &data) {
    this->track = data;
    emit trackSetted(track);
}

void TrackService::findTrack(QString name) {
    QNetworkRequest request(QUrl(SOUNDCLOUD_URL + "/tracks?q=" + name + "&limit=1"));
    request.setRawHeader("Authorization", QString("OAuth " + authService.accessToken).toUtf8());

    QNetworkReply *reply = manager.get(request);

    connect(reply, &QNetworkReply::finished, [this, reply]() {
        QByteArray data = reply->readAll();

        qWarning() << data;

        QJsonObject object = QJsonDocument::fromJson(data).object();

        this->setTrack(this->parseTrack(data));

        emit trackFound(track);

        reply->deleteLater();
    });
}

void TrackService::play() {
    paused = false;
    emit playing();
}

void TrackService::resolveStream(QString previewUrl) {
    QNetworkRequest request((QUrl(previewUrl)));
    request.setRawHeader("Authorization", QString("OAuth " + authService.accessToken).toUtf8());

    QNetworkReply *reply = manager.get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        QVariant redirect = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

        if (redirect.isValid()) {
            QString streamUrl = redirect.toUrl().toString();

            qDebug() << "Real Stream:" << streamUrl;

            this->track.streamUrl = streamUrl;

            emit trackSetted(this->track);
        }

        reply->deleteLater();
    });
}
