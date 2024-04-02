// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtGraphs
import Testbed

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
            colorTheme: SeriesTheme.SeriesColorTheme.SeriesTheme1
        }

        DynamicSplineSeries {
            id: spline
            name: "Spline"
            theme: seriesTheme
            width: 8
            animated: true

            axisY: ValueAxis {
                id: yAxis
                max: 10
            }

            XYPoint { x: 0; y: 0.0 }
        }
    }
}
