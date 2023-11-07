// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtGraphs2D

Item {
    anchors.fill: parent

    ChartView {
        id: chartView
        anchors.fill: parent
        anchors.margins: 20 * px
        theme: GraphTheme {
            id: myTheme
            colorTheme: GraphTheme.ColorThemeDark
            axisXLabelsFont.pixelSize: 20
        }
        BarSeries {
            axisX: BarCategoryAxis { categories: ["2023", "2024", "2025", "2026"] }
            axisY: ValueAxis { }
            selectable: true
            BarSet { id: set1; label: "Axel"; values: [1, 2, 3, 4] }
            BarSet { id: set2; label: "Frank"; values: [8, 2, 0, 7] }
            BarSet { id: set3; label: "James"; values: [4+3*Math.sin(fA.elapsedTime), 5+3*Math.sin(fA.elapsedTime), 2, 3] }
            FrameAnimation {
                id: fA
                running: true
            }
        }
    }
}
