import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

Page {
    id: root
    anchors.fill: parent
    title: qsTr('Lock screen', 'title')

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
            text: qsTr('Unlock session')
            Material.background: '#8bc34a'
        }
        Button {
            Layout.fillWidth: true
            text: qsTr('End session')
        }
        Item {
            Layout.fillHeight: true
        }
    }
}
