#ifndef TRACKSERVICE_H
#define TRACKSERVICE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QTemporaryFile>

#include "structs/track.h"
#include "authservice.h"

class TrackService : public QObject {
    Q_OBJECT
    Q_PROPERTY(qint64 position READ position NOTIFY positionChanged)
    Q_PROPERTY(qint64 duration READ duration NOTIFY durationChanged)

    public:
        Q_INVOKABLE Track track;
        explicit TrackService(QObject *parent = nullptr);
        Q_INVOKABLE void play();
        Q_INVOKABLE void pause();
        Q_INVOKABLE void stop();
        Q_INVOKABLE void seek(qint64 position);
        Q_INVOKABLE void setVolume(float volume);
        Q_INVOKABLE void findTrack(QString name);
        bool paused;
        int volume;

        qint64 position() const { return player->position(); };
        qint64 duration() const { return player->duration(); };

        QString streamUrl() const { return track.streamUrl; };
        QString artworkUrl() const { return track.artworkUrl; };
        QString title() const { return track.title; };

        Q_PROPERTY(QString streamUrl READ streamUrl NOTIFY trackSetted);
        Q_PROPERTY(QString artworkUrl READ artworkUrl NOTIFY trackSetted);
        Q_PROPERTY(QString title READ title NOTIFY trackSetted);

    signals:
        void trackSetted(const Track &data);
        void trackFound(const Track &data);
        void positionChanged(qint64 position);
        void durationChanged(qint64 duration);
        void playing();

    private:
        QMediaPlayer *player;
        QAudioOutput *audioOutput;
        QTemporaryFile *tempFile = nullptr;
        QNetworkAccessManager manager;
        Track parseTrack(QByteArray data);
        void setTrack(const Track &data);
        AuthService authService;
        void resolveStream(int trackId);
        void playFromUrl(const QString &url);
        void playFromBuffer(const QByteArray &data);
        void writeSongFile(QString url);
};

#endif // TRACKSERVICE_H
