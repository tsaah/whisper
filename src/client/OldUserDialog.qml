import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import Whisper.Controller 1.0

Dialog {
    id: root
    anchors.centerIn: parent
    property alias user: p1.text
    readonly property var loginAsOldUser: function() {
        controller.useOldUser(p1.text, p2.text)
        p1.text = ''
        p2.text = ''
        close()
    }
    onVisibleChanged: if (visible) {
                          p1.focus = true
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
            placeholderText: qsTr('user id')
            onEditingFinished: {
                p2.focus = true
            }
        }
        TextField {
            id: p2
            Layout.fillWidth: true
            Layout.rightMargin: 20
            onEditingFinished: root.loginAsOldUser()
            placeholderText: qsTr('password')
            RoundButton {
                width: 50
                height: 50

                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.right
                text: qsTr('OK')
                onClicked: root.loginAsOldUser()
            }
        }
    }
}
