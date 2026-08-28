#ifndef SOUNDCLOUD_QT_PLAYLISTSSERVICE_H
#define SOUNDCLOUD_QT_PLAYLISTSSERVICE_H
#include "structs/track.h"
#include <QNetworkAccessManager>
#include <QObject>
#include <QQmlListProperty>

class PlaylistsService : public QObject {
  Q_OBJECT
  Q_PROPERTY(QVariantList playlists READ playlists NOTIFY playlistsGot)
  Q_PROPERTY(QString urn READ urn NOTIFY currentPlaylistChanged)
  Q_PROPERTY(QString title READ title NOTIFY currentPlaylistChanged)
  Q_PROPERTY(QString artworkUrl READ artworkUrl NOTIFY currentPlaylistChanged)
  Q_PROPERTY(QVariantList currentPlaylistSongs READ currentPlaylistSongs NOTIFY
                 currentPlaylistChanged)

public:
  explicit PlaylistsService(QObject *parent = nullptr);

  void getPlaylistsPreview();
  void getPlaylistData(const QJsonObject &data);
  Q_INVOKABLE void getPlaylistSongs(const QString &urn);
  static void parsePlaylists(QByteArray data, QVariantList &playlists);

  [[nodiscard]] QVariantList playlists() const { return m_playlists; }
  QVariantList currentPlaylistSongs() const { return m_currentPlaylistSongs; }
  QString urn() const { return m_urn; }
  QString title() const { return m_title; }
  QString artworkUrl() const { return m_artworkUrl; }

signals:
  void playlistsGot();
  void currentPlaylistChanged();

private:
  void parseCurrentPlaylistSongs(const QJsonObject &playlistData,
                                 QVariantList &playlist);
  QVariantList m_playlists;
  QVariantList m_currentPlaylistSongs;
  QString m_urn;
  QString m_artworkUrl;
  QString m_title;
  QNetworkAccessManager manager;
};

#endif // SOUNDCLOUD_QT_PLAYLISTSSERVICE_H
