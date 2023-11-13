// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtGraphs

Rectangle {
    anchors.fill: parent
    color: "#404040"

    GraphsView {
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
            axisX: BarCategoryAxis {
                id: xAxis
                categories: [1, 2, 3, 4, 5, 6]
                gridVisible: checkBoxGridXMajor.checked
                minorGridVisible: checkBoxGridXMinor.checked
            }
            axisY: ValueAxis {
                id: yAxis
                max: 10
                autoScale: false
                minorTickCount: 1
                gridVisible: checkBoxGridYMajor.checked
                minorGridVisible: checkBoxGridYMinor.checked
            }

            BarSet { id: set1; label: "Bob"; values: [1, 2, 3, 4, 5, 6] }
        }
    }

    SettingsView {
        CustomLabel {
            text: "Margin: Left"
        }
        CustomSlider {
            sliderValue: chartView.marginLeft
            fromValue: 0
            toValue: 60
            onSliderValueChanged: chartView.marginLeft = sliderValue;
        }
        CustomLabel {
            text: "Margin: Right"
        }
        CustomSlider {
            sliderValue: chartView.marginRight
            fromValue: 0
            toValue: 60
            onSliderValueChanged: chartView.marginRight = sliderValue;
        }
        CustomLabel {
            text: "Margin: Top"
        }
        CustomSlider {
            sliderValue: chartView.marginTop
            fromValue: 0
            toValue: 60
            onSliderValueChanged: chartView.marginTop = sliderValue;
        }
        CustomLabel {
            text: "Margin: Bottom"
        }
        CustomSlider {
            sliderValue: chartView.marginBottom
            fromValue: 0
            toValue: 60
            onSliderValueChanged: chartView.marginBottom = sliderValue;
        }
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
        CustomCheckBox {
            id: checkBoxGridYMajor
            text: "Grid Y: Major lines"
            checked: true
        }
        CustomCheckBox {
            id: checkBoxGridYMinor
            text: "Grid Y: Minor lines"
            checked: true
        }
        CustomCheckBox {
            id: checkBoxGridXMajor
            text: "Grid X: Major lines"
            checked: false
        }
        CustomCheckBox {
            id: checkBoxGridXMinor
            text: "Grid X: Minor lines"
            checked: false
        }

        CustomLabel {
            text: "Y-coordinate: Minor Ticks"
        }
        CustomSlider {
            id: sliderYTicksValue
            sliderValue: yAxis.minorTickCount
            sliderStepSize: 1
            fromValue: 0
            toValue: 9
            onSliderValueChanged: yAxis.minorTickCount = sliderValue;
        }
    }
}
