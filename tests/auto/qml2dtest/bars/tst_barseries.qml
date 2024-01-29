// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick
import QtGraphs
import QtTest

Item {
    id: top
    height: 150
    width: 150

    BarSeries {
        id: initial
    }

    TestCase {
        name: "BarSeries Initial"

        function test_1_initial() {
            // Properties from QBarSeries
            compare(initial.axisX, null)
            compare(initial.axisY, null)
        }

        function test_2_initial_common() {
            // Common properties from QAbstractBarSeries
            compare(initial.barWidth, 0.5)
            compare(initial.count, 0)
            compare(initial.labelsVisible, false)
            compare(initial.labelsFormat, "")
            compare(initial.labelsPosition, BarSeries.LabelsCenter)
            compare(initial.labelsAngle, 0)
            compare(initial.labelsPrecision, 6)
        }
    }
}
