// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick
import QtGraphs
import QtTest

Item {
    height: 150
    width: 150

    SplineSeries {
        id: initial
    }

    SplineSeries {
        id: initialized

        name: "spline"
        axisX: ValueAxis{ max: 5; min: 0 }
        axisY: ValueAxis{ max: 5; min: 0 }
        width: 15
        capStyle: Qt.RoundCap
        pointMarker: Rectangle { width: 2; height: 2 }
        color: "#aabbcc"
        selectedColor:  "#aabbcc"
        markerSize: 2
        visible: false
        selectable: false
        hoverable: true
        opacity: 0.8
        valuesMultiplier: 0.8
    }

    ValueAxis { id: exampleX; max: 20 }
    ValueAxis { id: exampleY; max: 20 }
    Component { id: marker; Rectangle { width: 10; height: 10 } }

    TestCase {
        name: "SplineSeries Initial"

        function test_initial() {
            compare(initial.axisX, null)
            compare(initial.axisY, null)
            compare(initial.width, 1.0)
            compare(initial.pointMarker, null)
            compare(initial.capStyle, Qt.SquareCap)

            compare(initial.color, "#00000000")
            compare(initial.selectedColor, "#00000000")
            compare(initial.markerSize, 15.0)

            // compare(initial.theme, null) // New theme instantiated in QAbstractSeries
            compare(initial.name, "")
            compare(initial.visible, true)
            compare(initial.selectable, false)
            compare(initial.hoverable, false)
            compare(initial.opacity, 1.0)
            compare(initial.valuesMultiplier, 1.0)
        }

        function initial_changed() {
            initial.axisX = axisx
            initial.axisY = axisy
            initial.width = 10.0
            initial.capStyle = Qt.RoundCap
            initial.pointMarker = marker

            initial.color = "#ff00ff"
            initial.selectedColor = "#00ff00"
            initial.markerSize = 5.0

            initial.name = "spline"
            initial.visible = false
            initial.selectable = true
            initial.hoverable = true
            initial.opacity = 0.5
            initial.valuesMultiplier = 0.5

            compare(initial.axisX, axisx)
            compare(initial.axisY, axisy)
            compare(initial.axisX.max, 10)
            compare(initial.axisY.max, 10)
            compare(initial.width, 10.0)
            compare(initial.capStyle, Qt.RoundCap)
            compare(initial.pointMarker, marker)

            compare(initial.color, "#ff00ff")
            compare(initial.selectedColor, "#00ff00")
            compare(initial.markerSize, 5.0)

            compare(initial.name, "spline")
            compare(initial.visible, false)
            compare(initial.selectable, true)
            compare(initial.hoverable, true)
            compare(initial.opacity, 0.5)
            compare(initial.valuesMultiplier, 0.5)
        }
    }

    TestCase {
        name: "SplineSeries Initialized"

        function test_initialized() {
            compare(initialized.axisX.max, 5)
            compare(initialized.axisY.max, 5)
            compare(initialized.width, 15)
            compare(initialized.capStyle, Qt.RoundCap)
            verify(initialized.pointMarker)

            compare(initialized.color, "#aabbcc")
            compare(initialized.selectedColor, "#aabbcc")
            compare(initialized.markerSize, 2)

            compare(initialized.name, "spline")
            compare(initialized.visible, false)
            compare(initialized.selectable, false)
            compare(initialized.hoverable, true)
            compare(initialized.opacity, 0.8)
            compare(initialized.valuesMultiplier, 0.8)
        }
    }

}
