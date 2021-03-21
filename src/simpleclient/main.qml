import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import Whisper.Controller 1.0

Window {
    id: rootWindow
    title: 'Whisper'
    width: 350
    height: 625
    visible: true
    color: '#000'
    property var currentUserId

    QtObject {
        id: defaults
//        readonly property string host: '10.0.0.128'
        readonly property string host: '10.0.0.20'
        readonly property int port: 12345
    }


    Material.theme: Material.Dark
    Material.primary: '#3a3846' // Material.Purple
    Material.accent: '#24f4b9'
    Material.background: '#32303d'
    Material.foreground: '#fff'



    Component.onCompleted: {
        controller.connectToServer(defaults.host, defaults.port)
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        Button {
            id: connectionButton
            property var actions: [ 'connect', '…', '…', 'disconnect', '…', '…', '…' ]
            property var texts: [ 'unconnected', 'host lookup', 'connecting', 'connected', 'bound', 'closing', 'listening' ]
            property var colors: [ 'red', 'blue', 'blue', 'green', 'black', 'red', 'black' ]
            Layout.fillWidth: true
            enabled: controller.connectionState == 0 || controller.connectionState == 3
            text: connectionButton.actions[controller.connectionState]
            onClicked: if (controller.connectionState == 0) {
                           controller.connectToServer(defaults.host, defaults.port)
                       } else if (controller.connectionState == 3) {
                           controller.disconnectFromServer()
                       }

            Item {
                id: connectionStateIndicator
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.margins: 10
                width: 20
                height: 20



                Rectangle {
                    anchors.fill: parent
                    z: 10
                    color: connectionButton.colors[controller.connectionState]
                    radius: 10
                    border.width: 1
                    border.color: '#00000080'
                    opacity: controller.connectionState === 2 ? 0 : 1
                    Behavior on opacity {
                        OpacityAnimator {
                            duration: 250
                        }
                    }
                }
                BusyIndicator {
                    id: busyIndicator1
                    anchors.fill: parent

                    opacity: controller.connectionState === 2 ? 1 : 0
                    running: true
                    contentItem: Item {
                         implicitWidth: connectionStateIndicator.width
                         implicitHeight: connectionStateIndicator.height
                         width: connectionStateIndicator.width
                         height: connectionStateIndicator.height

                         Item {
                             id: item
                             anchors.fill: connectionStateIndicator
                             x: -connectionStateIndicator.width / 2 + 3
                             y: -connectionStateIndicator.height / 2 + 3
                             width: connectionStateIndicator.width
                             height: connectionStateIndicator.height
                             opacity: busyIndicator1.running ? 1 : 0

                             Behavior on opacity {
                                 OpacityAnimator {
                                     duration: 250
                                 }
                             }

                             RotationAnimator {
                                 target: item
                                 running: busyIndicator1.visible && busyIndicator1.running
                                 from: 0
                                 to: 360
                                 loops: Animation.Infinite
                                 duration: 1250
                             }

                             Repeater {
                                 id: repeater
                                 model: 3

                                 Rectangle {
                                     x: item.width / 2 + width / 2
                                     y: item.height / 2 + height / 2
                                     width: 6
                                     height: 6
                                     radius: 3
                                     color: "#214efb"
                                     transform: [
                                         Translate {
                                             x: -Math.min(item.width, item.height) * 0.5
                                         },
                                         Rotation {
                                             angle: index / repeater.count * 360
                                             origin.x: -3
                                             origin.y: -3
                                         }
                                     ]
                                 }
                             }
                         }
                     }
                }
            }
            Label {
                anchors.verticalCenter: connectionStateIndicator.verticalCenter
                anchors.left: connectionStateIndicator.right
                anchors.margins: 10
                color: 'gray'
                text: connectionButton.texts[controller.connectionState]
            }

        }

        Item { Layout.fillHeight: true }
    }
}
