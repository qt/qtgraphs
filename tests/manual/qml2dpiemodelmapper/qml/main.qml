// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtGraphs
import Qml2DPieModelMapperExample

// Manual test for QPieModelMapper not rebuilding the series more than needed.
//
// A rebuild starts with series.clear() and refills the series, so it shows up as a removed/added
// signal carrying every slice. Single edits only carry one slice. The "bulk" counters (signals
// with more than one slice) are therefore the rebuild indicator: after an edit that is not
// supposed to rebuild, they must not move. The slice identity check is the second indicator: a
// rebuild destroys the slice objects, an in-place update keeps them.
Window {
    id: root

    property int bulkRemoved: 0
    property int bulkAdded: 0
    property int singleAdded: 0
    property int singleRemoved: 0
    property int replaced: 0
    property int nextSlice: 100

    property var snapshot: []
    property int preserved: 0

    function resetCounters() {
        bulkRemoved = 0
        bulkAdded = 0
        singleAdded = 0
        singleRemoved = 0
        replaced = 0
    }

    function showLabels() {
        for (let i = 0; i < pieSeries.count; ++i)
            pieSeries.at(i).labelVisible = true
    }

    function takeSnapshot() {
        const slices = []
        for (let i = 0; i < pieSeries.count; ++i)
            slices.push(pieSeries.at(i))
        snapshot = slices
        updateIdentity()
    }

    // A destroyed slice reads as null in JS, so only live slices still in the series are counted.
    function updateIdentity() {
        let alive = 0
        for (const slice of snapshot) {
            if (!slice)
                continue
            for (let i = 0; i < pieSeries.count; ++i) {
                if (pieSeries.at(i) === slice) {
                    ++alive
                    break
                }
            }
        }
        preserved = alive
    }

    function randomValue() {
        return Math.round((Math.random() * 14 + 1) * 10) / 10
    }

    width: 1200
    height: 760
    visible: true
    title: "QML 2D PieModelMapper test"
    color: "#404040"

    Component.onCompleted: {
        showLabels()
        resetCounters()
    }

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

    PieMapperModel {
        id: myModel
    }

    Connections {
        target: pieSeries
        function onAdded(slices) {
            if (slices.length > 1)
                root.bulkAdded++
            else
                root.singleAdded++
            root.showLabels()
            Qt.callLater(root.updateIdentity)
        }
        function onRemoved(slices) {
            if (slices.length > 1)
                root.bulkRemoved++
            else
                root.singleRemoved++
            Qt.callLater(root.updateIdentity)
        }
        function onReplaced() {
            root.replaced++
            root.showLabels()
            Qt.callLater(root.updateIdentity)
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        // Model view
        ColumnLayout {
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.preferredWidth: 260
            Layout.maximumWidth: 260
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
                        required property var display
                        implicitWidth: 70
                        implicitHeight: 24
                        border.width: 1
                        color: "#f0f0f0"
                        Text {
                            anchors.fill: parent
                            anchors.leftMargin: 4
                            verticalAlignment: Text.AlignVCenter
                            elide: Text.ElideRight
                            text: typeof parent.display === "number"
                                  ? parent.display.toFixed(2) : parent.display
                        }
                    }
                }
            }
        }

        // Chart
        GraphsView {
            Layout.fillWidth: true
            Layout.minimumWidth: 200
            Layout.fillHeight: true

            PieSeries {
                id: pieSeries
            }

            PieModelMapper {
                id: mapper
                series: pieSeries
                model: myModel
                first: 0
                count: -1
                labelsSection: 0
                valuesSection: 1
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
                    Text { text: "bulk removed"; color: "white" }
                    Text { text: root.bulkRemoved; color: root.bulkRemoved ? "#ff8080" : "#80ff80" }
                    Text { text: "bulk added"; color: "white" }
                    Text { text: root.bulkAdded; color: root.bulkAdded ? "#ff8080" : "#80ff80" }
                    Text { text: "single added"; color: "white" }
                    Text { text: root.singleAdded; color: "white" }
                    Text { text: "single removed"; color: "white" }
                    Text { text: root.singleRemoved; color: "white" }
                    Text { text: "replaced"; color: "white" }
                    Text { text: root.replaced; color: "white" }
                    Text { text: "model resets emitted"; color: "white" }
                    Text { text: myModel.modelResetCount; color: "white" }
                    Text { text: "series count"; color: "white" }
                    Text { text: pieSeries.count; color: "white" }
                    Text { text: "model rows"; color: "white" }
                    Text { text: myModel.rowCount(); color: "white" }
                }
                Button {
                    text: "Reset counters"
                    onClicked: root.resetCounters()
                }

                Header { text: "Slice identity" }
                Text {
                    color: root.snapshot.length === 0 ? "white"
                           : root.preserved === root.snapshot.length ? "#80ff80" : "#ff8080"
                    text: root.snapshot.length === 0
                          ? "no snapshot"
                          : root.preserved + " of " + root.snapshot.length + " slices still alive"
                }
                Flow {
                    Layout.fillWidth: true
                    spacing: 4
                    Button {
                        text: "Snapshot slices"
                        onClicked: root.takeSnapshot()
                    }
                    Button {
                        text: "Check now"
                        onClicked: root.updateIdentity()
                    }
                }
                Hint {
                    text: "Snapshot, then edit. Value/label edits and adding or removing rows must "
                          + "keep the other slices alive. A rebuild destroys them (0 alive). The "
                          + "removed slice itself is expected to be gone."
                }

                Header { text: "Model type" }
                CheckBox {
                    text: "Model emits reset on insert/remove"
                    palette.windowText: "white"
                    checked: myModel.resetOnStructuralChange
                    onToggled: myModel.resetOnStructuralChange = checked
                }
                Hint {
                    text: "On: like some custom/QML models. Every series edit below calls "
                          + "insertRows/removeRows on the model, which then resets. The mapper must "
                          + "not rebuild the series because of that."
                }

                Header { text: "Edit the series" }
                Hint {
                    text: "Expect: the model follows; only the single counters move; bulk counters "
                          + "stay put."
                }
                Flow {
                    Layout.fillWidth: true
                    spacing: 4
                    Button {
                        text: "Append slice"
                        onClicked: pieSeries.append("New " + root.nextSlice++, root.randomValue())
                    }
                    Button {
                        text: "Remove last"
                        enabled: pieSeries.count > 0
                        onClicked: pieSeries.remove(pieSeries.count - 1)
                    }
                    Button {
                        text: "First: new value"
                        enabled: pieSeries.count > 0
                        onClicked: pieSeries.at(0).value = root.randomValue()
                    }
                    Button {
                        text: "First: new label"
                        enabled: pieSeries.count > 0
                        onClicked: pieSeries.at(0).label = "Renamed " + root.nextSlice++
                    }
                }

                Header { text: "Edit the model" }
                Hint {
                    text: "Expect: the pie follows incrementally; bulk counters stay put and "
                          + "slice identity is kept."
                }
                Flow {
                    Layout.fillWidth: true
                    spacing: 4
                    Button {
                        text: "Add row"
                        onClicked: myModel.addRow("Added " + root.nextSlice++, root.randomValue())
                    }
                    Button {
                        text: "Remove last row"
                        onClicked: myModel.removeLastRow()
                    }
                    Button {
                        text: "Edit random value"
                        enabled: myModel.rowCount() > 0
                        onClicked: myModel.editCell(Math.floor(Math.random() * myModel.rowCount()),
                                                    1, root.randomValue())
                    }
                    Button {
                        text: "Edit random label"
                        enabled: myModel.rowCount() > 0
                        onClicked: myModel.editCell(Math.floor(Math.random() * myModel.rowCount()),
                                                    0, "Edited " + root.nextSlice++)
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
                            myModel.setCellSilently(0, 1, root.randomValue())
                            myModel.resetNow()
                        }
                    }
                }
                Hint {
                    text: "Unchanged reset: bulk counters stay put and slices stay alive (the "
                          + "series already matches). Silent edit + reset: the first slice's "
                          + "value changes and ideally nothing is rebuilt."
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
                    Text { text: "labelsSection"; color: "white" }
                    SpinBox {
                        from: -1
                        to: 7
                        value: mapper.labelsSection
                        onValueModified: mapper.labelsSection = value
                    }
                    Text { text: "valuesSection"; color: "white" }
                    SpinBox {
                        from: -1
                        to: 7
                        value: mapper.valuesSection
                        onValueModified: mapper.valuesSection = value
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
                            mapper.labelsSection = mapper.labelsSection
                            mapper.valuesSection = mapper.valuesSection
                            mapper.orientation = mapper.orientation
                            mapper.model = myModel
                            mapper.series = pieSeries
                        }
                    }
                }
                Hint {
                    text: "Changing a value updates the series, ideally in place. Re-applying "
                          + "unchanged values must change nothing. Horizontal maps columns to "
                          + "slices, so the 3-column model gives 3 slices, and labels come from "
                          + "the numeric cells of a row (the label column only has text in "
                          + "column 0)."
                }
            }
        }
    }
}
