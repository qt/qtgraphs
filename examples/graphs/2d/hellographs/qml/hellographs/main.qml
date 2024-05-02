// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Layouts
import QtGraphs

Item {
    id: mainView
    width: 1280
    height: 720

    RowLayout  {
        id: graphsRow

        readonly property real margin:  mainView.width * 0.02

        anchors.fill: parent
        anchors.margins: margin
        spacing: margin

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: "#262626"
            border.color: "#4d4d4d"
            border.width: 1
            radius: graphsRow.margin
            //! [bargraph]
            GraphsView {
                anchors.fill: parent
                anchors.margins: 16
                axisX: BarCategoryAxis {
                    categories: [2024, 2025, 2026]
                    gridVisible: false
                    minorGridVisible: false
                }
                axisY: ValueAxis {
                    min: 20
                    max: 100
                    tickInterval: 10
                    minorTickCount: 9
                }
                theme: GraphsTheme {
                    colorScheme: Qt.Dark
                    theme: GraphsTheme.Theme.QtGreen
                }
                //! [bargraph]
                //! [barseries]
                BarSeries {
                    //! [barseries]
                    //! [barset]
                    BarSet {
                        values: [82, 50, 75]
                        borderWidth: 2
                        color: "#373F26"
                        borderColor: "#DBEB00"
                    }
                    //! [barset]
                }
            }
        }

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: "#262626"
            border.color: "#4d4d4d"
            border.width: 1
            radius: graphsRow.margin

            //! [linegraph]
            GraphsView {
                anchors.fill: parent
                anchors.margins: 16
                theme: GraphsTheme {
                    readonly property color c1: "#DBEB00"
                    readonly property color c2: "#373F26"
                    readonly property color c3: Qt.lighter(c2, 1.5)
                    colorScheme: Qt.Dark
                    theme: GraphsTheme.Theme.QtGreenNeon
                    gridMainColor: c3
                    gridSubColor: c2
                    axisXMainColor: c3
                    axisYMainColor: c3
                    axisXSubColor: c2
                    axisYSubColor: c2
                    axisXLabelColor: c1
                    axisYLabelColor: c1
                }
                axisX: ValueAxis {
                    max: 5
                    tickInterval: 1
                    minorTickCount: 9
                    labelDecimals: 1
                }
                axisY: ValueAxis {
                    max: 10
                    tickInterval: 1
                    minorTickCount: 4
                    labelDecimals: 1
                }
                //! [linegraph]

                //! [linemarker]
                component Marker : Rectangle {
                    width: 16
                    height: 16
                    color: "#ffffff"
                    radius: width * 0.5
                    border.width: 4
                    border.color: "#000000"
                }
                //! [linemarker]

                //! [lineseriestheme]
                GraphsTheme {
                    id: seriesTheme
                    seriesColors: ["#2CDE85", "#DBEB00"]
                }
                //! [lineseriestheme]

                //! [lineseries1]
                LineSeries {
                    id: lineSeries1
                    theme: seriesTheme
                    width: 4
                    pointMarker: Marker { }
                    XYPoint { x: 0; y: 0 }
                    XYPoint { x: 1; y: 2.1 }
                    XYPoint { x: 2; y: 3.3 }
                    XYPoint { x: 3; y: 2.1 }
                    XYPoint { x: 4; y: 4.9 }
                    XYPoint { x: 5; y: 3.0 }
                }
                //! [lineseries1]

                //! [lineseries2]
                LineSeries {
                    id: lineSeries2
                    theme: seriesTheme
                    width: 4
                    pointMarker: Marker { }
                    XYPoint { x: 0; y: 5.0 }
                    XYPoint { x: 1; y: 3.3 }
                    XYPoint { x: 2; y: 7.1 }
                    XYPoint { x: 3; y: 7.5 }
                    XYPoint { x: 4; y: 6.1 }
                    XYPoint { x: 5; y: 3.2 }
                }
                //! [lineseries2]
            }
        }
    }
}
