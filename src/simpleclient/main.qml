import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
//import Whisper.Controller 1.0

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
        readonly property string host: '10.0.0.128'
//        readonly property string host: '10.0.0.20'
        readonly property int port: 12345
    }
    QtObject {
        id: state
    }

//    Connections {
//        target: controller
//        function onHandshakeChallenge(challenge) {
//            challengeBox.doshow = true
//            challengeBox.achallenge = challenge
//            challengeBox.aretry = 0
//        }
//        function onHandshakeRetry() {
//            ++challengeBox.aretry
//        }
//        function onHandshakeSuccessfull() {
//            challengeBox.doshow = false
//            challengeBox.achallenge = ''
//            challengeBox.aretry = 0
//        }
//        function onContactRequest(userId) {
//            console.log('contact request from' + String(userId))
//        }
//        function onContactAccepted(userId) {
//            console.log('contact request accepted from' + String(userId))
//        }
//    }


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
            property var actions: [ 'Connect', '…', '…', 'Disconnect', '…', '…', '…' ]
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
//                    border.width: 1
//                    border.color: '#00000080'
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
                    Behavior on opacity {
                        OpacityAnimator {
                            duration: 250
                        }
                    }
                    running: true
                    contentItem: Item {
                         implicitWidth: connectionStateIndicator.width
                         implicitHeight: connectionStateIndicator.height
                         width: connectionStateIndicator.width
                         height: connectionStateIndicator.height

                         Item {
                             id: item
//                             anchors.fill: connectionStateIndicator
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
                                 model: 5

                                 Rectangle {
                                     x: item.width / 2 + width / 2
                                     y: item.height / 2 + height / 2
                                     width: 4
                                     height: 4
                                     radius: 2
                                     color: "#214efb"
                                     transform: [
                                         Translate {
                                             x: -Math.min(item.width, item.height) * 0.5
                                         },
                                         Rotation {
                                             angle: index / repeater.count * 360
                                             origin.x: -2
                                             origin.y: -2
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

        Rectangle {
            Layout.fillWidth: true
            color: 'yellow'
            radius: 4
            height: 50
            Label {
                color: controller.authorized ? 'geen' : 'red'
                text: controller.userId
                font.pixelSize: 30
            }
        }

        ColumnLayout {
            id: challengeBox
            visible: doshow
            property bool doshow: false
            property string achallenge: ''
            property int aretry: 0
            Label {
                color: 'gray'
                text: challengeBox.achallenge
            }
            Label {
                color: 'gray'
                text: 'retry %1'.arg(challengeBox.aretry)
            }
            TextField {
                Layout.rightMargin: 20
                Layout.fillWidth: true
                onEditingFinished: {
                    controller.sendHandshakeChallangeReply(text)
                    text = ''
                }
                RoundButton {
                    width: 50
                    height: 50

                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.right
                    text: 'OK'
                }
            }

        }


        Button {
            text: 'Change Device Certificate'
            Layout.fillWidth: true
            onClicked: {
                controller.changeDeviceCertificate()
                controller.connectToServer(defaults.host, defaults.port)
            }
        }
        ColumnLayout {
            Layout.fillWidth: true

            Dialog {
                id: newUserDialog
                ColumnLayout {
                    anchors.fill: parent
                    Label {
                        text: 'Enter a password'
                        Layout.fillWidth: true
                    }
                    TextField {
                        id: p1
                        Layout.fillWidth: true
                    }
                    TextField {
                        id: p2
                        Layout.fillWidth: true
                    }
                    Button {
                        text: 'Commit'
                        Layout.fillWidth: true
                        onClicked: {
                            if (p1.text !== p2.text) {
                                p1.text = ''
                                p2.text = ''
                                console.log('parsswords don\'t match')
                            } else {
                                if (p1.text.length < 3) {
                                    console.log('parsswords should be 3 symbols or more')
                                } else {
                                    controller.createNewUser(p1.text)
                                    newUserDialog.accept()
                                }
                            }
                        }
                    }
                }
            }

            Button {
                text: 'Create new user'
                Layout.fillWidth: true
                onClicked: newUserDialog.open()
            }

            Button {
                text: 'Enter as old user'
                Layout.fillWidth: true
            }
        }

        Button {
            Dialog {
                id: addContact
                ColumnLayout {
                    anchors.fill: parent
                    Label {
                        text: 'Enter contact id'
                        Layout.fillWidth: true
                    }
                    TextField {
                        id: contactId
                        Layout.fillWidth: true
                    }
                    Button {
                        text: 'Request'
                        Layout.fillWidth: true
                        onClicked: {
                            controller.addContact(Number(contactId.text))
                            addContact.accept()
                        }
                    }
                }
            }
            text: 'Add Contact'
            Layout.fillWidth: true
            onClicked: addContact.open()
        }

        ListView {
            id: contactList
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 2

            model: ListModel {
                ListElement {
                    name: "Bill Smith"
                    number: "1"
                }
                ListElement {
                    name: "John Brown"
                    number: "2"
                }
                ListElement {
                    name: "Sam Wise"
                    number: "3"
                }
            }

            delegate: Rectangle {
                height: 64
                width: contactList.width
                radius: 3
                color: mouseArea.containsMouse ? 'green' : 'gray'
                Text {
                    anchors.fill: parent
                    anchors.margins: 5
                    text: name
                    font.pixelSize: 18
                }
                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        messageBox.contactId = number
                        messageBox.open()
                    }
                }
                Dialog {
                    id: messageBox
                    property int contactId
                    ColumnLayout {
                        Label {
                            text: messageBox.contactId
                        }
                        TextArea {
                            width: messageBox.width
                            height: 256
                        }
                        Button {
                            text: 'send'
                            onClicked: messageBox.accept()
                        }
                    }
                }

            }
        }
    }

}
