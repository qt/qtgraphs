// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtGraphs
import QtTest

Item {
    id: top
    height: 150
    width: 150

    DateTimeAxis {
        id: initial
    }

    DateTimeAxis {
        id: initialized

        labelFormat: "yyyy"
        min: new Date(1960,1,1)
        max: new Date(2000,1,1)
        minorTickCount: 2
        tickInterval:  2

        gridVisible: false
        labelsAngle: 90
        labelsVisible: false
        lineVisible: false
        minorGridVisible: false
        visible: false
    }

    TestCase {
        name: "DateTimeAxis Initial"

        function test_1_initial() {
            compare(initial.labelFormat, "dd-MMMM-yy")
            compare(initial.min, new Date(1970,0,1))
            compare(initial.max, new Date(1980,0,1))
            compare(initial.minorTickCount, 0)
            compare(initial.tickInterval, 0)
        }

        function test_2_initial_common() {
            compare(initial.gridVisible, true)
            compare(initial.labelsAngle, 0)
            compare(initial.labelsVisible, true)
            compare(initial.lineVisible, true)
            compare(initial.minorGridVisible, true)
            compare(initial.visible, true)
        }

        function test_3_initial_change() {
            initial.labelFormat = "yyyy"
            initial.min = new Date(1960, 1, 1)
            initial.max = new Date(2000, 1, 1)
            initial.minorTickCount = 2
            initial.tickInterval = 2

            initial.gridVisible = false
            initial.labelsAngle = 90
            initial.labelsVisible = false
            initial.lineVisible = false
            initial.minorGridVisible = false
            initial.visible = false

            compare(initial.labelFormat, "yyyy")
            compare(initial.min, new Date(1960, 1, 1))
            compare(initial.max, new Date(2000, 1, 1))
            compare(initial.minorTickCount, 2)
            compare(initial.tickInterval, 2)

            compare(initial.gridVisible, false)
            compare(initial.labelsAngle, 90)
            compare(initial.labelsVisible, false)
            compare(initial.lineVisible, false)
            compare(initial.minorGridVisible, false)
            compare(initial.visible, false)
        }
    }

    TestCase {
        name: "DateTimeAxis Initialized"

        function test_1_initialized() {
            compare(initialized.labelFormat, "yyyy")
            compare(initialized.min, new Date(1960, 1, 1))
            compare(initialized.max, new Date(2000, 1, 1))
            compare(initialized.minorTickCount, 2)
            compare(initialized.tickInterval, 2)

            compare(initialized.gridVisible, false)
            compare(initialized.labelsAngle, 90)
            compare(initialized.labelsVisible, false)
            compare(initialized.lineVisible, false)
            compare(initialized.minorGridVisible, false)
            compare(initialized.visible, false)
        }

        function test_2_initialized_changed() {
            initialized.labelFormat = "dddd"
            initialized.min = new Date(2000, 1, 1)
            initialized.max = new Date(2025, 1, 1)
            initialized.minorTickCount = 8
            initialized.tickInterval = 8

            initialized.gridVisible = true
            initialized.labelsAngle = 50
            initialized.labelsVisible = true
            initialized.lineVisible = true
            initialized.minorGridVisible = true
            initialized.visible = true

            compare(initialized.labelFormat, "dddd")
            compare(initialized.min, new Date(2000, 1, 1))
            compare(initialized.max, new Date(2025, 1, 1))
            compare(initialized.minorTickCount, 8)
            compare(initialized.tickInterval, 8)

            compare(initialized.gridVisible, true)
            compare(initialized.labelsAngle, 50)
            compare(initialized.labelsVisible, true)
            compare(initialized.lineVisible, true)
            compare(initialized.minorGridVisible, true)
            compare(initialized.visible, true)
        }

        function test_3_invalid() {
            initialized.minorTickCount = -1;
            initialized.tickInterval = -1;

            compare(initialized.tickInterval, 0)
            compare(initialized.minorTickCount, 0)
        }
    }
}
