import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Pane {
    id: pane
    signal finished()
    Column {
        id: layout
        anchors.centerIn: parent
        spacing: 10

        Label {
            width: layout.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: qsTr("Подождите, генерируем приватные ключи...")
        }

        BusyIndicator {
            width: pane.width * 0.7
            height: width
            running: true
        }
    }

    Timer {
        interval: 2000
        running: true
        repeat: false
        onTriggered: pane.finished()
    }
}
