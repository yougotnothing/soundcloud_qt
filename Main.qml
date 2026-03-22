import QtQuick
import QtMultimedia
import QtQuick.Controls
import './components' as Components

Window {
    width: 820
    height: 640
    visible: true
    color: '#000000'

    Components.Playlists {
        width: 360
        height: parent.height
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 20
    }

    Column {
        width: 920
        spacing: 20
        anchors.centerIn: parent

        Button {
            text: "play track"
            onClicked: {
                console.log("STREAM: ", trackService.streamUrl)
                trackService.play()
            }
        }

        Components.Search {}

        Rectangle {
            width: parent.width
            height: 120
            color: "#181818"
            radius: 8

            MediaPlayer {
                id: player
                source: trackService.streamUrl
                audioOutput: AudioOutput {
                    id: audio
                }
            }

            Row {
                width: parent.width
                anchors.margins: 15
                spacing: 15

                Column {
                    spacing: 10

                    Image {
                        source: trackService.artworkUrl
                        sourceSize.width: 50
                        sourceSize.height: 50
                        width: 50
                        height: 50
                    }

                    Text {
                        text: trackService.title
                        color: "white"
                        font.pixelSize: 18
                    }
                }

                Button {
                    text: player.playbackState === MediaPlayer.PlayingState ? "Pause" : "Play"

                    onClicked: {
                        if (player.playbackState === MediaPlayer.PlayingState)
                            trackService.pause()
                        else
                            trackService.play()
                    }
                }

                Slider {
                    id: progress
                    width: 250
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
                    width: 80
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
}
