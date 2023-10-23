// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtGraphs2D
import QtQuick.Controls.Basic
import QtQuick.Dialogs

Rectangle {
    id: mainview
    width: 800
    height: 600
    color: "#404040"

    ColorDialog {
        id: colorDialog
        selectedColor: lineSeries.color
        onAccepted: lineSeries.color = selectedColor
    }

    Row {
        id: toolbar
        anchors.top: parent.top
        anchors.margins: 10
        anchors.left: parent.left
        anchors.leftMargin: 60
        spacing: 10

        Slider {
            id: widthSlider
            from: 0.1
            to: 10
            value: 2
        }

        Button {
            text: qsTr("Choose color")
            onClicked: colorDialog.open()
        }
    }

    Rectangle {
        id: background
        anchors.fill: chartView
        color: "#202020"
        border.color: "#606060"
        border.width: 2
        radius: 10
    }

    ChartView {
        id: chartView
        //title: "Bar Chart"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: toolbar.bottom
        anchors.margins: 10
        backgroundColor: "#202020"
        //legend.alignment: Qt.AlignBottom
        //antialiasing: true

        LineSeries {
            id: lineSeries
            name: "Line"
            width: widthSlider.value
            XYPoint { x: 0; y: 0 }
            XYPoint { x: 1.1; y: 2.1 }
            XYPoint { x: 1.9; y: 3.3 }
            XYPoint { x: 2.1; y: 2.1 }
            XYPoint { x: 2.9; y: 4.9 }
            XYPoint { x: 3.4; y: 3.0 }
            XYPoint { x: 4.1; y: 3.3 }
        }

        // TEMP
        FrameAnimation {
            id: fA
            running: true
            onTriggered: chartView.update();
        }
    }
}
