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
    Canvas {
        id: graph
        width: parent.width
        height: parent.height
        focus: true
        Keys.onDeletePressed: {
            console.log("Deleted Vertices")
            logic.HandleDelete()
            graph.requestPaint()
        }
        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton
            onClicked: {
                logic.HandleClick(mouse.x, mouse.y)
                graph.requestPaint()
            }
            onReleased: {
                logic.HandleRelease(mouse.x, mouse.y)
            }
            onPressAndHold: {
                console.log("HANDLED")
                logic.HandleHold(mouse.x, mouse.y)
            }
        }
        function drawCircle(id, is_active, x, y) {
            console.log(is_active)
            var ctx = graph.getContext("2d")
            if (is_active) {
                ctx.strokeStyle = /*"#922B21"*/ "#28B463"
                ctx.fillStyle = /*"#922B21"*/ "#28B463"
            } else {
                ctx.strokeStyle = "#34495E" //"#6495ED"
                ctx.fillStyle = "#34495E" //"#6495ED"
            }

            ctx.lineWidth = parent.size / 20
            ctx.beginPath()
            var startAngle = Math.PI / 5 * 3
            var endAngle = startAngle + 10 * Math.PI / 5 * 9
            ctx.arc(x, y, 30, startAngle, endAngle)
            ctx.stroke()
            ctx.fill()
            graph.requestPaint()
        }
        onPaint: {
            var ctx = graph.getContext("2d")
            ctx.clearRect(0, 0, parent.width, parent.height)
            let to_draw = logic.DrawVerticesAPI()
            to_draw.forEach(function (element) {
                drawCircle(element.x, element.y, element.z, element.w)
            })
            console.log("Adjacency List Size:" + logic.adjacency_list.length)
        }
        TapHandler {
            id: handler
            onTapped: {

                console.log("buba")
                graph.requestPaint()
            }
        }
    }

    function showMessageBox(message) {
        msgDialog.text = message
        msgDialog.visible = true
    }
}
