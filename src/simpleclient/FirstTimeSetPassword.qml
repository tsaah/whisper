import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Pane {
    id: pane

    readonly property int minimumPasswordLength: 4
    readonly property bool passwordShouldContainLetters: true
    readonly property bool passwrodShouldContainDifits: true

    signal newPassword(string password)

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
            text: qsTr("Программа звпущена впервые, установите пароль для доступа.")
        }

        Label {
            id: checkPSize
            width: parent.width
            wrapMode: Label.Wrap
            visible: p1.text.length < pane.minimumPasswordLength
            text: qsTr("-> 4 символа и более")
        }
        Label {
            id: checkPSymbols
            width: parent.width
            wrapMode: Label.Wrap
            visible: pane.passwordShouldContainLetters && !(/[a-zA-Z]/.test(p1.text)) || pane.passwrodShouldContainDifits && !(/\d/.test(p1.text))
            text: qsTr("-> цифры и буквы")
        }
        Label {
            id: checkPMatch
            width: parent.width
            wrapMode: Label.Wrap
            visible: p1.text != p2.text
            text: qsTr("-> пароли не совпадают")
        }

        TextField {
            id: p1
            width: parent.width
            echoMode: TextInput.Password
            inputMethodHints: Qt.ImhSensitiveData | Qt.ImhNoAutoUppercase | Qt.ImhHiddenText | Qt.ImhNoPredictiveText

            placeholderText: qsTr("Введите пароль")
        }

        TextField {
            id: p2
            width: parent.width
            echoMode: TextInput.Password
            inputMethodHints: Qt.ImhSensitiveData | Qt.ImhNoAutoUppercase | Qt.ImhHiddenText | Qt.ImhNoPredictiveText

            placeholderText: qsTr("Повторите пароль")
        }

        Button {
            width: parent.width
            enabled: !checkPSize.visible && !checkPSymbols.visible && !checkPMatch.visible
            text: qsTr("Готово")
            onClicked: pane.newPassword(p1.text)
        }

        Button {
            width: parent.width
            text: qsTr("Выйти")
            onClicked: Qt.quit()
        }

    }
}
