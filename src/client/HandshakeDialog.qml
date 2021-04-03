import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import Whisper.Controller 1.0

Dialog {
    id: root
    anchors.centerIn: parent
    property string challenge: ''
    property int retry: 0
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        Label {
            Layout.fillWidth: true
            color: 'gray'
            text: root.challenge
        }
        Label {
            Layout.fillWidth: true
            color: 'gray'
            text: 'retry %1'.arg(root.retry)
        }
        TextField {
            Layout.fillWidth: true
            id: challengeReplyValue
            Layout.rightMargin: 20
            focus: true
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
                onClicked: {
                    controller.sendHandshakeChallangeReply(challengeReplyValue.text)
                    challengeReplyValue.text = ''
                }
            }
        }
    }
    Connections {
        target: controller
        function onHandshakeChallenge(challenge) {
            root.challenge = challenge
            root.retry = 0
            root.open()
        }
        function onHandshakeRetry() {
            ++root.retry
        }
        function onHandshakeSuccessfull() {
            root.challenge = ''
            root.retry = 0
            root.close()
        }
    }
}
