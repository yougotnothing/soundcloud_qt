#include "trackservice.h"
#include "config.h"
#include "data/consts.h"
#include "structs/track.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QProcess>

TrackService::TrackService(QObject *parent) : QObject(parent) {
  player = new QMediaPlayer(this);
  audioOutput = new QAudioOutput(this);

  player->setAudioOutput(audioOutput);
  audioOutput->setVolume(0.3);

  connect(player, &QMediaPlayer::positionChanged, this,
          &TrackService::positionChanged);
  connect(player, &QMediaPlayer::durationChanged, this,
          &TrackService::durationChanged);
}

Track TrackService::parseTrack(const QByteArray &data) {
  Track track;
  const QJsonDocument parsedData = QJsonDocument::fromJson(data);
  QJsonArray object = parsedData.array();
  QJsonObject trackData = object[0].toObject();

  qDebug() << object[0];

  if (object.isEmpty()) {
    qWarning() << "No tracks found";
    return track;
  }

  track.id = trackData["id"].toInteger();
  track.artworkUrl = trackData["artwork_url"].toString();
  track.uri = trackData["uri"].toString();
  track.createdAt = trackData["created_at"].toString();
  trackData["bpm"].isNull() ? track.bpm = std::nullopt
                            : track.bpm = trackData["bpm"].toInt();
  track.commentCount = trackData["comment_count"].toInt();
  track.commentable = trackData["commentable"].toBool();
  track.favoritingsCount = trackData["favoritings_count"].toInt();
  track.playbackCount = trackData["playback_count"].toInt();
  track.releaseDay = trackData["release_day"].toInt();
  track.releaseMonth = trackData["release_month"].toInt();
  track.title = trackData["title"].toString();
  track.labelName = trackData["label_name"].toString();

  qDebug() << "track Id:" << track.id;

  resolveStream(track.id);

  emit trackSetted(this->track);

  return track;
}

void TrackService::setTrack(const Track &data) {
  this->track = data;
  emit trackSetted(track);
}

void TrackService::findTrack(const QString &name) {
  QNetworkRequest request(
      QUrl(SOUNDCLOUD_URL + "/tracks?q=" + name + "&limit=2"));
  request.setRawHeader(
      "Authorization",
      QString("OAuth " + Config::instance().accessToken).toUtf8());

  QNetworkReply *reply = manager.get(request);

  connect(reply, &QNetworkReply::finished, [this, reply]() {
    const QByteArray data = reply->readAll();

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

void TrackService::resolveStream(qint64 trackId) {
  QString url =
      QString("https://api.soundcloud.com/tracks/%1/streams").arg(trackId);
  QNetworkRequest request((QUrl(url)));

  request.setRawHeader(
      "Authorization",
      QString("OAuth %1").arg(Config::instance().accessToken).toUtf8());

  QNetworkReply *reply = manager.get(request);

  connect(reply, &QNetworkReply::finished, this, [this, reply, trackId]() {
    if (reply->error() == QNetworkReply::NoError) {

      QJsonObject response = QJsonDocument::fromJson(reply->readAll()).object();

      if (response["hls_aac_160_url"].isString()) {
        this->track.streamUrl = response["hls_aac_160_url"].toString();
      }

      if (this->track.streamUrl.isEmpty() &&
          response["hls_aac_96_url"].isString()) {
        this->track.streamUrl = response["hls_aac_96_url"].toString();
      }

      if (this->track.streamUrl.isEmpty() &&
          response["http_mp3_128_url"].isString()) {
        this->track.streamUrl = response["http_mp3_128_url"].toString();
      }

      if (this->track.streamUrl.isEmpty()) {
        qWarning() << "No playable stream found";
        qDebug() << "Streams response:" << response;
        reply->deleteLater();
        return;
      }

      playFromUrl(this->track.streamUrl, trackId);
      reply->deleteLater();
    } else {
      qWarning() << reply->errorString();
    }
  });
}

void TrackService::writeSongFile(const QString &url, const qint64 &trackId) {
  auto *process = new QProcess(this);
  QString appDataPath =
      QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
  QDir().mkpath(appDataPath);
  QString output = appDataPath + QString("/%1.m4a").arg(trackId);
  QString auth = QString("OAuth %1").arg(Config::instance().accessToken);
  QStringList args = {"-headers", QString("Authorization: %1\r\n").arg(auth),
                      "-i",       url,
                      "-c:a",     "copy",
                      "-y",       output};

  connect(process, &QProcess::readyReadStandardError, this, [process]() {
    qDebug().noquote() << process->readAllStandardError();
  });

  connect(process, qOverload<int, QProcess::ExitStatus>(&QProcess::finished),
          this,
          [this, process, output, trackId](int exitCode,
                                           QProcess::ExitStatus status) {
            if (status == QProcess::NormalExit && exitCode == 0) {
              qDebug() << "Song saved:" << output;

              playFromUrl(output, trackId);
            } else {
              qWarning() << "FFmpeg failed:" << exitCode;
            }

            process->deleteLater();
          });

  process->start("ffmpeg", args);
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

  emit togglePlay(player->isPlaying());
}

void TrackService::playFromUrl(const QString &url, const qint64 &trackId) {
  qDebug() << "Play URL:" << url;

  player->setSource(QUrl::fromLocalFile(url));
  player->play();

  emit togglePlay(player->isPlaying());
}

void TrackService::play() {
  player->play();
  emit togglePlay(player->isPlaying());
}

void TrackService::pause() {
  player->pause();
  emit togglePlay(player->isPlaying());
}

void TrackService::stop() const { player->stop(); }

void TrackService::seek(const qint64 position) const {
  player->setPosition(position);
}

void TrackService::setVolume(float volume) const {
  audioOutput->setVolume(volume);
}
