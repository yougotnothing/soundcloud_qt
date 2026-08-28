#ifndef TRACKSERVICE_H
#define TRACKSERVICE_H

#include <QAudioOutput>
#include <QMediaPlayer>
#include <QNetworkAccessManager>
#include <QObject>
#include <QTemporaryFile>

#include "structs/track.h"

class TrackService : public QObject {
  Q_OBJECT
  Q_PROPERTY(qint64 position READ position NOTIFY positionChanged)
  Q_PROPERTY(qint64 duration READ duration NOTIFY durationChanged)

public:
  Q_INVOKABLE Track track;
  explicit TrackService(QObject *parent = nullptr);
  Q_INVOKABLE void play();
  Q_INVOKABLE void pause();
  Q_INVOKABLE void stop() const;
  Q_INVOKABLE void seek(qint64 position) const;
  Q_INVOKABLE void setVolume(float volume) const;
  Q_INVOKABLE void findTrack(const QString &name);
  Q_INVOKABLE void resolveStream(qint64 trackId);
  bool paused{};
  int volume{};

  [[nodiscard]] qint64 position() const { return player->position(); };
  [[nodiscard]] qint64 duration() const { return player->duration(); };
  [[nodiscard]] QString streamUrl() const { return track.streamUrl; };
  [[nodiscard]] QString artworkUrl() const { return track.artworkUrl; };
  [[nodiscard]] QString labelName() const { return track.labelName; };
  [[nodiscard]] QString title() const { return track.title; };
  [[nodiscard]] bool isPlaying() const { return player->isPlaying(); };

  Q_PROPERTY(bool isPlaying READ isPlaying NOTIFY togglePlay);
  Q_PROPERTY(QString streamUrl READ streamUrl NOTIFY trackSetted);
  Q_PROPERTY(QString artworkUrl READ artworkUrl NOTIFY trackSetted);
  Q_PROPERTY(QString title READ title NOTIFY trackSetted);
  Q_PROPERTY(QString labelName READ labelName NOTIFY trackSetted);

signals:
  void trackSetted(const Track &data);
  void trackFound(const Track &data);
  void positionChanged(qint64 position);
  void durationChanged(qint64 duration);
  void playing();
  void togglePlay(bool playing);

private:
  QMediaPlayer *player;
  QAudioOutput *audioOutput;
  QTemporaryFile *tempFile = nullptr;
  QNetworkAccessManager manager;
  Track parseTrack(const QByteArray &data);
  void setTrack(const Track &data);
  void playFromUrl(const QString &url, const qint64 &trackId);
  void playFromBuffer(const QByteArray &data);
  void writeSongFile(const QString &url, const qint64 &trackId);
};

#endif // TRACKSERVICE_H
