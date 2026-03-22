#ifndef SOUNDCLOUD_QT_PLAYLIST_H
#define SOUNDCLOUD_QT_PLAYLIST_H
#include <QString>
#include <QJsonObject>

class PlaylistPreview : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString urn READ urn CONSTANT)
    Q_PROPERTY(QString title READ title CONSTANT)
    Q_PROPERTY(QString artworkUrl READ artworkUrl CONSTANT)

public:
    explicit PlaylistPreview(const QJsonObject& obj, QObject* parent = nullptr) : QObject(parent) {
        m_urn = obj.value("urn").toString();
        m_title = obj.value("title").toString();
        m_artworkUrl = obj.value("artwork_url").toString();
    }

    void fromJson(const QJsonObject &obj) {
        m_urn = obj.value("urn").toString();
        m_title = obj.value("title").toString();
        m_artworkUrl = obj.value("artwork_url").toString();
    }

    [[nodiscard]] QString urn() const { return m_urn; }
    [[nodiscard]] QString title() const { return m_title; }
    [[nodiscard]] QString artworkUrl() const { return m_artworkUrl; }

private:
    QString m_urn;
    QString m_title;
    QString m_artworkUrl;
};

#endif //SOUNDCLOUD_QT_PLAYLIST_H