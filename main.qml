import QtQuick 2.15
import QtQuick.Dialogs 1.3
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
    property bool isholded: true
    Logic {
        id: logic
    }

    FileDialog {
        id: dgde
        title: qsTr("Load from file")
        selectExisting: false
        onAccepted: {
            let path = dg.fileUrl.toString()
            // remove prefixed "file:///"
            path = path.replace(/^(file:\/{3})/, "")
            // unescape html codes like '%23' for '#'
            let cleanPath = '/' + decodeURIComponent(path)
            logic.Deserialize(cleanPath)
            graph.requestPaint()
            console.log(cleanPath)
        }
    }
    FileDialog {
        id: dg
        title: qsTr("Save as")
        selectExisting: false
        onAccepted: {
            let path = dg.fileUrl.toString()
            // remove prefixed "file:///"
            path = path.replace(/^(file:\/{3})/, "")
            // unescape html codes like '%23' for '#'
            let cleanPath = '/' + decodeURIComponent(path)
            logic.Serialize(cleanPath)
            graph.requestPaint()
            console.log(cleanPath)
        }
        //        Component.onCompleted: visible = true
    }
    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            Action {
                text: qsTr("&Export to file")
                onTriggered: dg.open()
            }
            Action {
                text: qsTr("&Load from file")
                onTriggered: {
                    dgde.open()
                    console.log("Opened")
                }
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
                let res = logic.HandleConnection()
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
                if (!isholded) {
                    logic.HandleClick(mouse.x, mouse.y)
                } else {
                    isholded = false
                }

                graph.requestPaint()
            }
            onPressAndHold: {
                console.log("holded")
                isholded = true
            }
            onReleased: {
                isholded = false
                graph.requestPaint()
            }

            onPositionChanged: {
                if (isholded) {
                    logic.SetIDByCoords(mouse.x, mouse.y)
                    graph.requestPaint()
                }
            }
        }
        function drawCircle(id, is_active, x, y) {
            let ctx = graph.getContext("2d")
            if (is_active) {
                ctx.strokeStyle = /*"#922B21"*/ "#28B463"
                ctx.fillStyle = /*"#922B21"*/ "#28B463"
            } else {
                ctx.strokeStyle = "#34495E" //"#6495ED"
                ctx.fillStyle = "#34495E" //"#6495ED"
            }

            ctx.lineWidth = parent.size / 20
            ctx.beginPath()
            let startAngle = Math.PI / 5 * 3
            let endAngle = startAngle + 10 * Math.PI / 5 * 9
            ctx.arc(x, y, 30, startAngle, endAngle)
            ctx.stroke()
            ctx.fill()
            ctx.fillStyle = "#F4D03F"
            ctx.font = "25px Verdana"
            ctx.fillText(id, (x - id.toString().length * 10), y + 10)
            graph.requestPaint()
        }
        function drawEdges(xfrom, yfrom, xto, yto) {
            let ctx = graph.getContext("2d")
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
            let ctx = graph.getContext("2d")
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
