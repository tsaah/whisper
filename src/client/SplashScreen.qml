import QtQuick 2.15
import QtQuick.Controls.Material 2.15

Rectangle {
    id: root
    anchors.fill: parent
    color: '#000000'

    readonly property int firstAnimationTime: 500
    readonly property int secondAnimationTime: 100
    readonly property int thirdAnimationTime: 500

    property bool finished: root.opacity == 0

    ColorAnimation on color { to: '#32303d'; duration: firstAnimationTime; easing.type: Easing.OutExpo }
    Behavior on opacity { NumberAnimation { duration: thirdAnimationTime; easing.type: Easing.OutExpo } }
    Behavior on scale { NumberAnimation { duration: thirdAnimationTime; easing.type: Easing.OutExpo } }

    Timer {
        interval: firstAnimationTime + secondAnimationTime
        running: true
        onTriggered: {
            root.opacity = 0
            root.scale = 2
        }
    }
    Image {
        id: splashLogo
        anchors.centerIn: parent
        width: Math.min(parent.width, parent.height) * 0.25
        height: width
        scale: 0.00001
        opacity: 0
        source: 'qrc:/images/silence-white.png'
        NumberAnimation on scale { to: 1; duration: firstAnimationTime; easing.type: Easing.OutElastic }
        NumberAnimation on opacity { to: 1; duration: firstAnimationTime; easing.type: Easing.OutElastic }
        RotationAnimation on rotation { direction: RotationAnimation.Clockwise; to: 360; duration: firstAnimationTime; easing.type: Easing.OutElastic }
    }
}
