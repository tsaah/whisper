import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Pane {
    id: pane
    signal user(int id)

    Column {
        id: layout
        anchors.centerIn: parent
        spacing: 10

        Label {
            width: layout.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: qsTr("Вы хотите создать нового подьзователя?")
        }

        Button {
            width:  pane.width * 0.7
            text: qsTr("Зарегистрировать нового пользователя")
            onClicked: pane.user(0)
        }

        Label {
            width: layout.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: qsTr("У Вас уже есть номер пользователя которым Вы хотите войти?")
        }

        TextField {
            id: user
            width: parent.width
            inputMethodHints: Qt.ImhDigitsOnly

            placeholderText: qsTr("Введите номер пользователя")
        }

        Button {
            enabled: /\d{6}/.test(user.text)
            width:  pane.width * 0.7
            text: qsTr("Войти существующим пользователем")
            onClicked: pane.user(user.text)
        }
    }

}
