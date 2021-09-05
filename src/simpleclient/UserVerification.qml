import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Pane {
    id: pane

    signal success()
    signal failure()



    Column {
        id: layout
        anchors.fill: parent
        width: parent.width
        spacing: 10
        Item {
            height: parent.height * 0.2
            width: parent.width
        }

        Label {
            width: layout.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: qsTr("Пожалуйста введите проверочный код")
        }

        TextField {
            id: p1
            width: parent.width
            inputMethodHints: Qt.ImhDigitsOnly

            placeholderText: qsTr("код")
        }

        Button {
            width: parent.width
            text: qsTr("Подтвердить")
        }
    }
}
