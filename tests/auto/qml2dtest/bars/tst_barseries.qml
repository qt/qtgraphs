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

    BarSeries {
        id: initialized

        barWidth: 0.2
        labelsVisible: true
        labelsFormat: "i"
        labelsPosition: BarSeries.LabelsPosition.OutsideEnd
        labelsMargin: 15.0
        labelsAngle: 25.0
        labelsPrecision: 10

        name: "BarSeries"
        visible: false
        selectable: true
        hoverable: true
        opacity: 0.75
        valuesMultiplier: 0.75

        BarSet { label: "Set1"; values: [1, 2, 3, 4, 5, 6] }
    }

    Component {
        id: customBarComponent
        Item {
            id: comp
            property color barColor
            property color barBorderColor
            property real barBorderWidth
            Rectangle {
                anchors.fill: parent
                color: comp.barColor
                border.color: comp.barBorderColor
                border.width: comp.barBorderWidth
            }
        }
    }

    TestCase {
        name: "BarSeries Initial"

        function test_1_initial_common() {
            // Common properties from QBarSeries
            compare(initial.barWidth, 0.5)
            compare(initial.count, 0)
            compare(initial.labelsVisible, false)
            compare(initial.labelsFormat, "")
            compare(initial.labelsPosition, BarSeries.LabelsPosition.Center)
            compare(initial.labelsMargin, 0)
            compare(initial.labelsAngle, 0)
            compare(initial.labelsPrecision, 6)
            compare(initial.seriesColors, [])
            compare(initial.borderColors, [])

            // Properties from QAbstractSeries
            compare(initial.name, "")
            compare(initial.visible, true)
            compare(initial.selectable, false)
            compare(initial.hoverable, false)
            compare(initial.opacity, 1.0)
            compare(initial.valuesMultiplier, 1.0)
            compare(initial.barDelegate, null)
        }

        function test_2_initial_change() {
            initial.barDelegate = customBarComponent

            initial.barWidth = 0.1
            // TODO: How to add a set dynamically?
            //initial.count = 1
            initial.labelsVisible = true
            initial.labelsFormat = "i"
            initial.labelsPosition = BarSeries.LabelsPosition.InsideBase
            initial.labelsMargin = 20.0
            initial.labelsAngle = 45.0
            initial.labelsPrecision = 3

            initial.name = "Bars"
            initial.visible = false
            initial.selectable = true
            initial.hoverable = true
            initial.opacity = 0.5
            initial.valuesMultiplier = 0.0

            compare(initial.barDelegate, customBarComponent)

            compare(initial.barWidth, 0.1)
            // TODO: How to add a set dynamically?
            //compare(initial.count, 1)
            compare(initial.labelsVisible, true)
            compare(initial.labelsFormat, "i")
            compare(initial.labelsPosition, BarSeries.LabelsPosition.InsideBase)
            compare(initial.labelsMargin, 20.0)
            compare(initial.labelsAngle, 45.0)
            compare(initial.labelsPrecision, 3)

            compare(initial.name, "Bars")
            compare(initial.visible, false)
            compare(initial.selectable, true)
            compare(initial.hoverable, true)
            compare(initial.opacity, 0.5)
            compare(initial.valuesMultiplier, 0.0)
        }
    }

    TestCase {
        name: "BarSeries Initialized"

        function test_1_initialized() {
            compare(initialized.barWidth, 0.2)
            compare(initialized.count, 1)
            compare(initialized.labelsVisible, true)
            compare(initialized.labelsFormat, "i")
            compare(initialized.labelsPosition, BarSeries.LabelsPosition.OutsideEnd)
            compare(initialized.labelsMargin, 15.0)
            compare(initialized.labelsAngle, 25.0)
            compare(initialized.labelsPrecision, 10)

            compare(initialized.name, "BarSeries")
            compare(initialized.visible, false)
            compare(initialized.selectable, true)
            compare(initialized.hoverable, true)
            compare(initialized.opacity, 0.75)
            compare(initialized.valuesMultiplier, 0.75)
        }

        function test_2_initialized_change() {
            initialized.barWidth = 0.1
            // TODO: How to add a set dynamically?
            //initialized.count = 2
            initialized.labelsVisible = true
            initialized.labelsFormat = "i"
            initialized.labelsPosition = BarSeries.LabelsPosition.InsideBase
            initialized.labelsMargin = 35.0
            initialized.labelsAngle = 45.0
            initialized.labelsPrecision = 3

            initialized.name = "Bars"
            initialized.visible = true
            initialized.selectable = false
            initialized.hoverable = false
            initialized.opacity = 0.5
            initialized.valuesMultiplier = 0.5

            compare(initialized.barWidth, 0.1)
            // TODO: How to add a set dynamically?
            //compare(initialized.count, 2)
            compare(initialized.labelsVisible, true)
            compare(initialized.labelsFormat, "i")
            compare(initialized.labelsPosition, BarSeries.LabelsPosition.InsideBase)
            compare(initialized.labelsMargin, 35.0)
            compare(initialized.labelsAngle, 45.0)
            compare(initialized.labelsPrecision, 3)

            compare(initialized.name, "Bars")
            compare(initialized.visible, true)
            compare(initialized.selectable, false)
            compare(initialized.hoverable, false)
            compare(initialized.opacity, 0.5)
            compare(initialized.valuesMultiplier, 0.5)
        }

        function test_3_initialized_change_to_invalid() {
            initialized.barWidth = 2.0 // range 0...1
            initialized.valuesMultiplier = 2.0 // range 0...1

            compare(initialized.barWidth, 1.0)
            compare(initialized.valuesMultiplier, 1.0)

            initialized.barWidth = -1.0 // range 0...1
            initialized.valuesMultiplier = -1.0 // range 0...1
            compare(initialized.barWidth, 0.0)
            compare(initialized.valuesMultiplier, 0.0)
        }

        BarSet {id:b1}
        BarSet {id:b2}
        BarSet {id:b3}
        BarSet {id:b4}
        BarSet {id:b5}
        BarSet {id:b6}
        BarSet {id:b7}

        function test_5_modify_series() {
            // append
            let list = [b4, b5, b6]

            initialized.append(b1)
            initialized.append(b2)
            initialized.append(b3)
            initialized.append(list)
            compare(initialized.count, 7)

            // insert
            initialized.insert(3, b7)
            compare(initialized.count, 8)

            // at
            let atBar = initialized.at(5)
            compare(atBar, b4)

            // find
            let findIndex = initialized.find(b6)
            compare(findIndex, 7)

            // remove
            initialized.remove(b1)
            initialized.remove(0)
            compare(initialized.count, 6)

            // remove multiple
            initialized.removeMultiple(0,3)
            compare(initialized.count, 3)

            // take
            verify(initialized.take(b5))
            compare(initialized.count, 2)
        }
    }
}
