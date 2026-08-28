import QtQuick.Controls
import QtQuick

Row {
    spacing: 15
    anchors.top: parent.top

    TextField {
        width: 320
        height: 32
        id: input
        font.pixelSize: 16
        color: '#ffffff'
        placeholderText: "Search..."
        placeholderTextColor: '#f2f2f2'
        onTextChanged: () => {
            if (!searchService.dropdownOpened) searchService.setOpened(true)
        }

        Rectangle {
            anchors.fill: parent
            color: "transparent"
            border.width: 0;
        }
    }

    Button {
        width: 54
        height: 54
        onClicked: () => {
            trackService.findTrack(input.text)
        }
    }
}
