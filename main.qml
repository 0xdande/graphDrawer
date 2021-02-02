import QtQuick 2.15
import QtQuick.Window 2.2
import QtQuick.Controls 2.15
import io.qt.graphdraw.logic 1.0

ApplicationWindow {
    id: window
    width: 320
    height: 260
    visible: true
    minimumHeight: 480
    minimumWidth: 720
    Logic {
        id: logic
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            Action {
                text: qsTr("&Export to file")
            }
            Action {
                text: qsTr("&Load from file")
            }
            MenuSeparator {}
            Action {
                text: qsTr("&Quit")
                onTriggered: Qt.quit()
            }
        }
        Menu {
            title: qsTr("&Edit")
            Action {
                text: qsTr("&Generate")
            }
            Action {
                text: qsTr("&Clear")
            }
        }
        Menu {
            title: qsTr("&Help")
            Action {
                text: qsTr("&About")
                onTriggered: showMessageBox('Created by Artem Siryk!')
            }
        }
    }

    Item {
        anchors.centerIn: parent
        width: msgDialog.width
        height: msgDialog.height
        MessageDialog {
            id: msgDialog
            title: qsTr("Information")
            visible: false
        }
    }
    function showMessageBox(message) {
        msgDialog.text = message
        msgDialog.visible = true
    }
    Canvas {
        id: graph
        width: parent.width
        height: parent.height
        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton
            onPressed: {
                logic.HandlePress()
            }
            onReleased: {
                logic.HandleRelease()
            }
        }

        onPaint: {
            var ctx = getContext("2d")
            ctx.fillStyle = Qt.rgba(Math.random(), Math.random(),
                                    Math.random(), 1)
            ctx.fillRect(0, 0, width, height)
            console.log("Panos")
        }
        TapHandler {
            id: handler
            onTapped: {
                graph.requestPaint()
            }
        }
    }
}
