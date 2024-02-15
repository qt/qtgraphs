// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
import QtQuick
import QtGraphs
import QtTest

Item {
    id: top
    height: 150
    width: 150
    GraphsView {
        id: graphsView
        height: top.height
        width: top.width

        theme: GraphTheme {
            id: myTheme
            colorTheme: GraphTheme.ColorThemeDark
            axisXLabelsFont.pixelSize: 20
            axisYLabelsFont.pixelSize: 16
        }

        BarSeries {
            id: barInitial
        }

        BarSeries {
            id: barInitialized

            theme: theme1

            name: "BarSeries"
            visible: false

            BarSet {
                label: "Set1"
                values: [1, 2, 3, 4, 5, 6]
            }
        }
        BarSeries {
            id: barInitialized2

            theme: theme2

            name: "BarSeries"
            visible: false

            BarSet {
                label: "Set1"
                values: [1, 2, 3, 4, 5, 6]
            }
            BarSet {
                label: "Set2"
                values: [1, 2, 3, 4, 5, 6]
            }
        }
    }

    SeriesTheme {
        id: theme1
        colors: ["#ff0000"]
        borderColors: ["#00ff00"]
    }

    SeriesTheme {
        id: theme2
        colors: ["#0000ff", "#ff0000"]
        borderColors: ["#00ffff", "#00ff00"]
    }

    TestCase {
        name: "LegendData BarSeries Initial"

        function test_1_initial() {
            // Properties from QBarSeries
            compare(barInitial.legendData.length, 0)
        }

        function test_1_initial_change() {
            barInitial.theme = theme1
            compare(barInitial.legendData.length, 0)
        }
    }

    TestCase {
        name: "LegendData BarSeries Initialized"

        function test_1_initialized() {
            compare(barInitialized.legendData.length, 1)

            compare(barInitialized.legendData[0].color, "#ff0000")
            compare(barInitialized.legendData[0].borderColor, "#00ff00")
            compare(barInitialized2.legendData[0].label, "Set1")
        }

        function test_2_initialized() {
            compare(barInitialized2.legendData.length, 2)

            compare(barInitialized2.legendData[0].color, "#0000ff")
            compare(barInitialized2.legendData[0].borderColor, "#00ffff")
            compare(barInitialized2.legendData[0].label, "Set1")

            compare(barInitialized2.legendData[1].color, "#ff0000")
            compare(barInitialized2.legendData[1].borderColor, "#00ff00")
            compare(barInitialized2.legendData[1].label, "Set2")
        }

        function test_3_initialized_change() {
            barInitialized.theme = theme2
            waitForRendering(top)
            compare(barInitialized.legendData.length, 1)

            compare(barInitialized.legendData[0].color, "#0000ff")
            compare(barInitialized.legendData[0].borderColor, "#00ffff")
            compare(barInitialized2.legendData[0].label, "Set1")

            barInitialized2.theme = theme1
            waitForRendering(top)
            compare(barInitialized2.legendData.length, 2)

            compare(barInitialized2.legendData[0].color, "#ff0000")
            compare(barInitialized2.legendData[0].borderColor, "#00ff00")
            compare(barInitialized2.legendData[0].label, "Set1")

            compare(barInitialized2.legendData[1].color, "#ff0000")
            compare(barInitialized2.legendData[1].borderColor, "#00ff00")
            compare(barInitialized2.legendData[1].label, "Set2")
        }
    }
}
