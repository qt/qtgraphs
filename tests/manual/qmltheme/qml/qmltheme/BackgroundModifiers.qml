// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion
import QtQuick.Dialogs
import QtGraphs

ColumnLayout {
    spacing: 10

    property alias testgridChange: testgridChange

    Label {
        visible: testgridChange.checked
        text: "Grid Width"
        color: "gray"
    }
    Slider {
        visible: testgridChange.checked
        from: 0.0
        to: 1.0
        value: customTheme.gridMainWidth
        onValueChanged: {
            if (testgridChange.checked) {
                customTheme.gridMainWidth = value
            }
        }
    }

    Label {
        text: "Grid color"
        color: "gray"
    }

    Button {
        Layout.preferredHeight: 25
        Layout.preferredWidth: 25

        Rectangle {
            anchors.fill: parent
            anchors.margins: 5
            color: customTheme.gridMainColor
        }

        onClicked: gridlineCol.open()
    }

    ColorDialog {
        id: gridlineCol
        selectedColor: customTheme.gridMainColor
        onAccepted: customTheme.gridMainColor = selectedColor
    }

    Label {
        text: "Subgrid color"
        color: "gray"
    }

    Button {
        Layout.preferredHeight: 25
        Layout.preferredWidth: 25

        Rectangle {
            anchors.fill: parent
            anchors.margins: 5
            color: customTheme.gridSubColor
        }

        onClicked: subgridlineCol.open()
    }

    ColorDialog {
        id: subgridlineCol
        selectedColor: customTheme.gridSubColor
        onAccepted: customTheme.gridSubColor = selectedColor
    }

    Label {
        text: "Subsegments"
        color: "gray"
    }
    Slider {
        from: 1
        to: 5
        stepSize: 1
        snapMode: Slider.SnapAlways
        value: bars.visible? bars.valueAxis.subSegmentCount : surface.axisX.subSegmentCount
        onValueChanged: {
            surface.axisX.subSegmentCount = value
            surface.axisY.subSegmentCount = value
            surface.axisZ.subSegmentCount = value
            bars.valueAxis.subSegmentCount = value
        }
    }

    Label {
        text: "Background"
        color: "gray"
    }
    CheckBox {
        checked: customTheme.backgroundEnabled
        onCheckedChanged: {
            customTheme.backgroundEnabled = checked
        }
    }

    Label {
        text: "Grid"
        color: "gray"
    }
    CheckBox {
        checked: customTheme.gridEnabled
        onCheckedChanged: {
            customTheme.gridEnabled = checked
        }
    }

    Label {
        text: "Geometry/Shader Grid"
        color: "gray"
    }
    CheckBox {
        id: testgridChange
        checked: false
    }

    Label {
        text: "Labels"
        color: "gray"
    }
    CheckBox {
        checked: customTheme.labelsEnabled
        onCheckedChanged: {
            customTheme.labelsEnabled = checked
        }
    }

    Label {
        text: "Label Background"
        color: "gray"
    }
    CheckBox {
        checked: customTheme.labelBackgroundEnabled
        onCheckedChanged: {
            customTheme.labelBackgroundEnabled = checked
        }
    }

    ColorDialog {
        id: labelTextCol
        selectedColor: customTheme.labelTextColor
        onAccepted: customTheme.labelTextColor = selectedColor
    }

    Label {
        text: "Label Border"
        color: "gray"
    }
    CheckBox {
        checked: customTheme.labelBorderEnabled
        onCheckedChanged: {
            customTheme.labelBorderEnabled = checked
        }
    }

    Label {
        text: "Axis label text color"
        color: "gray"
    }
    Button {
        Layout.preferredHeight: 25
        Layout.preferredWidth: 25

        Rectangle {
            anchors.fill: parent
            anchors.margins: 5
            color: customTheme.axisXLabelColor
        }

        onClicked: axisLabelTextCol.open()
    }

    ColorDialog {
        id: axisLabelTextCol
        selectedColor: customTheme.axisXLabelColor
        onAccepted: {
            customTheme.axisXLabelColor = selectedColor
            customTheme.axisYLabelColor = selectedColor
            customTheme.axisZLabelColor = selectedColor
        }
    }
}
