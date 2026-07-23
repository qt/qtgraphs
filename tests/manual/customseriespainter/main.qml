// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick
import QtQuick.Controls
import QtGraphs
import CustomSeriesPainterTest

Window {
    width: 640
    height: 520
    visible: true
    title: "Custom series painter test"
    Rectangle {
        width: parent.width
        height: parent.height
        color: "white"

        GraphsView {
            id: graphsView
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: controls.top
            anchors.margins: 10

            // Force the CanvasPainter (high-performance) backend, so this test
            // exercises QCustomSeriesCanvasRenderer instead of the Qt Quick
            // Shapes-based delegate path.
            useCanvasPainter: true

            theme: GraphsTheme {
                colorScheme: GraphsTheme.ColorScheme.Light
            }

            axisX: ValueAxis {
                min: 0
                max: 10
            }
            axisY: ValueAxis {
                min: 0
                max: 10
            }

            SquareSeries {
                id: squareSeries

                Component.onCompleted: {
                    squareSeries.addSquare(2, 2, 1.5, "red")
                    squareSeries.addSquare(5, 6, 2.5, "green")
                    squareSeries.addSquare(8, 3, 1, "blue")
                    squareSeries.addSquare(3, 8, 2, "orange")
                }
            }
        }

        // Rapidly destroys and recreates the whole square set on the GUI
        // thread while the render thread keeps painting from the previous
        // synchronizeData() snapshot. Exercises the GUI/render-thread sync
        // path that canvasPaint() relies on never seeing live series state.
        Timer {
            id: churnTimer
            interval: 1
            repeat: true
            onTriggered: {
                squareSeries.clearSquares()
                var count = 1 + Math.floor(Math.random() * 6)
                for (var i = 0; i < count; ++i) {
                    squareSeries.addSquare(Math.random() * 10,
                                           Math.random() * 10,
                                           0.5 + Math.random() * 2.5,
                                           Qt.rgba(Math.random(), Math.random(), Math.random(), 1))
                }
                churnCountLabel.count++
            }
        }

        // Used to spawn/destroy a second, independent SquareSeries below.
        Component {
            id: dynamicSquareSeriesComponent
            SquareSeries {}
        }

        // Repeatedly destroys and recreates a whole SquareSeries -- and thus
        // its QCustomSeriesCanvasRenderer -- on the GUI thread, while the
        // render thread may still be painting from a snapshot that referred
        // to the previous instance. Unlike churnTimer above (which only ever
        // mutates data on a series that stays alive), this exercises the
        // renderer object's own construction/destruction racing an in-flight
        // canvasPaint().
        Timer {
            id: teardownTimer
            interval: 10
            repeat: true
            property var currentSeries: null
            onTriggered: {
                if (currentSeries) {
                    graphsView.removeSeries(currentSeries)
                    currentSeries.destroy()
                    currentSeries = null
                }
                currentSeries = dynamicSquareSeriesComponent.createObject(graphsView)
                graphsView.addSeries(currentSeries)
                var count = 1 + Math.floor(Math.random() * 6)
                for (var i = 0; i < count; ++i) {
                    currentSeries.addSquare(Math.random() * 10,
                                            Math.random() * 10,
                                            0.5 + Math.random() * 2.5,
                                            Qt.rgba(Math.random(), Math.random(), Math.random(), 1))
                }
                teardownCountLabel.count++
            }
        }

        Column {
            id: controls
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: 10
            spacing: 6

            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 10

                Button {
                    text: churnTimer.running ? "Stop sync stress test" : "Start sync stress test"
                    onClicked: churnTimer.running = !churnTimer.running
                }

                Label {
                    id: churnCountLabel
                    property int count: 0
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Churn count: " + count
                }
            }

            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 10

                Button {
                    text: teardownTimer.running ? "Stop teardown stress test" : "Start teardown stress test"
                    onClicked: teardownTimer.running = !teardownTimer.running
                }

                Label {
                    id: teardownCountLabel
                    property int count: 0
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Teardown count: " + count
                }
            }
        }
    }
}
