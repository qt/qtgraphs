// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtGraphs2D

Item {
    anchors.fill: parent

    ChartView {
        id: chartView
        anchors.fill: parent
        anchors.margins: 20 * px
        theme: GraphTheme {
            id: myTheme
            colorTheme: GraphTheme.ColorThemeDark
            axisXLabelsFont.pixelSize: 20
        }
        BarSeries {
            id: mySeries
            axisX: ValueAxis {
                id: xAxis
            }
            axisY: ValueAxis {
                id: yAxis
                max: 10
                autoScale: false
            }

            BarSet { id: set1; label: "Bob"; values: [1, 2, 3, 4, 5, 6] }
        }
    }

    SettingsView {
        CustomLabel {
            text: "Y-coordinate: Max"
        }
        CustomSlider {
            id: sliderYMaxValue
            sliderValue: yAxis.max
            fromValue: 0.1
            toValue: 40
            onSliderValueChanged: yAxis.max = sliderValue;
        }
        CustomLabel {
            text: "Y-coordinate: Min"
        }
        CustomSlider {
            id: sliderYMinValue
            sliderValue: yAxis.min
            fromValue: -3
            toValue: 3
            onSliderValueChanged: yAxis.min = sliderValue;
        }
    }
}
