import QtQuick
import QtMultimedia
import QtQuick.Controls
import QtQuick.Layouts

Item {
  Rectangle {
    anchors.fill: parent
    color: "#181818"
    radius: 24
    anchors.margins: 12
    clip: true

    MediaPlayer {
      id: player
      source: trackService.streamUrl
      audioOutput: AudioOutput {
          id: audio
      }
    }

    RowLayout {
      width: parent.width
      spacing: 15
      Layout.margins: 12

      Column {
        Layout.preferredWidth: 320
        spacing: 10

        Rectangle {
          width: 50; height: 50; radius: 4
          color: "#131313"
          clip: true

          Image {
            source: trackService.artworkUrl
            sourceSize.width: 50
            sourceSize.height: 50
            width: 50
            height: 50
          }
        }

        Text {
          text: trackService.title
          color: "white"
          font.pixelSize: 18
          width: 240
          wrapMode: Text.NoWrap
          elide: Text.ElideRight
          horizontalAlignment: Text.AlignLeft
        }
      }

      Button {
        text: trackService.isPlaying ? "Pause" : "Play"

        onClicked: {
          if (trackService.isPlaying)
            trackService.pause()
          else
            trackService.play()
        }
      }

      Slider {
        id: progress
        Layout.preferredWidth: 250
        from: 0
        to: trackService.duration
        value: trackService.position
        onMoved: trackService.seek(value)
      }

      Text {
        text: Math.floor(trackService.position / 1000) + "s"
        color: "white"
      }

      Slider {
        id: volumeSlider
        Layout.preferredWidth: 80
        from: 0
        to: 1
        value: 0.2
        onMoved: trackService.setVolume(value)
      }
    }

    Connections {
      target: trackService
      function onPositionChanged() {
        progress.value = trackService.position
        progress.to = trackService.duration
      }

      function onTrackSetted() {
        player.source = trackService.streamUrl
        player.play()
        trackService.play()
      }
    }
  }
}
