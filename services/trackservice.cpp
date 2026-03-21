#include "trackservice.h"
#include "data/consts.h"
#include "structs/track.h"
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "authservice.h"
#include "config.h"

TrackService::TrackService(QObject *parent) : QObject(parent), authService(this) {
    player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);

    player->setAudioOutput(audioOutput);
    audioOutput->setVolume(0.3);

    connect(player, &QMediaPlayer::positionChanged, this, &TrackService::positionChanged);
    connect(player, &QMediaPlayer::durationChanged, this, &TrackService::durationChanged);
}

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

    track.id = trackData["id"].toInt();
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
    track.title = trackData["title"].toString();

    resolveStream(trackData["id"].toInt());

    emit trackSetted(this->track);

    return track;
}

void TrackService::setTrack(const Track &data) {
    this->track = data;
    emit trackSetted(track);
}

void TrackService::findTrack(QString name) {
    QNetworkRequest request(QUrl(SOUNDCLOUD_URL + "/tracks?q=" + name + "&limit=2"));
    request.setRawHeader("Authorization", QString("OAuth " + Config::instance().accessToken).toUtf8());

    QNetworkReply *reply = manager.get(request);

    connect(reply, &QNetworkReply::finished, [this, reply]() {
        QByteArray data = reply->readAll();
        QJsonObject object = QJsonDocument::fromJson(data).object();

        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << data;

            this->setTrack(this->parseTrack(data));

            emit trackFound(track);
        } else {
            qWarning() << reply->errorString();
        }

        reply->deleteLater();
    });
}

void TrackService::resolveStream(int trackId) {
    QString url = QString("https://api.soundcloud.com/tracks/%1/streams").arg(trackId);
    QNetworkRequest request((QUrl(url)));

    request.setRawHeader("Authorization", QString("OAuth %1").arg(Config::instance().accessToken).toUtf8());

    QNetworkReply *reply = manager.get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        QJsonObject response = QJsonDocument::fromJson(reply->readAll()).object();

        this->track.streamUrl = response["http_mp3_128_url"].toString();
        writeSongFile(response["http_mp3_128_url"].toString());
        reply->deleteLater();
    });
}

void TrackService::writeSongFile(QString url) {
    QNetworkRequest request((QUrl(url)));

    request.setRawHeader("Authorization", QString("OAuth %1").arg(Config::instance().accessToken).toUtf8());

    QNetworkReply *reply = manager.get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();

            playFromBuffer(data);
        } else {
            qWarning() << reply->errorString();
        }
    });
}

void TrackService::playFromBuffer(const QByteArray &data) {
    if (tempFile) {
        tempFile->close();
        delete tempFile;
        tempFile = nullptr;
    }

    tempFile = new QTemporaryFile(this);

    if (!tempFile->open()) {
        qWarning() << "Temp file open failed";
        return;
    }

    tempFile->write(data);
    tempFile->flush();

    qDebug() << "Play from file:" << tempFile->fileName();

    player->setSource(QUrl::fromLocalFile(tempFile->fileName()));
    player->play();
}

void TrackService::playFromUrl(const QString &url) {
    qDebug() << "Play URL:" << url;

    player->setSource(QUrl(url));
    player->play();
}

void TrackService::play() {
    player->play();
}

void TrackService::pause() {
    player->pause();
}

void TrackService::stop() {
    player->stop();
}

void TrackService::seek(qint64 pos) {
    player->setPosition(pos);
}

void TrackService::setVolume(float volume) {
    audioOutput->setVolume(volume);
}
