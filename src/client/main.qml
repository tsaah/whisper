import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import Whisper.Controller 1.0

Window {
    id: rootWindow
    title: 'Whisper'
    width: 350
    height: 625
    visible: true
    color: '#32303d'
    property var currentUserId

    Material.theme: Material.Dark
    Material.primary: '#3a3846' // Material.Purple
    Material.accent: '#24f4b9'
    Material.background: '#32303d'
    Material.foreground: '#fff'


    Component.onCompleted: {
        controller.connectToServer(defaults.host, defaults.port)
        if (controller.userId)
        console.log(controller.userId, controller.deviceCertificateHash)
    }

    QtObject {
        id: defaults
        readonly property string host: '10.0.0.128'
//        readonly property string host: '10.0.0.20'
        readonly property int port: 12345
    }


    StackView {
        id: stackView
        anchors.fill: parent
        enabled: controller.connectionState === 3

        initialItem: loginScreenComponent

        pushEnter: Transition {
            PropertyAnimation {
                property: 'x'
                from: rootWindow.width
                to: 0
                duration: 200
                easing.type: Easing.OutExpo
            }
        }
        pushExit: Transition {
            PropertyAnimation {
                property: 'opacity'
                from: 1
                to: 0.6
                duration: 200
                easing.type: Easing.OutExpo
            }
        }
        popEnter: Transition {
            PropertyAnimation {
                property: 'opacity'
                from: 0.6
                to: 1
                duration: 200
                easing.type: Easing.OutExpo
            }
        }
        popExit: Transition {
            PropertyAnimation {
                property: 'x'
                from: 0
                to: rootWindow.width
                duration: 200
                easing.type: Easing.OutExpo
            }
        }
    }

    Component {
        id: loginScreenComponent
        LoginScreen {}
    }
    Component {
        id: lockScreenComponent
        LockScreen {}
    }




//    Timer {
//        id: lockTimer
//        interval: 5000
//        running: true
//        onTriggered: stackView.push(lockScreenComponent)
//    }

    Timer {
        id: reconnectTimer
        interval: 300
        running: controller.connectionState !== 3
        onTriggered: controller.connectToServer(defaults.host, defaults.port)
    }

    HandshakeDialog {
        id: handshakeDialog
    }

    ConnectingWidget {}

    SplashScreen {}
}
