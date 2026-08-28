import QtQuick
import QtQuick.Layouts
import '../components' as Components

Item {
  property var window;

  MouseArea {
    anchors.fill: parent
    propagateComposedEvents: true

    property point clickPos

    onPressed: (mouse) => {
      if (mouse.button === Qt.LeftButton) {
        window.startSystemMove()
      }
    } 
  }

  Rectangle {
    id: navbar
    color: '#151515'
    width: parent.width
    height: parent.height
    z: 11
    topLeftRadius: 24
    topRightRadius: 24
    clip: true

    RowLayout {
      anchors.fill: parent
      spacing: 10
      anchors.leftMargin: 10
      anchors.rightMargin: 10
      z: 12

      RowLayout {
        spacing: 8
        Layout.alignment: Qt.AlignVCenter

        Rectangle {
          Layout.preferredWidth: 14; Layout.preferredHeight: 14; radius: 7
          color: "#ff5f57"
          z: 13

          MouseArea {
            anchors.fill: parent
            onClicked: () => {
              window.close();
            }
          }
        }

        Rectangle {
          Layout.preferredWidth: 14; Layout.preferredHeight: 14; radius: 7
          color: "#ffbd2e"
          z: 13

          MouseArea {
            anchors.fill: parent
            onClicked: () => {
              window.showMinimized();
            }
          }
        }

        Rectangle {
          Layout.preferredWidth: 14; Layout.preferredHeight: 14; radius: 7
          color: "#28c840"
          z: 13

          MouseArea {
            anchors.fill: parent
            onClicked: () => {
              if (window.visibility === Window.Maximized) {
                window.showNormal();
              } else window.showMaximized();
            }
          }
        }
      }

      Item { Layout.fillWidth: true }

      Components.Search {
        Layout.preferredWidth: 400
        Layout.alignment: Qt.AlignVCenter
      }

      Item { Layout.fillWidth: true }
    }
  }
}
