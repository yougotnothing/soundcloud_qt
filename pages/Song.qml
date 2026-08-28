import QtQuick
import QtQuick.Controls

Page {
  title: trackService.title

  Column {
    spacing: 12


    Row {
      spacing: 8

      Rectangle {
        width: 240; height: 240
        radius: 12

        Image {
          source: trackService.artworkUrl
          anchors.fill: parent
        }
      }

      Column {
        Button {
          text: qsTr(trackService.labelName)
        }
      }
    }
  }
}
