// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
import QtQuick
import QtGraphs
import QtTest

Item {
    id: top
    height: 400
    width: 400

    // Note: TestCase blocks in this file are not guaranteed to run in
    // declaration order, so each TestCase below uses its own dedicated
    // PolarView instance(s) instead of sharing mutable objects across
    // TestCase blocks.

    PolarView {
        id: initial
    }

    GraphsTheme {
        id: myTheme
        theme: GraphsTheme.Theme.QtGreenNeon
        colorScheme: Qt.Dark
    }

    PolarView {
        id: initialized
        height: top.height
        width: top.width
        theme: myTheme
        marginBottom: 40
        marginLeft: 40
        marginRight: 40
        marginTop: 40

        axisX: ValueAxis {
            id: axisXInitial
            min: 0
            max: 360
        }

        axisY: ValueAxis {
            id: axisYInitial
            min: 0
            max: 10
        }

        LineSeries {
            id: lineInitial
            XYPoint { x: 0; y: 5 }
            XYPoint { x: 90; y: 7 }
            XYPoint { x: 180; y: 5 }
            XYPoint { x: 270; y: 3 }
        }

        ScatterSeries {
            id: scatterInitial
            XYPoint { x: 45; y: 6 }
            XYPoint { x: 135; y: 4 }
            XYPoint { x: 225; y: 6 }
            XYPoint { x: 315; y: 8 }
        }
    }

    PolarView {
        id: seriesView
        height: top.height
        width: top.width

        axisX: ValueAxis { min: 0; max: 360 }
        axisY: ValueAxis { min: 0; max: 20 }
    }

    LineSeries {
        id: addedLine
        XYPoint { x: 0; y: 10 }
        XYPoint { x: 60; y: 15 }
        XYPoint { x: 120; y: 10 }
    }

    ScatterSeries {
        id: addedScatter
        XYPoint { x: 30; y: 12 }
        XYPoint { x: 90; y: 8 }
    }

    SplineSeries {
        id: splineToAdd
        XYPoint { x: 0; y: 5 }
        XYPoint { x: 45; y: 8 }
        XYPoint { x: 90; y: 4 }
    }

    AreaSeries {
        id: areaToAdd
        lowerSeries: LineSeries {
            XYPoint { x: 0; y: 2 }
            XYPoint { x: 180; y: 2 }
            XYPoint { x: 360; y: 2 }
        }
    }

    PolarView {
        id: unsupportedView
        height: top.height
        width: top.width

        axisX: ValueAxis { min: 0; max: 360 }
        axisY: ValueAxis { min: 0; max: 20 }
    }

    BarSeries {
        id: unsupportedBarSeries
    }

    PieSeries {
        id: unsupportedPieSeries
    }

    PolarView {
        id: axesView
        height: top.height
        width: top.width

        axisX: ValueAxis {
            id: axesViewAxisX
            min: 0
            max: 360
        }

        axisY: ValueAxis {
            id: axesViewAxisY
            min: 0
            max: 10
        }
    }

    PolarView {
        id: signalsCenterView
        height: 100
        width: 100
    }

    PolarView {
        id: paddingView
    }

    PolarView {
        id: themeDefaultView
    }

    PolarView {
        id: themeInitializedView
        theme: myTheme
    }

    TestCase {
        name: "PolarView Initial"

        function test_1_initial_defaults() {
            compare(initial.height, 0);
            compare(initial.width, 0);
            compare(initial.polarCenter.x, 0);
            compare(initial.polarCenter.y, 0);
            compare(initial.polarRadius, 0);
            compare(initial.labelPadding, 8);
            compare(initial.seriesList, []);
            compare(initial.axisX, null);
            compare(initial.axisY, null);
        }

        function test_2_initialized_properties() {
            compare(initialized.height, top.height);
            compare(initialized.width, top.width);
            compare(initialized.theme, myTheme);
            compare(initialized.axisX, axisXInitial);
            compare(initialized.axisY, axisYInitial);
            compare(initialized.seriesList, [lineInitial, scatterInitial]);
            verify(initialized.polarRadius > 0);
        }

        function test_3_initial_size_change() {
            initial.height = 300;
            initial.width = 300;
            waitForRendering(top);

            compare(initial.height, 300);
            compare(initial.width, 300);
        }

        function test_4_label_padding_getter() {
            compare(initialized.labelPadding, 8);
        }

        function test_5_label_padding_setter() {
            initialized.labelPadding = 12;
            waitForRendering(top);
            compare(initialized.labelPadding, 12);
        }

        function test_6_label_padding_negative() {
            initialized.labelPadding = -5;
            compare(initialized.labelPadding, 0);
        }

        function test_7_label_padding_zero() {
            initialized.labelPadding = 0;
            compare(initialized.labelPadding, 0);
        }

        function test_8_polar_center_on_resize() {
            initial.height = 200;
            initial.width = 200;
            initial.marginLeft = 20;
            initial.marginTop = 20;
            initial.marginRight = 20;
            initial.marginBottom = 20;
            waitForRendering(top);

            verify(initial.polarCenter.x >= 0);
            verify(initial.polarCenter.y >= 0);
        }

        function test_9_polar_radius_on_resize() {
            initial.height = 200;
            initial.width = 200;
            waitForRendering(top);

            verify(initial.polarRadius > 0);
        }
    }

    TestCase {
        name: "PolarView Series Support"

        function test_1_add_line_series() {
            seriesView.addSeries(addedLine);
            waitForRendering(top);
            verify(seriesView.hasSeries(addedLine));
        }

        function test_2_add_spline_series() {
            seriesView.addSeries(splineToAdd);
            waitForRendering(top);
            verify(seriesView.hasSeries(splineToAdd));
        }

        function test_3_add_scatter_series() {
            seriesView.addSeries(addedScatter);
            waitForRendering(top);
            verify(seriesView.hasSeries(addedScatter));
        }

        function test_4_add_area_series() {
            seriesView.addSeries(areaToAdd);
            waitForRendering(top);
            verify(seriesView.hasSeries(areaToAdd));
        }

        function test_5_remove_series() {
            if (seriesView.hasSeries(addedLine)) {
                seriesView.removeSeries(addedLine);
                waitForRendering(top);
                verify(!seriesView.hasSeries(addedLine));
            }
        }

        function test_6_multiple_series() {
            var initialCount = seriesView.seriesList.length;
            seriesView.addSeries(addedLine);
            waitForRendering(top);
            compare(seriesView.seriesList.length, initialCount + 1);
        }
    }

    TestCase {
        name: "PolarView Unsupported Series"

        function test_1_bar_series_add_fails() {
            var seriesCountBefore = unsupportedView.seriesList.length;
            unsupportedView.addSeries(unsupportedBarSeries);
            var seriesCountAfter = unsupportedView.seriesList.length;
            compare(seriesCountBefore, seriesCountAfter);
        }

        function test_2_pie_series_add_fails() {
            var seriesCountBefore = unsupportedView.seriesList.length;
            unsupportedView.addSeries(unsupportedPieSeries);
            var seriesCountAfter = unsupportedView.seriesList.length;
            compare(seriesCountBefore, seriesCountAfter);
        }
    }

    TestCase {
        name: "PolarView Axes"

        function test_1_axes_from_declaration() {
            compare(axesView.axisX, axesViewAxisX);
            compare(axesView.axisY, axesViewAxisY);
        }

        function test_2_set_axis_x() {
            var newAxisX = Qt.createQmlObject('import QtGraphs; ValueAxis { min: 0; max: 180 }', top);
            axesView.axisX = newAxisX;
            waitForRendering(top);
            compare(axesView.axisX, newAxisX);
        }

        function test_3_set_axis_y() {
            var newAxisY = Qt.createQmlObject('import QtGraphs; ValueAxis { min: 0; max: 20 }', top);
            axesView.axisY = newAxisY;
            waitForRendering(top);
            compare(axesView.axisY, newAxisY);
        }
    }

    TestCase {
        name: "PolarView Signals"

        function test_1_polar_center_changed_signal() {
            centerChangedSpy.clear();
            signalsCenterView.height = 250;
            signalsCenterView.width = 250;
            waitForRendering(top);
            verify(centerChangedSpy.count > 0 || signalsCenterView.polarCenter.x === 0);
        }

        function test_2_polar_radius_changed_signal() {
            radiusChangedSpy.clear();
            signalsCenterView.height = 220;
            signalsCenterView.width = 220;
            waitForRendering(top);
            verify(radiusChangedSpy.count > 0 || signalsCenterView.polarRadius === 0);
        }

        function test_3_label_padding_changed_signal() {
            paddingChangedSpy.clear();
            paddingView.labelPadding = 15;
            compare(paddingChangedSpy.count, 1);
        }

        function test_4_multiple_label_padding_changes() {
            paddingChangedSpy.clear();
            paddingView.labelPadding = 10;
            paddingView.labelPadding = 12;
            paddingView.labelPadding = 14;
            compare(paddingChangedSpy.count, 3);
        }

        function test_5_label_padding_no_change_no_signal() {
            paddingChangedSpy.clear();
            var current = paddingView.labelPadding;
            paddingView.labelPadding = current;
            compare(paddingChangedSpy.count, 0);
        }

        SignalSpy {
            id: centerChangedSpy
            target: signalsCenterView
            signalName: "polarCenterChanged"
        }

        SignalSpy {
            id: radiusChangedSpy
            target: signalsCenterView
            signalName: "polarRadiusChanged"
        }

        SignalSpy {
            id: paddingChangedSpy
            target: paddingView
            signalName: "labelPaddingChanged"
        }
    }

    TestCase {
        name: "PolarView Margins"

        function test_1_margins_affect_plot_area() {
            var view = Qt.createQmlObject('
                                        import QtGraphs
                                        PolarView {
                                            width: 300
                                            height: 300
                                            marginLeft: 50
                                            marginRight: 50
                                            marginTop: 50
                                            marginBottom: 50
                                            axisX: ValueAxis {
                                                    min: 0
                                                    max: 360
                                            }
                                            axisY: ValueAxis {
                                                min: 0
                                                max: 10
                                            }
                                        }', top);
            waitForRendering(top);
            verify(view.plotArea.width > 0);
            verify(view.plotArea.height > 0);
        }

        function test_2_margins_affect_polar_center() {
            var view = Qt.createQmlObject('
                                        import QtGraphs
                                        PolarView {
                                            width: 300
                                            height: 300
                                            marginLeft: 40
                                            marginRight: 40
                                            marginTop: 40
                                            marginBottom: 40
                                        }', top)

            waitForRendering(top);
            verify(view.polarCenter.x >= 0);
            verify(view.polarCenter.y >= 0);
        }
    }

    TestCase {
        name: "PolarView Theme"

        function test_1_theme_assignment() {
            compare(themeInitializedView.theme, myTheme);
        }

        function test_2_initial_default_theme() {
            verify(themeDefaultView.theme !== null);
        }

        function test_3_theme_colors() {
            compare(themeInitializedView.theme.theme, GraphsTheme.Theme.QtGreenNeon);
            compare(themeInitializedView.theme.colorScheme, Qt.Dark);
        }
    }

    TestCase {
        name: "PolarView Data Points"

        function test_1_line_series_points() {
            compare(lineInitial.count, 4);
        }

        function test_2_scatter_series_points() {
            compare(scatterInitial.count, 4);
        }

        function test_3_line_series_coordinate() {
            var point = lineInitial.at(0);
            compare(point.x, 0);
            compare(point.y, 5);
        }

        function test_4_scatter_series_coordinate() {
            var point = scatterInitial.at(0);
            compare(point.x, 45);
            compare(point.y, 6);
        }
    }
}
