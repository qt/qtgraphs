// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick
import QtGraphs
import QtQuick.Dialogs
import QtQuick.Controls.Basic
import QtQuick.Layouts

Rectangle {
    id: mainView
    width: 800
    height: 600
    color: "#202020"
    ColumnLayout {
        id: infoLayout
        anchors {
            left: parent.left
            right: parent.right
            leftMargin: 25
        }
        height: 100
        spacing: 0
        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Text {
                Layout.leftMargin: 20
                font.pixelSize: 12
                color: "#ffffff"
                text: qsTr("X tick count: %1").arg(xAxis.tickCount)
            }
            Text {
                Layout.leftMargin: 20
                font.pixelSize: 24
                color: "#ffffff"
                text: "X sub-ticks:"
            }
            SpinBox {
                from: -1
                to: 15
                value: -1
                onValueChanged: xAxis.subTickCount = value
            }

            Text {
                Layout.leftMargin: 20
                font.pixelSize: 24
                color: "#ffffff"
                text: "X Base:"
            }
            SpinBox {
                value: 10
                onValueChanged: xAxis.base = value
            }
            CheckBox {
                id: zoomAreaEnabled
                text: "Zoom area enabled"
                contentItem: Text {
                    text: zoomAreaEnabled.text
                    font: zoomAreaEnabled.font
                    color: "#ffffff"
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: zoomAreaEnabled.indicator.width + zoomAreaEnabled.spacing
                }
            }
        }
        RowLayout {
            Layout.fillHeight: true
            spacing: 50
            Column {
                Label {
                    id: visualXMin
                    text: "X Axis visualMin: " + xAxis.visualMin
                    color: "white"
                }
                Label {
                    id: visualXMax
                    text: "X Axis visualMax: " + xAxis.visualMax
                    color: "white"
                }
            }
            Column {
                Label {
                    id: visualYMin
                    text: "Y Axis visualMin: " + yAxis.visualMin
                    color: "white"
                }
                Label {
                    id: visualYMax
                    text: "Y Axis visualMax: " + yAxis.visualMax
                    color: "white"
                }
            }
        }
    }



    GraphsView {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: infoLayout.bottom
        anchors.margins: 10
        zoomStyle: GraphsView.ZoomStyle.Center;
        panStyle: zoomAreaEnabled.checked ? GraphsView.PanStyle.None : GraphsView.PanStyle.Drag
        zoomAreaEnabled: zoomAreaEnabled.checked

        theme: gtheme

        GraphsTheme {
            id: gtheme
            axisYLabelFont.pixelSize: 8
            colorScheme: GraphsTheme.ColorScheme.Dark
            theme: GraphsTheme.Theme.QtGreen
        }

        axisX: LogValueAxis {
            id: xAxis
            labelsAngle: 45
            min: 0.1
            max: 10
            labelPrecision: -1
        }

        axisY: LogValueAxis {
            id: yAxis
            labelsAngle: 45
            min: 0.1
            max: 10
        }

        ToolTip {
            id: tooltip
        }

        onHoverEnter: {
            tooltip.visible = true;
        }

        onHoverExit: {
            tooltip.visible = false;
        }

        onHover: (seriesName, position, value) => {
            tooltip.x = position.x + 1;
            tooltip.y = position.y + 1;
            tooltip.text = qsTr("x: %1, y: %2").arg(value.x).arg(value.y)
        }

        LineSeries {
            id: line
            width: 8
            hoverable: true

            XYPoint { x: 0.1; y: 0.1 }
            XYPoint { x: 0.5; y: 0.5 }
            XYPoint { x: 1; y: 1 }
            XYPoint { x: 2; y: 2 }
            XYPoint { x: 3; y: 3 }
            XYPoint { x: 4; y: 4 }
            XYPoint { x: 5; y: 5 }
            XYPoint { x: 8; y: 8 }
            XYPoint { x: 10; y: 10 }
        }

        SplineSeries {
            hoverable: true
            id: spline
            width: 4
            XYPoint { x: 0.1000; y: .00001 }
            XYPoint { x: 0.1668; y: 0.1111 }
            XYPoint { x: 0.2783; y: 0.3333 }
            XYPoint { x: 0.4642; y: 0.5556 }
            XYPoint { x: 0.7743; y: 0.7782 }
            XYPoint { x: 1.2915; y: 1.0 }
            XYPoint { x: 2.1544; y: 1.3333 }
            XYPoint { x: 3.5938; y: 1.5556 }
            XYPoint { x: 5.9948; y: 1.7782 }
            XYPoint { x: 10.0000; y:  2.0000 }
        }

        AreaSeries {
            hoverable: true
            upperSeries: LineSeries {
                XYPoint { x: 0.1; y: 1 }
                XYPoint { x: 0.5; y: 10 }
                XYPoint { x: 1; y: 100 }
            }

            lowerSeries: LineSeries {
                XYPoint { x: 0.1; y: 0.1 }
                XYPoint { x: 0.5; y: 1 }
                XYPoint { x: 1; y: 10 }
            }
        }

        CustomSeries {
            id: customSeries
            hoverable: true
            delegate: Item {
                property var data
                Rectangle {
                    id: delRect
                    color: "orange"
                    y: customSeries.mapY(parent.data.valueY) - height / 2 + 1
                    x: customSeries.mapX(parent.data.valueX) - width / 2
                    height: 32
                    width: 32

                    function updatePosition() {
                        y = customSeries.mapY(parent.data.valueY) - height / 2 + 1
                        x = customSeries.mapX(parent.data.valueX) - width / 2
                    }

                    Connections {
                        target: xAxis
                        function onVisualMinChanged(visualMin) { delRect.updatePosition() }
                        function onVisualMaxChanged(visualMin) { delRect.updatePosition() }
                    }

                    Connections {
                        target: yAxis
                        function onVisualMinChanged(visualMin) { delRect.updatePosition() }
                        function onVisualMaxChanged(visualMin) { delRect.updatePosition() }
                    }
                }
            }

            CustomSeriesData {
                data: ({
                    valueX: 10,
                    valueY: 0.1
                })
            }
            CustomSeriesData {
                data: ({
                    valueX: 100,
                    valueY: 1
                })
            }
            CustomSeriesData {
                data: ({
                    valueX: 1000,
                    valueY: 10
                })
            }
        }
    }
}
