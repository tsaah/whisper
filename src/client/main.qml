import QtQuick 2.15
import QtQuick.Layouts 1.13
import QtQuick.Window 2.13
import QtQuick.Controls 2.13
import QtQuick.Controls.Material 2.13
import Whisper.Connection 1.0

Window {
    id: rootWindow
    title: 'Whisper'
    width: 350
    height: 625
    visible: true
    color: '#000'
    property var currentUserId

    Material.theme: Material.Dark
    Material.primary: '#3a3846' // Material.Purple
    Material.accent: '#24f4b9'
    Material.background: '#32303d'
    Material.foreground: '#fff'


    Component.onCompleted: {
        connection.connectToServer('127.0.0.1', 12345)

    }
}
