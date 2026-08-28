import QtQuick
import QtQuick.Controls

Item {
  id: playlistRoot

  property string playlistId: ""

  Component.onCompleted: {
    playlistsService.getPlaylistSongs(playlistId)
  }

  anchors.fill: parent

  Column {
    anchors.fill: parent

    Row {
      spacing: 32

      Rectangle {
        width: 240
        height: 240
        radius: 4
        color: "transparent"
        clip: true
        antialiasing: true

        Image {
          anchors.fill: parent
          source: playlistsService.artworkUrl
          fillMode: Image.PreserveAspectCrop
          smooth: true
          layer.enabled: true
          layer.smooth: true
          layer.mipmap: true
        }
      }

      Text {
        text: playlistsService.title || "Loading..."
      }
    }

    ListView {
      width: 420
      height: 220
      spacing: 12
      model: playlistsService.currentPlaylistSongs
      delegate: Button {
        width: 420
        height: 64

        onClicked: {
          trackService.resolveStream(modelData.id)
        }

        Row {
          Image {
            height: 64
            width: 64
            source: modelData.artworkUrl
          }

          Text {
            text: modelData.title
          }
        }
      }
    }
  }
}
