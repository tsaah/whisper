import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import Whisper.Controller 1.0

Item {
    id: root
    anchors.left: parent.left
    anchors.bottom: parent.bottom

    readonly property int connectionState: controller.connectionState

    readonly property var colors: [ '#ef9a9a', '#9fa8da', '#9fa8da', '#8bc34a', 'black', '#ef9a9a', 'black' ]
    readonly property color widgetColor: '#302930'
    readonly property color textColor: '#fff'
    readonly property int size: 20

    width: widgetBackground.width + root.size * 0.8
    height: widgetBackground.height + root.size * 0.8

    Rectangle {
        id: widgetBackground
        radius: height * 0.5
        color: root.widgetColor
        width: widgetLayout.implicitWidth + root.size * 0.5
        height: widgetLayout.implicitHeight + root.size * 0.5

        x: root.size * 0.25
        opacity: root.connectionState !== 3 ? 1 : 0
        y: root.connectionState !== 3 ? root.size * 0.5 : (height + size)
        Behavior on opacity { NumberAnimation { duration: 300; easing.type: Easing.OutExpo } }
        Behavior on y { NumberAnimation { duration: 300; easing.type: Easing.OutExpo } }

        RowLayout {
            id: widgetLayout
            anchors.centerIn: parent
            spacing: root.size * 0.5
            Rectangle {
                id: spinnerBackground
                width: root.size
                height: width
                radius: width * 0.5
                color: root.colors[root.connectionState]
                Behavior on color { ColorAnimation { duration: 100 } }
                BusyIndicator {
                    id: busyIndicator
                    readonly property int dotSize: 6
                    anchors.fill: parent


                    running: true
                    contentItem: Item {
                        implicitWidth: spinnerBackground.width
                        implicitHeight: spinnerBackground.height
                        width: spinnerBackground.width
                        height: spinnerBackground.height

                        Item {
                            id: item
                            anchors.fill: spinnerBackground
                            x: -spinnerBackground.width / 2 + busyIndicator.dotSize * 0.5 + 1 + busyIndicator.dotSize * 0.5
                            y: -spinnerBackground.height / 2 + busyIndicator.dotSize * 0.5 + 1 + busyIndicator.dotSize * 0.5
                            width: spinnerBackground.width - busyIndicator.dotSize
                            height: spinnerBackground.height - busyIndicator.dotSize
                            opacity: busyIndicator.running ? 1 : 0

                            Behavior on opacity {
                                OpacityAnimator {
                                    duration: 250
                                }
                            }

                            RotationAnimator {
                                target: item
                                running: busyIndicator.visible && busyIndicator.running
                                from: 0
                                to: 360
                                loops: Animation.Infinite
                                duration: 1250
                            }

                            Repeater {
                                id: repeater
                                model: 6

                                Rectangle {
                                    x: item.width / 2 + width / 2
                                    y: item.height / 2 + height / 2
                                    width: busyIndicator.dotSize
                                    height: busyIndicator.dotSize
                                    radius: busyIndicator.dotSize * 0.5
                                    color: '#222'
                                    transform: [
                                        Translate {
                                            x: -Math.min(item.width, item.height) * 0.5
                                        },
                                        Rotation {
                                            angle: index / repeater.count * 360
                                            origin.x: -busyIndicator.dotSize * 0.5
                                            origin.y: -busyIndicator.dotSize * 0.5
                                        }
                                    ]
                                }
                            }
                        }
                    }
                }
            }
            Label {
                Layout.rightMargin: root.size * 0.7
                height: root.size
                verticalAlignment: Qt.AlignVCenter
                text: 'Connecting...'
                color: root.textColor
                font.pixelSize: root.size * 0.7
            }
        }
    }
}
