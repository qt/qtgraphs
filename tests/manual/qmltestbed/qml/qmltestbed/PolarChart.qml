// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick
import QtQuick.Layouts
import QtQuick.Shapes
import QtGraphs

Rectangle {
    id: mainView
    width: 1400
    height: 900
    color: "#202020"

    SettingsView {
        id: settingsView

        CustomCheckBox {
            id: zoomPanEnabled
            text: "Enable Mouse Zoom/Pan"
            checked: false
        }
    }

    GridLayout {
        anchors.fill: parent
        anchors.margins: 10
        columns: 3
        rows: 2

        // 1) Line + Scatter on a plain ValueAxis/ValueAxis polar view.
        // Also exercises labelPadding and the radial axis's tickAnchor
        // (labels 1, 3, 5, 7, 9 instead of 2, 4, 6, 8, 10).
        ColumnLayout {
            Text { text: "Line + Scatter (ValueAxis / ValueAxis)"; color: "white" }
            PolarView {
                Layout.fillWidth: true
                Layout.fillHeight: true

                labelPadding: 12

                axisX: ValueAxis { min: 0; max: 360 } // angular, in degrees
                axisY: ValueAxis { min: 0; max: 10; tickInterval: 2; tickAnchor: 1 } // radial

                zoomStyle: zoomPanEnabled.checked ? GraphsView.ZoomStyle.Center : GraphsView.ZoomStyle.None
                panStyle: zoomPanEnabled.checked ? GraphsView.PanStyle.Drag : GraphsView.PanStyle.None

                LineSeries {
                    XYPoint { x: 0; y: 10 }
                    XYPoint { x: 60; y: 5 }
                    XYPoint { x: 120; y: 3 }
                    XYPoint { x: 180; y: 8 }
                    XYPoint { x: 240; y: 4 }
                    XYPoint { x: 300; y: 6 }
                    XYPoint { x: 360; y: 2 }
                }

                ScatterSeries {
                    color: "#ffaa00"
                    XYPoint { x: 30; y: 7 }
                    XYPoint { x: 150; y: 9 }
                    XYPoint { x: 270; y: 5 }
                }
            }
        }

        // 2) SplineSeries.
        ColumnLayout {
            Text { text: "SplineSeries (ValueAxis / ValueAxis)"; color: "white" }
            PolarView {
                Layout.fillWidth: true
                Layout.fillHeight: true

                axisX: ValueAxis { min: 0; max: 360 }
                axisY: ValueAxis { min: 0; max: 10 }

                zoomStyle: zoomPanEnabled.checked ? GraphsView.ZoomStyle.Center : GraphsView.ZoomStyle.None
                panStyle: zoomPanEnabled.checked ? GraphsView.PanStyle.Drag : GraphsView.PanStyle.None

                SplineSeries {
                    color: "#66ccff"
                    XYPoint { x: 0; y: 3 }
                    XYPoint { x: 90; y: 9 }
                    XYPoint { x: 180; y: 2 }
                    XYPoint { x: 270; y: 7 }
                    XYPoint { x: 360; y: 3 }
                }
            }
        }

        // 3) AreaSeries (upper-only, filled from the pole out to the line).
        ColumnLayout {
            Text { text: "AreaSeries (ValueAxis / ValueAxis)"; color: "white" }
            PolarView {
                id: polarArea

                Layout.fillWidth: true
                Layout.fillHeight: true

                axisX: ValueAxis { min: 0; max: 360; tickInterval: 30 }
                axisY: ValueAxis { min: 0; max: 10 }

                zoomStyle: zoomPanEnabled.checked ? GraphsView.ZoomStyle.Center : GraphsView.ZoomStyle.None
                panStyle: zoomPanEnabled.checked ? GraphsView.PanStyle.Drag : GraphsView.PanStyle.None

                AreaSeries {
                    color: "#aa4444"
                    borderColor: "#ff8888"
                    upperSeries: LineSeries {
                        XYPoint { x: 0; y: 4 }
                        XYPoint { x: 90; y: 6 }
                        XYPoint { x: 180; y: 5 }
                        XYPoint { x: 270; y: 8 }
                        XYPoint { x: 360; y: 4 }
                    }

                    gradient: RadialGradient {
                        centerX: polarArea.polarCenter.x
                        centerY: polarArea.polarCenter.y
                        centerRadius: polarArea.polarRadius
                        // Required for a simple, centered radial gradient.
                        focalX: polarArea.polarCenter.x
                        focalY: polarArea.polarCenter.y
                        stops: [
                            GradientStop { position: 0.0; color: "red" },
                            GradientStop { position: 0.3; color: "lime" },
                            GradientStop { position: 1.0; color: "blue" }
                        ]
                    }
                }
            }
        }

        // 4) DateTimeAxis used as the angular axis.
        ColumnLayout {
            Text { text: "DateTimeAxis (angular) / ValueAxis (radial)"; color: "white" }
            PolarView {
                Layout.fillWidth: true
                Layout.fillHeight: true

                axisX: DateTimeAxis {
                    min: new Date(2026, 0, 1)
                    max: new Date(2026, 11, 31)
                    labelFormat: "MMM"
                }
                axisY: ValueAxis { min: 0; max: 100 }

                zoomStyle: zoomPanEnabled.checked ? GraphsView.ZoomStyle.Center : GraphsView.ZoomStyle.None
                panStyle: zoomPanEnabled.checked ? GraphsView.PanStyle.Drag : GraphsView.PanStyle.None

                LineSeries {
                    XYPoint { x: new Date(2026, 0, 1).getTime(); y: 20 }
                    XYPoint { x: new Date(2026, 2, 1).getTime(); y: 60 }
                    XYPoint { x: new Date(2026, 5, 1).getTime(); y: 90 }
                    XYPoint { x: new Date(2026, 8, 1).getTime(); y: 40 }
                    XYPoint { x: new Date(2026, 11, 31).getTime(); y: 20 }
                }
            }
        }

        // 5) LogValueAxis used as the radial axis.
        ColumnLayout {
            Text { text: "ValueAxis (angular) / LogValueAxis (radial)"; color: "white" }
            PolarView {
                Layout.fillWidth: true
                Layout.fillHeight: true

                axisX: ValueAxis { min: 0; max: 360; tickInterval: 30 }
                axisY: LogValueAxis { min: 1; max: 1000; base: 10 }

                zoomStyle: zoomPanEnabled.checked ? GraphsView.ZoomStyle.Center : GraphsView.ZoomStyle.None
                panStyle: zoomPanEnabled.checked ? GraphsView.PanStyle.Drag : GraphsView.PanStyle.None

                ScatterSeries {
                    color: "#ff66aa"
                    XYPoint { x: 0; y: 1 }
                    XYPoint { x: 72; y: 10 }
                    XYPoint { x: 144; y: 100 }
                    XYPoint { x: 216; y: 1000 }
                    XYPoint { x: 288; y: 50 }
                }
            }
        }

        // 6) Unsupported series type: BarSeries should be rejected with a
        // qWarning and simply not appear (see QPolarView::insertSeries).
        ColumnLayout {
            Text { text: "BarSeries on PolarView (expect a warning, no bars drawn)"; color: "white" }
            PolarView {
                Layout.fillWidth: true
                Layout.fillHeight: true

                axisX: ValueAxis { min: 0; max: 360 }
                axisY: ValueAxis { min: 0; max: 10 }

                zoomStyle: zoomPanEnabled.checked ? GraphsView.ZoomStyle.Center : GraphsView.ZoomStyle.None
                panStyle: zoomPanEnabled.checked ? GraphsView.PanStyle.Drag : GraphsView.PanStyle.None

                LineSeries {
                    XYPoint { x: 0; y: 1 }
                    XYPoint { x: 90; y: 9 }
                    XYPoint { x: 170; y: 7 }
                    XYPoint { x: 360; y: 1 }
                }

                BarSeries {
                    // Expect: "Can not add series. Series type is not
                    // supported by a polar view." in the console, and no
                    // bars rendered.
                    BarSet { values: [1, 2, 3] }
                }
            }
        }
    }
}
