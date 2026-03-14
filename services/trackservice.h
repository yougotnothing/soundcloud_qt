#ifndef TRACKSERVICE_H
#define TRACKSERVICE_H

#include <QObject>
#include <QNetworkAccessManager>
#include "structs/track.h"
#include "authservice.h"

class TrackService : public QObject {
    Q_OBJECT

    public:
        Q_INVOKABLE Track track;
        explicit TrackService(QObject *parent = nullptr);
        Q_INVOKABLE void play();
        Q_INVOKABLE void findTrack(QString name);
        bool paused;
        int volume;

        QString streamUrl() const { return track.streamUrl; }

        Q_PROPERTY(QString streamUrl READ streamUrl NOTIFY trackSetted);

    signals:
        void trackSetted(const Track &data);
        void trackFound(const Track &data);
        void playing();

    private:
        QNetworkAccessManager manager;
        Track parseTrack(QByteArray data);
        void setTrack(const Track &data);
        AuthService authService;
        void resolveStream(QString previewUrl);
};

#endif // TRACKSERVICE_H
