import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import Whisper.Controller 1.0

Page {
    id: root
    anchors.fill: parent
    title: qsTr('Login screen', 'title')

    readonly property int userId: controller.userId

    header: ToolBar {

    }
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 5
        spacing: 10
        Item {
            height: 10
        }
        Button {
            Layout.fillWidth: true
            visible: root.userId != 0
            text: qsTr('Login as #%1').arg(root.userId)
            Material.background: '#8bc34a'
            onClicked: handshakeDialog.open()
        }
        Button {
            Layout.fillWidth: true
            text: qsTr('Create and login as new user')
            Material.background: '#ff9800'
        }
        Button {
            Layout.fillWidth: true
            text: qsTr('Login as different user')
            Material.background: '#ff9800'
        }
        Button {
            Layout.fillWidth: true
            text: qsTr('Change device certificate')
            Material.background: '#ff5722'
            onClicked: {
                controller.changeDeviceCertificate()
                controller.connectToServer(defaults.host, defaults.port)
            }
        }
        Item {
            Layout.fillHeight: true
        }
    }
}
