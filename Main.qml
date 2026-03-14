import QtQuick
import QtMultimedia
import QtQuick.Controls

Window {
    width: 640
    height: 480
    visible: true
    color: '#000000'

    Text {
        text: "title"
        color: 'green'
        font.pixelSize: 32
    }

    Column {
        spacing: 20
        anchors.centerIn: parent

        Button {
            text: "play track"
            onClicked: {
                console.log("STREAM: ", trackService.streamUrl)
                player.play()
            }
        }

        TextInput {
            id: input
            text: "placeholder"
            color: 'white'
            width: 200
            height: 30
        }

        Button {
            text: "find"
            onClicked: {
                console.log("you entered: ", input.text)
                trackService.findTrack(input.text)
            }
        }


        Rectangle {
            width: 640
            height: 120
            color: "#181818"
            radius: 8

            MediaPlayer {
                id: player
                source: trackService.streamUrl
                audioOutput: AudioOutput {
                    id: audio
                    volume: .5
                }
            }

            Row {
                anchors.fill: parent
                anchors.margins: 15
                spacing: 15

                Button {
                    text: player.playbackState === MediaPlayer.PlayingState ? "Pause" : "Play"

                    onClicked: {
                        if (player.playbackState === MediaPlayer.PlayingState)
                            player.pause()
                        else
                            player.play()
                    }
                }

                Slider {
                    id: progress
                    width: 250
                    from: 0
                    to: player.duration

                    value: player.position

                    onMoved: player.setPosition(value)
                }

                Text {
                    text: Math.floor(player.position / 1000) + "s"
                    color: "white"
                }

                Slider {
                    id: volumeSlider
                    width: 80
                    from: 0
                    to: 1
                    value: 0.2
                }
            }

            Connections {
                target: trackService
                function onTrackSetted() {
                    player.source = trackService.streamUrl
                    player.play()
                    trackService.play()
                }
            }
        }
    }
}
