// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
import QtQuick 2.0
import QtGraphs
import QtTest 1.0

Item {
    id: top
    height: 150
    width: 150

    property var empty: null
    property var basic: null
    property var common: null
    property var common_init: null

    function constructEmpty() {
        empty = Qt.createQmlObject("
import QtQuick 2.2
import QtGraphs
Surface3D {
}", top)
    }

    function constructBasic() {
        basic = Qt.createQmlObject("
import QtQuick 2.2
import QtGraphs
Surface3D {
anchors.fill: parent
flipHorizontalGrid: true
}", top)
        basic.anchors.fill = top
    }

    function constructCommon() {
        common = Qt.createQmlObject("
import QtQuick 2.2
import QtGraphs
Surface3D {
anchors.fill: parent
}", top)
        common.anchors.fill = top
    }

    function constructCommonInit() {
        common_init = Qt.createQmlObject("
import QtQuick 2.2
import QtGraphs
Surface3D {
anchors.fill: parent
selectionMode: Graphs3D.SelectionFlag.None
shadowQuality: Graphs3D.ShadowQuality.Low
msaaSamples: 2
theme: GraphsTheme { }
renderingMode: Graphs3D.RenderingMode.Indirect
measureFps: true
orthoProjection: false
aspectRatio: 3.0
optimizationHint: Graphs3D.OptimizationHint.Default
polar: false
radialLabelOffset: 2
horizontalAspectRatio: 0.2
locale: Qt.locale(\"UK\")
margin: 0.2
labelMargin: 0.2
lightColor: \"black\"
ambientLightStrength: 0.5
lightStrength: 10.0
shadowStrength: 50.0
}", top)
        common_init.anchors.fill = top
    }

    TestCase {
        name: "Surface3D Empty"
        when: windowShown

        function test_empty() {
            constructEmpty()
            compare(empty.width, 0, "width")
            compare(empty.height, 0, "height")
            compare(empty.seriesList.length, 0, "seriesList")
            compare(empty.selectedSeries, null, "selectedSeries")
            compare(empty.flipHorizontalGrid, false, "flipHorizontalGrid")
            compare(empty.axisX.orientation, Abstract3DAxis.AxisOrientation.X)
            compare(empty.axisZ.orientation, Abstract3DAxis.AxisOrientation.Z)
            compare(empty.axisY.orientation, Abstract3DAxis.AxisOrientation.Y)
            compare(empty.axisX.type, Abstract3DAxis.AxisType.Value)
            compare(empty.axisZ.type, Abstract3DAxis.AxisType.Value)
            compare(empty.axisY.type, Abstract3DAxis.AxisType.Value)
            waitForRendering(top)
            empty.destroy()
            waitForRendering(top)
        }
    }

    TestCase {
        name: "Surface3D Basic"
        when: windowShown

        function test_1_basic() {
            constructBasic()
            compare(basic.width, 150, "width")
            compare(basic.height, 150, "height")
            compare(basic.flipHorizontalGrid, true, "flipHorizontalGrid")
        }

        function test_2_change_basic() {
            basic.flipHorizontalGrid = false
            compare(basic.flipHorizontalGrid, false, "flipHorizontalGrid")
            waitForRendering(top)
            basic.destroy()
            waitForRendering(top)
        }
    }

    TestCase {
        name: "Surface3D Common"
        when: windowShown

        function test_1_common() {
            constructCommon()
            compare(common.selectionMode, Graphs3D.SelectionFlag.Item,
                    "selectionMode")
            compare(common.shadowQuality, Graphs3D.ShadowQuality.Medium,
                    "shadowQuality")
            compare(common.msaaSamples, 4, "msaaSamples")
            compare(common.theme.theme, GraphsTheme.Theme.QtGreen, "theme")
            compare(common.renderingMode, Graphs3D.RenderingMode.Indirect,
                    "renderingMode")
            compare(common.measureFps, false, "measureFps")
            compare(common.customItemList.length, 0, "customItemList")
            compare(common.orthoProjection, false, "orthoProjection")
            compare(common.selectedElement, Graphs3D.ElementType.None,
                    "selectedElement")
            compare(common.aspectRatio, 2.0, "aspectRatio")
            compare(common.optimizationHint, Graphs3D.OptimizationHint.Default,
                    "optimizationHint")
            compare(common.polar, false, "polar")
            compare(common.radialLabelOffset, 1, "radialLabelOffset")
            compare(common.horizontalAspectRatio, 0, "horizontalAspectRatio")
            compare(common.locale, Qt.locale("C"), "locale")
            compare(common.queriedGraphPosition, Qt.vector3d(0, 0, 0),
                    "queriedGraphPosition")
            compare(common.margin, -1, "margin")
            compare(common.labelMargin, 0.1, "labelMargin")
            compare(common.lightColor, "#ffffff", "lightColor")
            compare(common.ambientLightStrength, 0.25, "ambientLightStrength")
            compare(common.lightStrength, 5.0, "lightStrength")
            compare(common.shadowStrength, 25.0, "shadowStrength")

            common.destroy()
        }

        function test_2_change_common() {
            constructCommon()
            common.selectionMode = Graphs3D.SelectionFlag.Item
                    | Graphs3D.SelectionFlag.Row | Graphs3D.SelectionFlag.Slice
            common.shadowQuality = Graphs3D.ShadowQuality.SoftHigh
            compare(common.shadowQuality, Graphs3D.ShadowQuality.SoftHigh,
                    "shadowQuality")
            common.msaaSamples = 8
            compare(common.msaaSamples, 8, "msaaSamples")
            common.theme.theme = GraphsTheme.Theme.QtGreen
            // TODO: Seems to be causing crashes in testing - QTBUG-122089
            // common.renderingMode = Graphs3D.RenderingMode.DirectToBackground
            common.measureFps = true
            common.orthoProjection = true
            common.aspectRatio = 1.0
            common.optimizationHint = Graphs3D.OptimizationHint.Default
            common.polar = true
            common.radialLabelOffset = 2
            common.horizontalAspectRatio = 1
            common.locale = Qt.locale("FI")
            common.margin = 1.0
            common.labelMargin = 1.0
            common.lightColor = "#ff0000"
            common.ambientLightStrength = 0.5
            common.lightStrength = 10.0
            common.shadowStrength = 50.0
            compare(common.selectionMode,
                    Graphs3D.SelectionFlag.Item | Graphs3D.SelectionFlag.Row
                    | Graphs3D.SelectionFlag.Slice, "selectionMode")
            compare(common.shadowQuality, Graphs3D.ShadowQuality.None,
                    "shadowQuality") // Ortho disables shadows
            // TODO: Seems to be causing crashes in testing - QTBUG-122089
            // compare(common.msaaSamples, 0, "msaaSamples") // Rendering mode changes this to zero
            compare(common.theme.theme, GraphsTheme.Theme.QtGreen, "theme")
            // TODO: Seems to be causing crashes in testing - QTBUG-122089
            // compare(common.renderingMode, Graphs3D.RenderingMode.DirectToBackground, "renderingMode")
            compare(common.measureFps, true, "measureFps")
            compare(common.orthoProjection, true, "orthoProjection")
            compare(common.aspectRatio, 1.0, "aspectRatio")
            compare(common.optimizationHint, Graphs3D.OptimizationHint.Default,
                    "optimizationHint")
            compare(common.polar, true, "polar")
            compare(common.radialLabelOffset, 2, "radialLabelOffset")
            compare(common.horizontalAspectRatio, 1, "horizontalAspectRatio")
            compare(common.locale, Qt.locale("FI"), "locale")
            compare(common.margin, 1.0, "margin")
            compare(common.labelMargin, 1.0, "labelMargin")
            compare(common.lightColor, "#ff0000", "lightColor")
            compare(common.ambientLightStrength, 0.5, "ambientLightStrength")
            compare(common.lightStrength, 10.0, "lightStrength")
            compare(common.shadowStrength, 50.0, "shadowStrength")

            common.destroy()
        }

        function test_3_change_invalid_common() {
            constructCommon()
            common.selectionMode = Graphs3D.SelectionFlag.Row
                    | Graphs3D.SelectionFlag.Column | Graphs3D.SelectionFlag.Slice
            common.theme.type = -2
            common.renderingMode = -1
            common.measureFps = false
            common.orthoProjection = false
            common.aspectRatio = -1.0
            common.polar = false
            common.horizontalAspectRatio = -2
            compare(common.selectionMode, Graphs3D.SelectionFlag.Item,
                    "selectionMode")
            compare(common.theme.type, -2 /*Theme3D.Theme.Retro*/
                    , "theme") // TODO: Fix once QTRD-3367 is done
            compare(common.renderingMode,
                    -1 /*Graphs3D.RenderingMode.DirectToBackground*/
                    , "renderingMode") // TODO: Fix once QTRD-3367 is done
            compare(common.aspectRatio, -1.0 /*1.0*/
                    , "aspectRatio") // TODO: Fix once QTRD-3367 is done
            compare(common.horizontalAspectRatio, -2 /*1*/
                    ,
                    "horizontalAspectRatio") // TODO: Fix once QTRD-3367 is done

            common.ambientLightStrength = -1.0
            compare(common.ambientLightStrength, 0.25, "ambientLightStrength")
            common.ambientLightStrength = -1.1
            compare(common.ambientLightStrength, 0.25, "ambientLightStrength")
            common.lightStrength = 11.0
            compare(common.lightStrength, 5.0, "lightStrength")
            common.lightStrength = -1.0
            compare(common.lightStrength, 5.0, "lightStrength")
            common.shadowStrength = 101.0
            compare(common.shadowStrength, 25.0, "shadowStrength")
            common.shadowStrength = -1.0
            compare(common.shadowStrength, 25.0, "shadowStrength")

            common.destroy()
        }

        function test_4_common_initialized() {
            constructCommonInit()

            compare(common_init.selectionMode, Graphs3D.SelectionFlag.None,
                    "selectionMode")
            tryCompare(common_init, "shadowQuality", Graphs3D.ShadowQuality.Low)
            compare(common_init.msaaSamples, 2, "msaaSamples")
            compare(common_init.theme.theme,
                    GraphsTheme.Theme.QtGreen, "theme")
            compare(common_init.renderingMode, Graphs3D.RenderingMode.Indirect,
                    "renderingMode")
            compare(common_init.measureFps, true, "measureFps")
            compare(common_init.customItemList.length, 0, "customItemList")
            compare(common_init.orthoProjection, false, "orthoProjection")
            compare(common_init.aspectRatio, 3.0, "aspectRatio")
            compare(common_init.optimizationHint,
                    Graphs3D.OptimizationHint.Default, "optimizationHint")
            compare(common_init.polar, false, "polar")
            compare(common_init.radialLabelOffset, 2, "radialLabelOffset")
            compare(common_init.horizontalAspectRatio, 0.2,
                    "horizontalAspectRatio")
            compare(common_init.locale, Qt.locale("UK"), "locale")
            compare(common_init.margin, 0.2, "margin")
            compare(common_init.labelMargin, 0.2, "labelMargin")
            compare(common_init.lightColor, "#000000", "lightColor")
            compare(common_init.ambientLightStrength, 0.5,
                    "ambientLightStrength")
            compare(common_init.lightStrength, 10.0, "lightStrength")
            compare(common_init.shadowStrength, 50.0, "shadowStrength")

            common_init.destroy()
        }
    }
}
