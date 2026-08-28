import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import './components' as Components
import './pages' as Pages

ApplicationWindow {
    id: window
    minimumWidth: 920
    minimumHeight: 640
    visible: true
    color: 'transparent'
    flags: Qt.FramelessWindowHint

    Rectangle {
        width: parent.width
        height: parent.height
        color: '#151515'
        radius: 24

        Components.Navbar {
            id: navbar
            anchors.top: parent.top
            anchors.right: parent.right
            height: 64
            width: parent.width
            window: window
            z:100
        }

        RowLayout {
            anchors.top: navbar.bottom
            anchors.bottom: player.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 20
            spacing: 20

            Components.Playlists {
                id: playlists
                Layout.preferredWidth: 200
                Layout.fillHeight: true
            }

            StackView {
                id: stack
                Layout.fillWidth: true
                Layout.fillHeight: true
                initialItem: Qt.resolvedUrl("pages/MainPage.qml")

                Component.onCompleted: {
                    Components.Router.stack = stack
                }
            }
        }

        Components.Player {
            id: player
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: 120
            z: 100
        }
    }
}
