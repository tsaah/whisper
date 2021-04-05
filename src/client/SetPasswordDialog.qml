import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import Whisper.Controller 1.0

Dialog {
    id: root
    anchors.centerIn: parent
    readonly property var setPass: function() {
        if (p1.text === '' || p1.text !== p2.text) {
            // TODO: notofocation that password is wrong
        } else {
            controller.createNewUser(p1.text)
            p1.text = ''
            p2.text = ''
            close()
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        Label {
            Layout.fillWidth: true
            color: 'gray'
            text: qsTr('Enter new password')
        }
        TextField {
            id: p1
            Layout.fillWidth: true
            Layout.rightMargin: 20
            focus: true
            onEditingFinished: {
                p2.focus = true
            }
        }
        TextField {
            id: p2
            Layout.fillWidth: true
            Layout.rightMargin: 20
            onEditingFinished: root.setPass()
            RoundButton {
                width: 50
                height: 50

                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.right
                text: qsTr('OK')
                onClicked: root.setPass()
            }
        }
    }
}
