import QtQuick
import QtQuick.Controls

Item {
    id: playlistRoot
    width: 360
    height: 420

    Text {
        id: emptyMessage
        anchors.centerIn: parent
        visible: playlistsService.playlists.length === 0
        text: "No playlists loaded"
        color: "white"
        font.pixelSize: 16
    }

    ListView {
        anchors.fill: parent
        spacing: 5
        bottomMargin: 12
        model: playlistsService.playlists

        delegate: Row {
            width: parent.width
            height: 80
            spacing: 10

            Rectangle {
                width: 70; height: 70
                radius: 8
                clip: true

                Image {
                    anchors.fill: parent
                    source: modelData.artworkUrl
                    fillMode: Image.PreserveAspectCrop
                }
            }

            Column {
                spacing: 5
                Text { text: modelData.title; color: "white" }
            }
        }
    }
}