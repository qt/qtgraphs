// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtGraphs
import QtQuick.Controls

Rectangle {
    id: mainView
    width: 800
    height: 600
    color: "#4F4040"

    GraphsView {
        property int xMax: xAxis.max
        property int yMax: yAxis.max

        id: chartView
        anchors.left: parent.left
        anchors.right: rcolumn.left
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.margins: 10
        backgroundColor: "#202020"

        SeriesTheme {
            id: seriesTheme
            colorTheme: SeriesTheme.SeriesTheme1
        }

        SplineSeries {
            id: spline
            name: "Spline"
            theme: seriesTheme
            width: 4

            axisY: ValueAxis {
                id: yAxis
                max: 5
            }

            axisX: ValueAxis {
                id: xAxis
                tickInterval: 1
                max: 6
                min: 0
            }

            GraphTransition {
                GraphPointAnimation { duration: 1000; easingCurve.type: Easing.OutCubic }
                SplineControlAnimation { duration: 1000; easingCurve.type: Easing.OutCubic }
            }

            XYPoint { x: 0.0; y: 0.0 }
        }

        LineSeries {
            id: line
            name: "Line"
            theme: seriesTheme
            width: 8

            GraphTransition {
                GraphPointAnimation { duration: 1000; easingCurve.type: Easing.OutCubic }
            }

            XYPoint { x: 0.0; y: 0.0 }
        }
    }

    Column {
        property int splineValue: 0
        property int lineValue: 0

        id: rcolumn
        spacing: 5
        anchors.margins: 30
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: 300

        Text {
            text: "Spline:"
            font.pixelSize: 24
            color: "#ffffff"
        }

        RoundButton {
            property alias v: rcolumn.splineValue
            text: "Add Point"
            anchors.left: parent.left
            anchors.right: parent.right

            onClicked: {
                if (v >= chartView.xMax)
                    return

                spline.append(++v, Math.random() * chartView.yMax)
                spline.update()
            }
        }

        SpinBox {
            id: sbox
            to: chartView.xMax
            anchors.left: parent.left
            anchors.right: parent.right
        }

        RoundButton {
            property alias v: rcolumn.splineValue
            text: "Replace Point"
            anchors.left: parent.left
            anchors.right: parent.right

            onClicked: {
                sbox.value = Math.min(v, sbox.value)

                spline.replace(sbox.value, sbox.value, Math.random() * chartView.yMax)
                spline.update()
            }
        }

        RoundButton {
            property alias v: rcolumn.splineValue
            text: "Remove Point"
            anchors.left: parent.left
            anchors.right: parent.right
            enabled: false

            onClicked: {
                spline.remove(v--)
                spline.update()
            }
        }

        Item {
            height: 50
            width: 200
        }

        Text {
            text: "Line:"
            font.pixelSize: 24
            color: "#ffffff"
        }

        RoundButton {
            property alias v: rcolumn.lineValue
            text: "Add Point"
            anchors.left: parent.left
            anchors.right: parent.right

            onClicked: {
                if (v >= chartView.xMax)
                    return

                line.append(++v, Math.random() * chartView.yMax)
                line.update()
            }
        }

        SpinBox {
            id: lbox
            to: chartView.xMax
            anchors.left: parent.left
            anchors.right: parent.right
        }

        RoundButton {
            property alias v: rcolumn.lineValue
            text: "Replace Point"
            anchors.left: parent.left
            anchors.right: parent.right

            onClicked: {
                lbox.value = Math.min(v, lbox.value)

                line.replace(lbox.value, lbox.value, Math.random() * chartView.yMax)
                line.update()
            }
        }

        RoundButton {
            property alias v: rcolumn.lineValue
            text: "Remove Point"
            anchors.left: parent.left
            anchors.right: parent.right
            enabled: false

            onClicked: {
                line.remove(v--)
                line.update()
            }
        }
    }
}
