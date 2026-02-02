// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

pragma Singleton

import QtGraphs
import QtQuick
import AerospaceHubExample

QtObject {
    id: global

    // Window size
    property int rootwidth: 1920
    property int rootheight: 1080

    // Component heights
    property int headerheight: rootheight / 6                   // 180
    property int qtlogoheight: rootheight / 10                  // 108
    property int statusheaderheight: rootheight / 20            // 54
    property int notificationheight: rootheight / 27            // 40
    property int graphdetailheaderheight: rootheight / 12       // 90
    property int graphdetailfooterheight: rootheight / 32       // 34
    property int comboboxheight: graphdetailheaderheight / 2    // 45
    property int dashboardgraphheaderheight: rootheight / 60    // 18
    property int settingssliderheight: 4                        // 4
    property int settingssliderhandleheight: rootheight / 90    // 12
    property int settingsitemheight: rootheight / 34            // 32
    property int settingscheckboxheight: rootheight / 45        // 24
    property int settingscomboboxheight: settingsitemheight     // 32

    // Component widths
    property int settingsgraphwidth: rootwidth * 0.6                    // 1152
    property int settingpanewidth: rootwidth / 8                        // 240
    property int graphdetailcomboboxwidth: rootwidth / 9                // 213
    property int dashboardwidth: rootwidth - statusimagewidth
                                 - toolbarwidth - defaultspacing * 8    // 1040
    property int toolbarwidth: rootwidth / 24                           // 80
    property int statusimagewidth: rootwidth / 3.345                    // 574
    property int settingssliderwidth: graphdetailcomboboxwidth          // 213
    property int settingscomboboxwidth: settingssliderwidth / 1.5       // 142
    property int settingscheckboxwidth: rootwidth / 30                  // 64

    // Default visual dimensions
    property int defaultspacing: rootheight / 54        // 20
    property int halfspacing: defaultspacing / 2        // 10
    property int quarterspacing: halfspacing / 2        // 5
    property int doublespacing: defaultspacing * 2      // 40
    property int statusspacing: halfspacing             // 10
    property int defaultradius: doublespacing           // 40
    property int quarterradius: halfspacing             // 10
    property int defaultmargin: doublespacing           // 40
    property int doublemargin: doublespacing * 2        // 80

    // Component margins
    property int settingsleftmargin: defaultspacing * 4 + toolbarwidth                  // 160
    property int settingstopmargin: defaultspacing / 2 + headerheight                   // 190
    property int graphdetailmargin: statusheaderheight                                  // 54
    property int settingscomboboxmargin: defaultspacing + graphdetailheaderheight / 2   // 65

    // Font sizes
    property real fontsize54px: rootwidth / 35.5
    property real fontsize36px: rootwidth / 53
    property real fontsize28px: rootwidth / 68.5
    property real fontsize24px: rootwidth / 80
    property real fontsize20px: rootwidth / 96
    property real fontsize18px: rootwidth / 106.7
    property real fontsize14px: rootwidth / 137
    property real fontsize12px: rootwidth / 160

    // 3D graph defaults
    property int defaultzoom: 120
    property var defaultpreset: Graphs3D.CameraPreset.IsometricRight

    // Currently active graph
    property var activegraph
    property bool is3dgraph

    // Custom gradient for 3D graphs
    property var customgradient: Gradient {
        GradientStop { position: 0.0; color: "black" }
        GradientStop { position: 0.33; color: "green" }
        GradientStop { position: 0.67; color: "yellow" }
        GradientStop { position: 1.0; color: "red" }
    }

    // 3D graph color style
    property int colorstyle: GraphsTheme.ColorStyle.RangeGradient

    // Simulated data in data models
    property bool bar3ddataready: false
    property var barDataModel: SimulatedDataModel {
        rowRole: "row"
        columnRole: "column"
        valueRole: "value"
    }

    property bool scatter3ddataready: false
    property var scatterDataModel: SimulatedDataModel {
        rowRole: "x"
        columnRole: "y"
        valueRole: "z"
    }

    property bool surface3ddataready: false
    property var surfaceDataModel: SimulatedDataModel {
        rowRole: "row"
        columnRole: "column"
        valueRole: "y"
    }

    property bool area2ddataready: false

    // Turbine status
    property string turbineid: "36.12"
    property int winddir: 254
    property real windspeed: 12

    // Common graph theme
    property var theme: GraphsTheme {
        plotAreaBackgroundVisible: false
        backgroundVisible: false
        colorScheme: GraphsTheme.ColorScheme.Dark
        theme: GraphsTheme.Theme.BlueSeries
        labelTextColor: "#AEAEAE"
        labelBackgroundVisible: false
        labelFont.pointSize: global.fontsize28px
        grid.mainColor: "#AEAEAE"
        grid.mainWidth: 1
        baseGradients: [ global.customgradient ]
    }
}
