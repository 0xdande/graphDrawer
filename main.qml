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
                onTriggered: {
                    logic.HandleDeleteAll()
                    graph.requestPaint()
                }
            }
        }
        Menu {
            title: qsTr("&Help")
            Action {
                text: qsTr("&About")
                onTriggered: showMessageBox(
                                 'Created by Artem Siryk!\n Press C to connect 2 selected Vertices\n Press D to delete Vertices\nPress E to delete Edges')
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
            logic.HandleDelete()
            graph.requestPaint()
        }
        Keys.onPressed: {
            if (event.key == Qt.Key_C) {
                var res = logic.HandleConnection()
                graph.requestPaint()
                if (!res) {
                    showMessageBox(
                                "Can connect only 2 selected Vertices that are not connected!")
                }
            }
            if (event.key == Qt.Key_E) {
                logic.HandleDeleteEdge()
                graph.requestPaint()
            }
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
                logic.HandleHold(mouse.x, mouse.y)
            }
        }
        function drawCircle(id, is_active, x, y) {
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
            ctx.fillStyle = "#F4D03F"
            ctx.font = "25px Verdana"
            ctx.fillText(id, (x - id.toString().length * 10), y + 10)
            graph.requestPaint()
        }
        function drawEdges(xfrom, yfrom, xto, yto) {
            var ctx = graph.getContext("2d")
            ctx.lineWidth = 3
            if (xfrom <= 0) {
                ctx.strokeStyle = "#922B21"
                xfrom = Math.abs(xfrom)
                yfrom = Math.abs(yfrom)
                xto = Math.abs(xto)
                yto = Math.abs(yto)
            } else {
                ctx.strokeStyle = "#17A589"
            }
            ctx.beginPath()
            ctx.moveTo(xfrom, yfrom)
            ctx.lineTo(xto, yto)
            ctx.closePath()
            ctx.stroke()
        }

        onPaint: {
            var ctx = graph.getContext("2d")
            ctx.clearRect(0, 0, parent.width, parent.height)
            let to_draw_edges = logic.DrawEdgesAPI()
            console.log("Died")
            to_draw_edges.forEach(function (element) {
                drawEdges(element.x, element.y, element.z, element.w)
            })
            let to_draw_vertices = logic.DrawVerticesAPI()
            to_draw_vertices.forEach(function (element) {
                drawCircle(element.x, element.y, element.z, element.w)
            })
        }
        TapHandler {
            id: handler
            onTapped: {
                graph.requestPaint()
            }
        }
    }

    function showMessageBox(message) {
        msgDialog.text = message
        msgDialog.visible = true
    }
}
