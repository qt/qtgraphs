// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Universal
import AerospaceHubExample

ApplicationWindow {
    width: GlobalSettings.rootwidth
    height: GlobalSettings.rootheight
    visible: true
    title: "Wind Turbine Dashboard"

    FontLoader {
        id: interFont
        source: "./fonts/Inter-VariableFont_opsz,wght.ttf"
    }

    font.family: interFont.name

    // Data simulators
    PieSimulator {
        id: piesim
        max: 1000.0
        min: 1.0
        numberOfData: 3
        live: GlobalSettings.livedata
        deviation: 1
        updatePeriod: GlobalSettings.updateinterval * 3

        Component.onCompleted: {
            generateData()
        }
    }

    BarSimulator {
        id: barsim
        max: 33.0
        min: 1.0
        numberOfData: 3
        numberOfCategory: 12
        live: GlobalSettings.livedata
        deviation: 1
        updatePeriod: GlobalSettings.updateinterval * 50

        Component.onCompleted: {
            generateData()
        }
    }

    LineSimulator {
        id: linesim
        max: 2.0
        min: 0.0
        numberOfData: 25
        order: LineSimulator.SortingOrder.Ascending
        deviation: 2
        live: GlobalSettings.livedata
        updatePeriod: GlobalSettings.updateinterval * 10

        Component.onCompleted: {
            generateData()
            GlobalSettings.area2ddataready = true
            GlobalSettings.area2ddataready = false
        }

        onSimulateDataCompleted: {
            GlobalSettings.area2ddataready = true
            GlobalSettings.area2ddataready = false
        }
    }

    Data3DSimulator {
        id: barDataSimulator
        max: 10.0
        min: 1.0
        numberOfRows: 30
        numberOfColumns: 30
        dataModel: GlobalSettings.barDataModel
        live: GlobalSettings.livedata
        updatePeriod: GlobalSettings.updateinterval * 100
        deviation: 2

        Component.onCompleted: {
            generateData()
        }

        onSimulateDataCompleted: {
            GlobalSettings.bar3ddataready = true
        }
    }

    Data3DSimulator {
        id: scatterDataSimulator
        max: 30.0
        min: 1.0
        numberOfRows: 30
        numberOfColumns: 30
        numberOfData: 3
        dataModel: GlobalSettings.scatterDataModel
        live: GlobalSettings.livedata
        updatePeriod: GlobalSettings.updateinterval * 100
        deviation: 10

        Component.onCompleted: {
            generateData()
        }

        onSimulateDataCompleted: {
            GlobalSettings.scatter3ddataready = true
        }
    }

    Data3DSimulator {
        id: surfaceDataSimulator
        max: 10.0
        min: 0.0
        numberOfRows: 20
        numberOfColumns: 20
        dataModel: GlobalSettings.surfaceDataModel
        order: Data3DSimulator.SortingOrder.Ascending
        deviation: 50
        live: GlobalSettings.livedata
        updatePeriod: GlobalSettings.updateinterval * 100

        Component.onCompleted: {
            generateData()
        }

        onSimulateDataCompleted: {
            GlobalSettings.surface3ddataready = true
        }
    }

    Timer {
        running: GlobalSettings.livedata
        interval: GlobalSettings.updateinterval * 100
        repeat: true
        onTriggered: {
            GlobalSettings.winddir += Math.floor(Math.random() * 3) - 1
            if (GlobalSettings.winddir > 360)
                GlobalSettings.winddir = 0
            else if (GlobalSettings.winddir < 0)
                GlobalSettings.winddir = 360
        }
    }

    // Root item
    Item {
        id: root
        anchors.fill: parent

        state: "ShowDashboard"

        property alias settingsstate: settingscontent.settingsstate

        // Update global settings that rely on window size
        onHeightChanged: {
            GlobalSettings.rootheight = height
        }

        onWidthChanged: {
            GlobalSettings.rootwidth = width
        }

        // Grouping for vertical toolbar buttons
        ButtonGroup {
            buttons: viewbuttons.children
        }

        // Background image
        Image {
            anchors.fill: parent
            source: root.state === "ShowSettings" ? "images/settings-bg.png"
                                                  : "images/dashboard-bg.png"
            fillMode: Image.PreserveAspectCrop
        }

        // Everything except background image
        ColumnLayout {
            anchors.fill: parent
            anchors.leftMargin: GlobalSettings.doublespacing
            anchors.rightMargin: GlobalSettings.doublespacing
            anchors.bottomMargin: GlobalSettings.defaultspacing

            // Header; logos and title
            Item {
                id: header
                Layout.fillWidth: true
                Layout.preferredHeight: GlobalSettings.headerheight
                Layout.leftMargin: -GlobalSettings.halfspacing

                Image {
                    id: icon
                    anchors.verticalCenter: parent.verticalCenter
                    source: "images/turbine-logo.png"
                    height: GlobalSettings.headerheight * 0.61
                    sourceSize.width: 110
                    sourceSize.height: 110
                    fillMode: Image.PreserveAspectFit
                }

                Text {
                    anchors.verticalCenter: icon.verticalCenter
                    anchors.left: icon.right
                    anchors.leftMargin: 30
                    text: "AeroScope Hub"
                    color: "#EFEFEF"
                    verticalAlignment: Qt.AlignVCenter
                    font.pixelSize: GlobalSettings.fontsize54px
                    font.bold: true
                }

                Image {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    source: "images/BuiltWithQtLogo.png"
                    height: GlobalSettings.qtlogoheight
                    sourceSize.width: 242
                    sourceSize.height: 72
                    fillMode: Image.PreserveAspectFit
                }
            }

            // Everything except header and background image
            RowLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: GlobalSettings.doublespacing

                // Vertical toolbar
                Rectangle {
                    id: verticaltoolbar
                    Layout.preferredWidth: GlobalSettings.toolbarwidth
                    Layout.fillHeight: true
                    color: "#181818"
                    radius: GlobalSettings.defaultradius / 2

                    ColumnLayout {
                        id: viewbuttons
                        spacing: GlobalSettings.defaultspacing
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.topMargin: GlobalSettings.defaultspacing * 1.5
                        anchors.leftMargin: GlobalSettings.defaultspacing
                        anchors.rightMargin: GlobalSettings.defaultspacing

                        Button {
                            Layout.fillWidth: true
                            Layout.preferredHeight: width
                            icon.source: "images/dashboard-icon.png"
                            icon.color: checked ? "#181818" : "#F2F2F2"
                            autoExclusive: true
                            checkable: true
                            checked: root.state !== "ShowSettings"
                            enabled: root.state === "ShowSettings"
                            background: Rectangle {
                                anchors.centerIn: parent
                                color: parent.checked ? "#F2F2F2" : "#181818"
                                radius: width / 9
                                width: verticaltoolbar.width * 0.7
                                height: width
                            }
                            onClicked: {
                                root.state = "ShowDashboard"
                            }
                        }

                        Button {
                            Layout.fillWidth: true
                            Layout.preferredHeight: width
                            icon.source: "images/settings-icon.png"
                            icon.color: checked ? "#181818" : "#F2F2F2"
                            autoExclusive: true
                            checkable: true
                            checked: root.state === "ShowSettings"
                            background: Rectangle {
                                anchors.centerIn: parent
                                color: parent.checked ? "#F2F2F2" : "#181818"
                                radius: width / 9
                                width: verticaltoolbar.width * 0.7
                                height: width
                            }
                            onClicked: root.state = "ShowSettings"
                        }

                        Button {
                            Layout.fillWidth: true
                            Layout.preferredHeight: width
                            icon.source: "images/split-icon.png"
                            icon.color: "gray"
                            autoExclusive: true
                            checkable: true
                            enabled: false
                            background: Rectangle {
                                anchors.centerIn: parent
                                color: parent.checked ? "#EFEFEF" : "#181818"
                                radius: width / 9
                                width: verticaltoolbar.width * 0.7
                                height: width
                            }
                        }
                    }
                }

                // Everything except vertical toolbar, header, and background image
                ColumnLayout {
                    id: maincontentview
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    // Status row: back button (when applicable) and view name
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.preferredHeight: GlobalSettings.statusheaderheight
                        spacing: GlobalSettings.defaultspacing

                        Button {
                            icon.source: "images/back-icon.png"
                            icon.color: "#EFEFEF"
                            background: Rectangle {
                                anchors.centerIn: parent
                                width: parent.width * 1.1
                                height: width
                                radius: height / 6
                                color: "#80181818"
                                border.width: parent.hovered ? 1 : 0
                                border.color: "#EFEFEF"
                            }
                            visible: root.state !== "ShowDashboard"
                            onClicked: root.state = "ShowDashboard"
                        }

                        Label {
                            id: viewtext
                            text: "Dashboard"
                            font.pixelSize: GlobalSettings.fontsize28px
                            color: "#EFEFEF"
                            visible: root.state !== "ShowDashboard"
                            background: Button {
                                anchors.centerIn: parent
                                width: parent.width * 1.2
                                height: parent.height * 1.2
                                onClicked: root.state = "ShowDashboard"
                                background: Rectangle {
                                    radius: parent.height / 5
                                    color: parent.hovered ? "#181818" : "transparent"
                                }
                            }
                        }

                        Label {
                            id: viewtextdesc
                            text: "Dashboard: Turbine status"
                            font.pixelSize: GlobalSettings.fontsize28px
                            font.bold: true
                            color: "#EFEFEF"
                        }
                    }

                    // Main content
                    RowLayout {
                        id: maincontent
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        spacing: GlobalSettings.doublespacing

                        // Status area; turbine image, notification, status texts, and turbine name
                        Item {
                            id: turbineimage
                            Layout.preferredWidth: GlobalSettings.statusimagewidth
                            Layout.fillHeight: true

                            // Image
                            ImageItem {
                                anchors.fill: parent
                            }

                            // Notification
                            NotificationItem {
                                anchors.top: turbineimage.top
                                anchors.horizontalCenter: turbineimage.horizontalCenter
                                anchors.topMargin: GlobalSettings.doublespacing
                                width: turbineimage.width / 1.25
                            }

                            // Status
                            StatusItem {
                                anchors.centerIn: turbineimage
                                width: turbineimage.width / 1.5
                                height: turbineimage.width / 1.75
                            }

                            // Turbine name
                            Label {
                                anchors.bottom: turbineimage.bottom
                                anchors.right: turbineimage.right
                                anchors.bottomMargin: GlobalSettings.defaultspacing * 1.5
                                anchors.rightMargin: GlobalSettings.doublespacing
                                text: "Turbine " + GlobalSettings.turbineid
                                color: "#EFEFEF"
                                font.pixelSize: GlobalSettings.fontsize54px
                                font.bold: true
                            }
                        }

                        // Big single graph, when in correct state
                        BigGraphItem {
                            id: bigraph
                            Layout.preferredWidth: GlobalSettings.dashboardwidth
                            Layout.fillHeight: true
                            state: root.state
                            radius: GlobalSettings.defaultradius
                            piesimulator: piesim
                            barsimulator: barsim
                            linesimulator: linesim
                            detailsview: true
                        }

                        // Scrollable dashboard graph area, when in ShowDashboard state
                        Flickable {
                            id: flick
                            Layout.preferredWidth: GlobalSettings.dashboardwidth
                            Layout.fillHeight: true
                            clip: true

                            contentHeight: content.height

                            boundsBehavior: Flickable.StopAtBounds
                            interactive: true
                            visible: opacity > 0.5

                            Item {
                                id: content
                                width: parent.width
                                height: turbineimage.height * 1.5

                                GridLayout {
                                    id: grid
                                    columns: 2
                                    rowSpacing: GlobalSettings.doublespacing
                                    columnSpacing: GlobalSettings.doublespacing
                                    anchors.fill: parent

                                    // Graph 1: Power output over time
                                    SmallGraphItem {
                                        Layout.fillWidth: true
                                        Layout.fillHeight: true
                                        state: "Area2D"
                                        linesimulator: linesim
                                    }

                                    // Graph 2: Energy production x Turbine
                                    SmallGraphItem {
                                        Layout.fillWidth: true
                                        Layout.fillHeight: true
                                        state: "Bar2D"
                                        barsimulator: barsim
                                    }

                                    // Graph 3: Power curve visualization
                                    SmallGraphItem {
                                        Layout.fillWidth: true
                                        Layout.fillHeight: true
                                        state: "Surface3D"
                                    }

                                    // Graph 4: Fleet-level performance distribution
                                    SmallGraphItem {
                                        Layout.fillWidth: true
                                        Layout.fillHeight: true
                                        state: "Scatter3D"
                                    }

                                    // Graph 5: Component failure distribution
                                    SmallGraphItem {
                                        Layout.fillWidth: true
                                        Layout.fillHeight: true
                                        state: "Pie2D"
                                        piesimulator: piesim
                                    }

                                    // Graph 6: Energy production breakdown x Turbine
                                    SmallGraphItem {
                                        Layout.fillWidth: true
                                        Layout.fillHeight: true
                                        state: "Bar3D"
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        // Settings
        SettingsItem {
            id: settingscontent
            anchors.fill: parent
            anchors.leftMargin: GlobalSettings.settingsleftmargin
            anchors.rightMargin: GlobalSettings.defaultmargin
            anchors.topMargin: GlobalSettings.settingstopmargin
            anchors.bottomMargin: GlobalSettings.defaultmargin
            opacity: 0
            piesimulator: piesim
            barsimulator: barsim
            linesimulator: linesim
        }

        states: [
            State {
                name: "ShowDashboard"
            },
            State {
                name: "ShowSettings"
                PropertyChanges {
                    maincontent.opacity: 0
                    settingscontent.opacity: 1
                }
                PropertyChanges {
                    viewtextdesc.text: "/  Settings"
                }
            },
            State {
                name: "ShowArea2D"
                PropertyChanges {
                    flick.opacity: 0
                }
                PropertyChanges {
                    bigraph.opacity: 1
                }
                PropertyChanges {
                    viewtextdesc.text: "/  Power output over time"
                }
            },
            State {
                name: "ShowBar2D"
                PropertyChanges {
                    flick.opacity: 0
                }
                PropertyChanges {
                    bigraph.opacity: 1
                }
                PropertyChanges {
                    viewtextdesc.text: "/  Energy production x Turbine"
                }
            },
            State {
                name: "ShowSurface3D"
                PropertyChanges {
                    flick.opacity: 0
                }
                PropertyChanges {
                    bigraph.opacity: 1
                }
                PropertyChanges {
                    viewtextdesc.text: "/  Power curve visualization"
                }
            },
            State {
                name: "ShowScatter3D"
                PropertyChanges {
                    flick.opacity: 0
                }
                PropertyChanges {
                    bigraph.opacity: 1
                }
                PropertyChanges {
                    viewtextdesc.text: "/  Fleet-level performance distribution"
                }
            },
            State {
                name: "ShowPie2D"
                PropertyChanges {
                    flick.opacity: 0
                }
                PropertyChanges {
                    bigraph.opacity: 1
                }
                PropertyChanges {
                    viewtextdesc.text: "/  Component failure distribution"
                }
            },
            State {
                name: "ShowBar3D"
                PropertyChanges {
                    flick.opacity: 0
                }
                PropertyChanges {
                    bigraph.opacity: 1
                }
                PropertyChanges {
                    viewtextdesc.text: "/  Energy production breakdown x Turbine"
                }
            }
        ]

        // Use transitions to bring the selected graph to front
        transitions: [
            Transition {
                NumberAnimation {
                    properties: "opacity"
                    easing.type: Easing.InOutCubic
                    duration: 1000
                }
            }
        ]
    }
}
