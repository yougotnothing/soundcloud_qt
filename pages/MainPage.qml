import QtQuick
import QtQuick.Controls
import "../components"

Page {
    title: "Main"

    Rectangle {
        anchors.fill: parent
        color: "#000"
    }

    Item {
        anchors.fill: parent

        Text {
            anchors.centerIn: parent
            text: "text"
            color: "white"
        }
    }
}
