import QtQuick
import QtQuick.Controls

Row {
    spacing: 15

    TextField {
        width: 320
        height: 54
        id: input
        font.pixelSize: 16
        color: '#ffffff'
        placeholderText: "Search..."
        placeholderTextColor: '#f2f2f2'
    }

    Button {
        width: 54
        height: 54
        onClicked: {
            trackService.findTrack(input.value)
        }
    }
}