// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtGraphs

Rectangle {
    id: mainView
    width: 800
    height: 600
    color: "#4F4040"

    GraphsView {
        id: chartView
        anchors.left: parent.left
        anchors.right: parent.right
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
            selectable: true
            width: 2

            axisX: ValueAxis {
                id: xAxis
                max: 4.1
            }
            axisY: ValueAxis {
                id: yAxis
                max: 3.2
            }

            XYPoint { x: 0; y: 0.0 }
            XYPoint { x: 1.1; y: 1.1 }
            XYPoint { x: 1.9; y: 2.4 }
            XYPoint { x: 1.5; y: 1.8 }
            XYPoint { x: 2.9; y: 2.6 }
            XYPoint { x: 3.4; y: 2.3 }
            XYPoint { x: 4.1; y: 3.1 }
        }
    }
}
