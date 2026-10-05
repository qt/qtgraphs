// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtGraphs
import Qml2DXYModelMapperExample

// Manual test for QXYModelMapper not rebuilding the series more than needed.
//
// A rebuild starts with series.clear() and refills the series with one bulk append, so it shows up
// as a bulk pointsRemoved/pointsAdded signal. Single point edits only emit the singular
// pointRemoved/pointAdded/pointReplaced signals. The "bulk" counters are therefore the rebuild
// indicator: after an edit that is not supposed to rebuild, they must not move.
Window {
    id: root

    property int bulkRemoved: 0
    property int bulkAdded: 0
    property int singleAdded: 0
    property int singleRemoved: 0
    property int singleReplaced: 0
    property int nextX: 100

    function resetCounters() {
        bulkRemoved = 0
        bulkAdded = 0
        singleAdded = 0
        singleRemoved = 0
        singleReplaced = 0
        nextX = 100
    }

    width: 1200
    height: 760
    visible: true
    title: "QML 2D XYModelMapper test"
    color: "#404040"

    component Hint: Text {
        Layout.fillWidth: true
        color: "#c0c0c0"
        font.pixelSize: 11
        wrapMode: Text.WordWrap
    }

    component Header: Text {
        Layout.topMargin: 8
        color: "white"
        font.bold: true
    }

    XYMapperModel {
        id: myModel
    }

    Connections {
        target: lineSeries
        function onPointsRemoved() { root.bulkRemoved++ }
        function onPointsAdded() { root.bulkAdded++ }
        function onPointAdded() { root.singleAdded++ }
        function onPointRemoved() { root.singleRemoved++ }
        function onPointReplaced() { root.singleReplaced++ }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        // Model view
        ColumnLayout {
            Layout.fillHeight: true
            Layout.preferredWidth: 230
            spacing: 0

            HorizontalHeaderView {
                syncView: tableView
                Layout.leftMargin: verticalHeader.width
                Layout.preferredWidth: tableView.width
                delegate: Text {
                    required property string display
                    padding: 3
                    color: "white"
                    text: display
                    horizontalAlignment: Text.AlignHCenter
                }
            }
            RowLayout {
                spacing: 0
                VerticalHeaderView {
                    id: verticalHeader
                    syncView: tableView
                    Layout.fillHeight: true
                    delegate: Text {
                        required property string display
                        padding: 3
                        color: "white"
                        text: display
                        horizontalAlignment: Text.AlignHCenter
                    }
                }
                TableView {
                    id: tableView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    reuseItems: false
                    model: myModel
                    delegate: Rectangle {
                        required property string display
                        implicitWidth: 70
                        implicitHeight: 24
                        border.width: 1
                        color: "#f0f0f0"
                        Text {
                            anchors.fill: parent
                            anchors.leftMargin: 4
                            verticalAlignment: Text.AlignVCenter
                            elide: Text.ElideRight
                            text: Number(parent.display).toFixed(2)
                        }
                    }
                }
            }
        }

        // Chart
        GraphsView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            axisX: ValueAxis {
                min: 0
                max: 12
                tickInterval: 1
            }
            axisY: ValueAxis {
                min: -2
                max: 2
                tickInterval: 1
            }

            LineSeries {
                id: lineSeries
                width: 2
                color: "#80d0ff"
                pointDelegate: Rectangle {
                    width: 8
                    height: 8
                    radius: 4
                    color: "#80d0ff"
                }
            }

            XYModelMapper {
                id: mapper
                series: lineSeries
                model: myModel
                first: 0
                count: -1
                xSection: 0
                ySection: 1
                orientation: Qt.Vertical
            }
        }

        // Controls
        ScrollView {
            Layout.fillHeight: true
            Layout.preferredWidth: 300
            contentWidth: availableWidth
            clip: true

            ColumnLayout {
                width: parent.width
                spacing: 4

                Header { text: "Counters (bulk = series rebuilt)" }
                GridLayout {
                    columns: 2
                    Text { text: "bulk pointsRemoved"; color: "white" }
                    Text { text: root.bulkRemoved; color: root.bulkRemoved ? "#ff8080" : "#80ff80" }
                    Text { text: "bulk pointsAdded"; color: "white" }
                    Text { text: root.bulkAdded; color: root.bulkAdded ? "#ff8080" : "#80ff80" }
                    Text { text: "pointAdded"; color: "white" }
                    Text { text: root.singleAdded; color: "white" }
                    Text { text: "pointRemoved"; color: "white" }
                    Text { text: root.singleRemoved; color: "white" }
                    Text { text: "pointReplaced"; color: "white" }
                    Text { text: root.singleReplaced; color: "white" }
                    Text { text: "model resets emitted"; color: "white" }
                    Text { text: myModel.modelResetCount; color: "white" }
                    Text { text: "series count"; color: "white" }
                    Text { text: lineSeries.count; color: "white" }
                    Text { text: "model rows"; color: "white" }
                    Text { text: myModel.rowCount(); color: "white" }
                }
                Button {
                    text: "Reset counters"
                    onClicked: root.resetCounters()
                }

                Header { text: "Model type" }
                CheckBox {
                    text: "Model emits reset on insert/remove"
                    palette.windowText: "white"
                    checked: myModel.resetOnStructuralChange
                    onToggled: myModel.resetOnStructuralChange = checked
                }
                Hint {
                    text: "On: like some custom/QML models. Before the fix, every series edit below "
                          + "rebuilt the series with this on."
                }

                Header { text: "Edit the series" }
                Hint { text: "Expect: only the single-point counters move; bulk counters stay put." }
                Flow {
                    Layout.fillWidth: true
                    spacing: 4
                    Button {
                        text: "Append point"
                        onClicked: lineSeries.append(lineSeries.count % 12, Math.random() * 3 - 1.5)
                    }
                    Button {
                        text: "Remove last"
                        onClicked: lineSeries.remove(lineSeries.count - 1)
                    }
                    Button {
                        text: "Replace first"
                        onClicked: lineSeries.replace(0, Qt.point(0, Math.random() * 3 - 1.5))
                    }
                }

                Header { text: "Edit the model" }
                Hint { text: "Expect: the chart follows incrementally; bulk counters stay put." }
                Flow {
                    Layout.fillWidth: true
                    spacing: 4
                    Button {
                        text: "Add row"
                        onClicked: myModel.addRow(myModel.rowCount() % 12, Math.random() * 3 - 1.5)
                    }
                    Button {
                        text: "Remove last row"
                        onClicked: myModel.removeLastRow()
                    }
                    Button {
                        text: "Edit random y"
                        enabled: myModel.rowCount() > 0
                        onClicked: myModel.editCell(Math.floor(Math.random() * myModel.rowCount()),
                                                    1, Math.random() * 3 - 1.5)
                    }
                }

                Header { text: "Model reset" }
                Flow {
                    Layout.fillWidth: true
                    spacing: 4
                    Button {
                        text: "Reset, unchanged"
                        onClicked: myModel.resetNow()
                    }
                    Button {
                        text: "Silent edit + reset"
                        enabled: myModel.rowCount() > 0
                        onClicked: {
                            myModel.setCellSilently(0, 1, Math.random() * 3 - 1.5)
                            myModel.resetNow()
                        }
                    }
                }
                Hint {
                    text: "Unchanged reset: bulk counters stay put (the series already matches). "
                          + "Silent edit + reset: the first point moves and bulk counters go up by 1."
                }

                Header { text: "Mapper properties" }
                GridLayout {
                    columns: 2
                    Text { text: "first"; color: "white" }
                    SpinBox {
                        from: 0
                        to: 20
                        value: mapper.first
                        onValueModified: mapper.first = value
                    }
                    Text { text: "count (-1 = all)"; color: "white" }
                    SpinBox {
                        from: -1
                        to: 20
                        value: mapper.count
                        onValueModified: mapper.count = value
                    }
                    Text { text: "xSection"; color: "white" }
                    SpinBox {
                        from: -1
                        to: 3
                        value: mapper.xSection
                        onValueModified: mapper.xSection = value
                    }
                    Text { text: "ySection"; color: "white" }
                    SpinBox {
                        from: -1
                        to: 3
                        value: mapper.ySection
                        onValueModified: mapper.ySection = value
                    }
                }
                Flow {
                    Layout.fillWidth: true
                    spacing: 4
                    Button {
                        text: mapper.orientation === Qt.Vertical ? "Orientation: vertical"
                                                                 : "Orientation: horizontal"
                        onClicked: mapper.orientation = mapper.orientation === Qt.Vertical
                                   ? Qt.Horizontal : Qt.Vertical
                    }
                    Button {
                        text: "Re-apply current values"
                        onClicked: {
                            mapper.first = mapper.first
                            mapper.count = mapper.count
                            mapper.xSection = mapper.xSection
                            mapper.ySection = mapper.ySection
                            mapper.orientation = mapper.orientation
                            mapper.model = myModel
                            mapper.series = lineSeries
                        }
                    }
                }
                Hint {
                    text: "Changing a value rebuilds once (bulk counters +1; no change if the "
                          + "result is identical). Re-applying unchanged values must change nothing. "
                          + "Horizontal maps rows 0/1 of the 3-column model, so only 3 points show."
                }
            }
        }
    }
}
