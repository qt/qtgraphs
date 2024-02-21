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
        LineSeries {
            id: lineInitial
            XYPoint {
                x: 0
                y: 6.6
            }
        }

        LineSeries {
            id: lineInitialized
            name: "Second"
            theme: theme1

            axisX: ValueAxis {
                id: xAxis
                max: 4
            }
            axisY: ValueAxis {
                id: yAxis
                max: 8
            }

            XYPoint {
                x: 0
                y: 6.6
            }
            XYPoint {
                x: 0.6
                y: 4.1
            }
            XYPoint {
                x: 1.5
                y: 5.3
            }
            XYPoint {
                x: 2.2
                y: 7.1
            }
            XYPoint {
                x: 3.3
                y: 6.9
            }
            XYPoint {
                x: 3.6
                y: 5.0
            }
            XYPoint {
                x: 4.0
                y: 5.3
            }
        }

        PieSeries {
            id: pieInitial
        }

        PieSeries {
            id: pieInitialized
            theme: theme1

            PieSlice {
                label: "Volkswagen"
                labelVisible: true
                value: 13.5
            }
            PieSlice {
                label: "Toyota"
                labelVisible: true
                labelPosition: PieSlice.LabelInsideHorizontal
                value: 10.9
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
        colors: ["#0000ff", "#ff00ff"]
        borderColors: ["#00ffff", "#ffff00"]
    }

    TestCase {
        name: "LegendData BarsRenderer Initial"

        function test_1_initial() {
            // Properties from QBarSeries
            compare(barInitial.legendData.length, 0)
        }

        function test_1_initial_change() {
            barInitial.theme = theme1
            waitForRendering(top)
            compare(barInitial.legendData.length, 0)
        }
    }

    TestCase {
        name: "LegendData BarsRenderer Initialized"

        function test_1_initialized() {
            waitForRendering(top)
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

            compare(barInitialized2.legendData[1].color, "#ff00ff")
            compare(barInitialized2.legendData[1].borderColor, "#ffff00")
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
    TestCase {
        name: "LegendData PointRenderer Initial"

        function test_1_Initial() {
            compare(lineInitial.legendData.length, 1)
            // TODO: Change this to test fillcolor after QTBUG-122434
            compare(lineInitial.legendData[0].color, "#3d9c73")
            compare(lineInitial.legendData[0].borderColor, "#3d9c73")
            compare(lineInitial.legendData[0].label, "")
        }
        function test_2_Initial_change() {
            lineInitial.theme = theme1
            waitForRendering(top)

            compare(lineInitial.legendData.length, 1)
            // TODO: Change this to test fillcolor after QTBUG-122434
            compare(lineInitial.legendData[0].color, "#ff0000")
            compare(lineInitial.legendData[0].borderColor, "#ff0000")
            compare(lineInitial.legendData[0].label, "")
        }
    }

    TestCase {
        name: "LegendData PointRenderer Initialized"

        function test_1_Initialized() {
            waitForRendering(top)
            compare(lineInitialized.legendData.length, 1)

            // TODO: Change this to test fillcolor after QTBUG-122434
            compare(lineInitialized.legendData[0].color, "#ff0000")
            compare(lineInitialized.legendData[0].borderColor, "#ff0000")
            compare(lineInitialized.legendData[0].label, "Second")
        }

        function test_2_Initialized_Change() {
            lineInitialized.theme = theme2
            waitForRendering(top)
            compare(lineInitialized.legendData.length, 1)
            // TODO: Change this to test fillcolor after QTBUG-122434
            compare(lineInitialized.legendData[0].color, "#ff00ff")
            compare(lineInitialized.legendData[0].borderColor, "#ff00ff")
            compare(lineInitialized.legendData[0].label, "Second")
        }
    }

    TestCase {
        name: "LegendData PieRenderer Initial"

        function test_1_initial() {
            waitForRendering(top)

            compare(pieInitial.legendData.length, 0)
        }

        function test_2_initial_change() {
            pieInitial.theme = theme1
            waitForRendering(top)
            compare(pieInitial.legendData.length, 0)
        }
    }

    TestCase {
        name: "LegendData PieRenderer Initialized"

        function test_1_initialized() {
            waitForRendering(top)
            compare(pieInitialized.legendData.length, 2)

            compare(pieInitialized.legendData[0].color, "#ff0000")
            compare(pieInitialized.legendData[0].borderColor, "#00ff00")
            compare(pieInitialized.legendData[0].label, "Volkswagen")

            compare(pieInitialized.legendData[1].color, "#ff0000")
            compare(pieInitialized.legendData[1].borderColor, "#00ff00")
            compare(pieInitialized.legendData[1].label, "Toyota")
        }

        function test_2_initialized_change() {
            pieInitialized.theme = theme2
            waitForRendering(top)
            compare(pieInitialized.legendData.length, 2)

            compare(pieInitialized.legendData[0].color, "#0000ff")
            compare(pieInitialized.legendData[0].borderColor, "#00ffff")
            compare(pieInitialized.legendData[0].label, "Volkswagen")

            compare(pieInitialized.legendData[1].color, "#ff00ff")
            compare(pieInitialized.legendData[1].borderColor, "#ffff00")
            compare(pieInitialized.legendData[1].label, "Toyota")
        }
    }
}
