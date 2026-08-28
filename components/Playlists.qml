import QtQuick
import QtQuick.Controls

Item {
    id: playlistRoot
    anchors.fill: parent

    Text {
        id: emptyMessage
        anchors.centerIn: parent
        visible: playlistsService.playlists.length === 0
        text: "No playlists loaded"
        color: "white"
        font.pixelSize: 16
    }

    ListView {
        height: parent.height
        width: 260
        spacing: 5
        model: playlistsService.playlists
        cacheBuffer: 1000
        reuseItems: true
        boundsBehavior: Flickable.StopAtBounds
        clip: true

        delegate: Button {
            width: parent.width
            height: 80
            spacing: 10
            onClicked: () => {
                Router.go("qrc:/qt/qml/soundcloud_qt/pages/Playlist.qml", { playlistId: modelData.urn })
            }

            Row {
                height: 80
                width: 260

                Rectangle {
                    width: 80
                    height: 80
                    radius: 4
                    color: "transparent"
                    clip: true
                    antialiasing: true

                    Image {
                        anchors.fill: parent
                        source: modelData.artworkUrl
                        fillMode: Image.PreserveAspectCrop
                    }
                }

                Column {
                    spacing: 5

                    Text { 
                        text: modelData.title;
                        color: "white"
                        width: 160
                        wrapMode: Text.NoWrap
                        elide: Text.ElideRight
                        horizontalAlignment: Text.AlignLeft
                    }
                }
            }
        }
    }
}
