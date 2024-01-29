// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick 2.0
import QtGraphs
import QtTest 1.0

Item {
    id: top
    height: 150
    width: 150

    ScatterSeries {
        id: initial
    }

    TestCase {
        name: "ScatterSeries Initial"

        function test_1_initial() {
            // Properties from QScatterSeries
            compare(initial.axisX, null)
            compare(initial.axisY, null)
            compare(initial.pointMarker, null)
        }
    }
}
