// clang-format off
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
    function create_timer() {
        return Qt.createQmlObject("import QtQuick 2.0; Timer {}", window)
    }
    function delay(delayTime, cb) {
        let timer = create_timer()
        timer.interval = delayTime
        timer.repeat = true
        timer.triggered.connect(cb)
        timer.start()
    }

    FileDialog {
        id: dgde
        title: qsTr("Load from file")
        selectExisting: false
        onAccepted: {
            let path = dgde.fileUrl.toString()
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
            title: qsTr("&Algorithms")
            Action {
                text: qsTr("&Eulers Path")
                onTriggered: {
                    var a = logic.EulersPathAPI()
                    //                    for (var i = 0; i < a * 2; i++) {
                    delay(500, graph.requestPaint)
                    //                    }
                }
            }
            Action {
                text: qsTr("&Djikstra")
                onTriggered: {
                    let res = logic.DjikstraAPI()
                    if (!res) {
                        showMessageBox(
                                    "Select 2 vertices, first - start, second - endpoint")
                    }
                    delay(500, graph.requestPaint)
                }
            }
            Action {
                text: qsTr("&Kruskal Algo")
                onTriggered: {
                    logic.KruskalAlgo()

                    graph.requestPaint()
                }
            }
        }
        Menu {
            title: qsTr("&Help")
            Action {
                text: qsTr("&About")
                onTriggered: showMessageBox(
                                 'Created by Artem Siryk!\nPress C to connect 2 selected Vertices\nPress D to delete Vertices\nPress E to delete Edges\nRead Algorithms section for more info')
            }
            Action {
                text: qsTr("&Test")
                onTriggered: showInputBox('Ssanina')
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
    Item {
        anchors.centerIn: parent
        width: priceDialog.width
        height: priceDialog.height
        PriceDialog {
            id: priceDialog
            title: qsTr("Input Price")
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
                let r = logic.HandleClick(mouse.x, mouse.y)
                if (r.x == 1) {
                    PriceDialog.from = r.y
                    priceDialog.from = r.y
                    PriceDialog.to = r.z
                    priceDialog.to = r.z
                    showInputBox("")
                }

                graph.requestPaint()
            }
            onPressAndHold: {
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
                ctx.strokeStyle = "#28B463"
                ctx.fillStyle = "#28B463"
            } else {
                ctx.strokeStyle = "#34495E"
                ctx.fillStyle = "#34495E"
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
        function drawEdges(xfrom, yfrom, xto, yto, price) {
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

            ctx.fillStyle = "#F4D03F"
            ctx.font = "25px Verdana"
            ctx.fillText(
                        price, ((xfrom + xto) / 2) + ((Math.min(yto,
                                                                yfrom) / Math.max(
                                                           yto, yfrom))) * 20, ((yfrom + yto) / 2)
                        + ((1 - (Math.min(yto, yfrom) / Math.max(yto,
                                                                 yfrom)))) * 20)

            ctx.stroke()
        }

        onPaint: {
            let ctx = graph.getContext("2d")
            ctx.clearRect(0, 0, parent.width, parent.height)

            logic.DrawEdgesAPI().forEach(function (element) {
                drawEdges(element.m11, element.m12, element.m13, element.m14,
                          element.m21)
            })
            logic.DrawVerticesAPI().forEach(function (element) {
                drawCircle(element.m11, element.m12, element.m13, element.m14)
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
    function showInputBox(message) {
        priceDialog.visible = true
        console.log(PriceDialog.ready)
    }
} // clang-format on
