#ifndef SOUNDCLOUD_QT_PLAYLISTSSERVICE_H
#define SOUNDCLOUD_QT_PLAYLISTSSERVICE_H
#include <QNetworkAccessManager>
#include <QObject>
#include <QQmlListProperty>

#include "authservice.h"
#include "structs/Playlist.h"

class PlaylistsService : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantList playlists READ playlists NOTIFY playlistsGot)

    public:
        explicit PlaylistsService(QObject *parent = nullptr);

        void getPlaylistsPreview();

        [[nodiscard]] QVariantList playlists() const { return m_playlists; }

    signals:
        void playlistsGot();

    private:
        QVariantList m_playlists;
        QNetworkAccessManager manager;
        AuthService authService;
};


#endif //SOUNDCLOUD_QT_PLAYLISTSSERVICE_H