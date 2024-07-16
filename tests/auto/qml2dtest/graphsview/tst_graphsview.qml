// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
import QtQuick
import QtGraphs
import QtTest

Item {
    id: top
    height: 150
    width: 150

    GraphsView {
        id: initial
    }

    GraphsTheme {
        id: myTheme
        theme: GraphsTheme.Theme.QtGreenNeon
        colorScheme: Qt.Dark
    }

    GraphsTheme {
        id: newTheme
    }

    GraphsView {
        id: initialized
        height: top.height
        width: top.width
        theme: myTheme
        marginBottom: 5
        marginLeft: 10
        marginRight: 10
        marginTop: 5

        axisX: BarCategoryAxis {
            id: axisXInitial
            categories: [ "2002", "2003", "2004" ]
        }

        axisY: ValueAxis {
            id: axisYInitial
            max: 4
        }

        BarSeries {
            id: barInitial
        }

        LineSeries {
            id: lineInitial
        }

        AreaSeries {
            id: areaInitial
        }
    }

    BarCategoryAxis {
        id: axisX
        categories: [ "2012", "2013", "2014" ]
    }

    ValueAxis {
        id: axisY
        max: 8
    }

    TestCase {
        name: "GraphsView Initial"

        function test_1_initial() {
            compare(initial.height, 0)
            compare(initial.width, 0)
            compare(initial.marginTop, 20)
            compare(initial.marginBottom, 20)
            compare(initial.marginLeft, 20)
            compare(initial.marginRight, 20)
            compare(initial.seriesList, [])
            compare(initial.axisX, null)
            compare(initial.axisY, null)
            // compare some of the contents of the initial theme, as theme itself cannot be
            compare(initial.theme.theme, GraphsTheme.Theme.QtGreen)
            compare(initial.theme.colorScheme, GraphsTheme.ColorScheme.Automatic)
            compare(initial.theme.seriesColors.length, 5)
        }

        function test_1_initial_change() {
            initial.height = 100
            initial.width = 100
            initial.marginTop = 10
            initial.marginBottom = 11
            initial.marginLeft = 12
            initial.marginRight = 13
            initial.theme = myTheme
            initial.axisX = axisX
            initial.axisY = axisY
            initial.addSeries(barInitial)

            waitForRendering(top)

            compare(initial.height, 100)
            compare(initial.width, 100)
            compare(initial.marginTop, 10)
            compare(initial.marginBottom, 11)
            compare(initial.marginLeft, 12)
            compare(initial.marginRight, 13)
            compare(initial.axisX, axisX)
            compare(initial.axisY, axisY)
            compare(initial.seriesList, [barInitial])
            compare(initial.theme, myTheme)
            compare(initial.theme.theme, GraphsTheme.Theme.QtGreenNeon)
            compare(initial.theme.colorScheme, Qt.Dark)
            compare(initial.theme.seriesColors.length, 5)
        }
    }

    TestCase {
        name: "GraphsView Initialized"

        function test_1_initialized() {
            compare(initialized.height, top.height)
            compare(initialized.width, top.width)
            compare(initialized.marginTop, 5)
            compare(initialized.marginBottom, 5)
            compare(initialized.marginLeft, 10)
            compare(initialized.marginRight, 10)
            compare(initialized.seriesList, [barInitial, lineInitial, areaInitial])
            compare(initialized.theme, myTheme)
            compare(initialized.theme.theme, GraphsTheme.Theme.QtGreenNeon)
            compare(initialized.theme.colorScheme, Qt.Dark)
            compare(initialized.theme.seriesColors.length, 5)
            compare(initialized.axisX, axisXInitial)
            compare(initialized.axisY, axisYInitial)
        }

        function test_2_initialized_change() {
            initialized.height = 100
            initialized.width = 100
            initialized.marginTop = 10
            initialized.marginBottom = 11
            initialized.marginLeft = 12
            initialized.marginRight = 13
            initialized.theme = newTheme
            initialized.axisX = axisX;
            initialized.axisY = axisY;
            initialized.removeSeries(barInitial)
            initialized.removeSeries(1) // areaInitial

            waitForRendering(top)

            compare(initialized.height, 100)
            compare(initialized.width, 100)
            compare(initialized.marginTop, 10)
            compare(initialized.marginBottom, 11)
            compare(initialized.marginLeft, 12)
            compare(initialized.marginRight, 13)
            compare(initialized.axisX, axisX)
            compare(initialized.axisY, axisY)
            compare(initialized.seriesList, [lineInitial])
            compare(initialized.theme, newTheme)
            compare(initialized.theme.theme, GraphsTheme.Theme.QtGreen)
            compare(initialized.theme.colorScheme, GraphsTheme.ColorScheme.Automatic)
            compare(initialized.theme.seriesColors.length, 5)
        }

        function test_3_initialized_change_to_invalid() {
            initialized.theme = null
            initialized.addSeries(areaInitial)
            initialized.addSeries(null)
            initialized.addSeries(myTheme)
            initialized.removeSeries(-1)
            initialized.removeSeries(15)

            waitForRendering(top)

            compare(initialized.seriesList, [lineInitial, areaInitial])
            // Using default theme, so not null and not any of ours.
            verify(initialized.theme !== null)
            verify(initialized.theme !== myTheme)
            verify(initialized.theme !== newTheme)
        }

        function test_4_initialized_change_to_null() {
            initialized.axisX = null;
            initialized.axisY = null;

            waitForRendering(top)

            compare(initial.axisX, null)
            compare(initial.axisY, null)
        }
    }
}
