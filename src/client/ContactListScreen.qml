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
        Label {
            text: controller.userId
        }
    }
    footer: Rectangle {
        height: 64
        width: root.width
    }

    ListView {
        anchors.fill: parent

    }
    RoundButton {
        id: addContact

    }
}
