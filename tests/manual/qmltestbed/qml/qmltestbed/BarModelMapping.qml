// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
import QtQuick
import QtGraphs
import QtQuick.Controls.Basic
import QtQuick.Layouts
import TestbedExample

Rectangle {
    id: background
    anchors.fill: parent
    color: "#404040"

    property real minCount: -1.0
    property real minFirst: 0.0
    property real minFirstSetSection: 0.0
    property real minLastSetSection: 0.0

    property real startCount: -1.0
    property real startFirst: 4.0
    property real startFirstSetSection: 0.0
    property real startLastSetSection: 1.0

    property real maxCount: 6.0
    property real maxFirst: 6.0
    property real maxFirstSetSection: 5.0
    property real maxLastSetSection: 5.0

    function addMapping() {
        myModel.clearMapping();
        if (vModelMapper.orientation === Qt.Vertical) {
            for (var i = 0; i < barSeries.legendData.length; i++) {
                var cf = vModelMapper.firstBarSetSection + i;
                var cl = 1;
                var rf = vModelMapper.first;
                var rl = barSeries.barSets[i].count;
                var c = barSeries.legendData[i].color;
                myModel.addMapping(c, cf, rf, cl, rl);
            }
        } else {
            for (var i = 0; i < barSeries.legendData.length; i++) {
                var rf = vModelMapper.firstBarSetSection + i;
                var rl = 1;
                var cf = vModelMapper.first;
                var cl = barSeries.barSets[i].count;
                var c = barSeries.legendData[i].color;
                myModel.addMapping(c, cf, rf, cl, rl);
            }
        }
    }
    function updateCategoryAxis() {
        if (vModelMapper.orientation === Qt.Vertical) {
            var categories = copyArray(vHeaderView.rowNames);
            var categorySubset = categories.splice(vModelMapper.first, vModelMapper.count);
            categoryAxis.categories = categorySubset;
        } else {
            var categories = [];
            var end = vModelMapper.first + vModelMapper.count;
            if (end >= myModel.columnCount())
                end = myModel.columnCount();

            for (var i = vModelMapper.first; i < end; ++i)
                categories.push(myModel.headerData(i, Qt.Horizontal));

            categoryAxis.categories = categories;
        }
    }

    function handleOrientationChange() {
        if (vModelMapper.orientation === Qt.Vertical) {
            firstSelectionLabel.text = "first row";
            countSelectionLabel.text = "row count";
            firstSetSectionSelectionLabel.text = "first column";
            lastSetSectionSelectionLabel.text = "last column";
            countSelectionSlider.to = 12;
            firstSelectionSlider.to = 12;
            firstSetSectionSelectionSlider.to = 5;
            lastSetSectionSelectionSlider.to = 5;
        } else {
            firstSelectionLabel.text = "first column";
            countSelectionLabel.text = "column count";
            firstSetSectionSelectionLabel.text = "first row";
            lastSetSectionSelectionLabel.text = "last row";
            countSelectionSlider.to = 6;
            firstSelectionSlider.to = 5;
            firstSetSectionSelectionSlider.to = 12;
            lastSetSectionSelectionSlider.to = 12;
        }
        myModel.startAddMapping();
        addMapping();
        myModel.endAddMapping();
        updateCategoryAxis();
    }

    function copyArray(arr) {
        var copy = [];
        for (var i = 0; i < arr.length; ++i)
            copy.push(arr[i]);
        return copy;
    }

    Item {
        id: tableViewItem

        anchors.top: background.top
        anchors.left: background.left
        anchors.bottom: background.bottom
        anchors.topMargin: 80 * px
        HorizontalHeaderView {
            id: hHeaderView

            anchors.top: tableViewItem.top
            anchors.left: tableView.left
            syncView: tableView
            Layout.fillWidth: true
            delegate: Text {
                padding: 3
                text: display
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
                color: "#FFFFFF"
                font.pointSize: 10
            }
        }
        VerticalHeaderView {
            id: vHeaderView

            readonly property var rowNames: ["January", "February", "March", "April", "May", "June",
                "July", "August", "September", "October", "November", "December"]
            anchors.top: tableView.top
            anchors.left: tableViewItem.left
            syncView: tableView
            Layout.fillHeight: true
            delegate: Text {
                required property int index
                padding: 3
                text: vHeaderView.rowNames[index]
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
                color: "#FFFFFF"
                font.pointSize: 16
            }
        }
        TableView {
            id: tableView
            anchors.top: hHeaderView.bottom
            anchors.left: vHeaderView.right

            implicitWidth: 300
            implicitHeight: 300
            reuseItems: false
            clip: true
            model: BarModelMapperModel {
                id: myModel
            }
            delegate: Rectangle {
                id: delegateRoot
                required property string display
                required property color background
                implicitHeight: 30
                implicitWidth: tableView.width / 6

                border.width: 1
                color: background
                Text {
                    id: delegateText
                    text: delegateRoot.display
                    anchors.verticalCenter: parent.verticalCenter
                    width: parent.width
                    anchors.leftMargin: 4
                    anchors.left: parent.left
                    anchors.right: parent.right
                }
            }
        }
        Column {
            anchors.top: tableView.bottom
            anchors.left: vHeaderView.left
            anchors.topMargin: 10
            anchors.leftMargin: 10
            Row {
                anchors.horizontalCenter: firstSelectionSlider.horizontalCenter
                Text {
                    id: firstSelectionLabel
                    text: qsTr("first row")
                    color: "#FFFFFF"
                    font.pointSize: 16
                }
                Text {
                    id: firstSelectionValueLabel
                    leftPadding: 50
                    text: firstSelectionSlider.value
                    color: "#FFFFFF"
                    font.pointSize: 16
                }
            }

            Slider {
                id: firstSelectionSlider
                from: background.minFirst
                to: background.maxFirst
                stepSize: 2.0
                value: background.startFirst
                handle: Rectangle {
                    // Position the handle based on the slider's value
                    x: firstSelectionSlider.leftPadding
                       + firstSelectionSlider.visualPosition
                       * firstSelectionSlider.availableWidth - width / 2
                    y: firstSelectionSlider.topPadding
                       + firstSelectionSlider.availableHeight / 2 - height / 2

                    width: 24 // Custom width for the handle
                    height: 24 // Custom height for the handle
                    radius: 12 // Make it a circle

                    color: "white"

                    // --- THE CORE LOGIC FOR HIGHLIGHTING ---
                    // Bind the border properties to the slider's focus state
                    border.color: firstSelectionSlider.focus ? "#41a8e0" : "#7f8c8d"
                    border.width: firstSelectionSlider.focus ? 3 : 1
                }
                onMoved: () => {
                    myModel.startAddMapping();
                    vModelMapper.first = firstSelectionSlider.value;
                    myModel.endAddMapping();
                    updateCategoryAxis();
                }
            }
            Row {
                anchors.horizontalCenter: countSelectionSlider.horizontalCenter
                Text {
                    id: countSelectionLabel
                    text: qsTr("row count")
                    color: "#FFFFFF"
                    font.pointSize: 16
                }
                Text {
                    id: countSelectionValueLabel
                    text: countSelectionSlider.value
                    leftPadding: 50
                    color: "#FFFFFF"
                    font.pointSize: 16
                }
            }
            Slider {
                id: countSelectionSlider
                from: background.minCount
                to: background.maxCount
                value: background.startCount
                stepSize: 1.0

                handle: Rectangle {
                    // Position the handle based on the slider's value
                    x: countSelectionSlider.leftPadding + countSelectionSlider.visualPosition
                       * countSelectionSlider.availableWidth - width / 2
                    y: countSelectionSlider.topPadding + countSelectionSlider.availableHeight / 2 - height / 2

                    width: 24
                    height: 24
                    radius: 12

                    color: "white"

                    // --- THE CORE LOGIC FOR HIGHLIGHTING ---
                    // Bind the border properties to the slider's focus state
                    border.color: countSelectionSlider.focus ? "#41a8e0" : "#7f8c8d"
                    border.width: countSelectionSlider.focus ? 3 : 1
                }

                onMoved: () => {
                    myModel.startAddMapping();
                    vModelMapper.count = countSelectionSlider.value;
                    myModel.endAddMapping();
                    updateCategoryAxis();
                }
            }
            Row {
                anchors.horizontalCenter: firstSetSectionSelectionSlider.horizontalCenter
                Text {
                    id: firstSetSectionSelectionLabel
                    text: qsTr("first column")
                    color: "#FFFFFF"
                    font.pointSize: 16
                }
                Text {
                    id: firstSetSectionSelectionValueLabel
                    text: firstSetSectionSelectionSlider.value
                    leftPadding: 50
                    color: "#FFFFFF"
                    font.pointSize: 16
                }
            }
            Slider {
                id: firstSetSectionSelectionSlider
                from: background.minFirstSetSection
                to: background.maxFirstSetSection
                value: background.startFirstSetSection

                handle: Rectangle {
                    // Position the handle based on the slider's value
                    x: firstSetSectionSelectionSlider.leftPadding
                       + firstSetSectionSelectionSlider.visualPosition
                       * firstSetSectionSelectionSlider.availableWidth - width / 2
                    y: firstSetSectionSelectionSlider.topPadding
                       + firstSetSectionSelectionSlider.availableHeight / 2 - height / 2

                    width: 24
                    height: 24
                    radius: 12

                    color: "white"

                    // --- THE CORE LOGIC FOR HIGHLIGHTING ---
                    // Bind the border properties to the slider's focus state
                    border.color: firstSetSectionSelectionSlider.focus ? "#41a8e0" : "#7f8c8d"
                    border.width: firstSetSectionSelectionSlider.focus ? 3 : 1
                }
                stepSize: 1.0
                onMoved: () => {
                    myModel.startAddMapping()
                    vModelMapper.firstBarSetSection = firstSetSectionSelectionSlider.value;
                    myModel.endAddMapping()
                    updateCategoryAxis();
                }
            }
            Row {
                anchors.horizontalCenter: lastSetSectionSelectionSlider.horizontalCenter
                Text {
                    id: lastSetSectionSelectionLabel
                    text: qsTr("last column")
                    color: "#FFFFFF"
                    font.pointSize: 16
                }
                Text {
                    id: lastSetSectionSelectionValueLabel
                    text: lastSetSectionSelectionSlider.value
                    leftPadding: 50
                    color: "#FFFFFF"
                    font.pointSize: 16
                }
            }
            Slider {
                id: lastSetSectionSelectionSlider
                from: background.minLastSetSection
                to: background.maxLastSetSection
                stepSize: 1.0
                value: background.startLastSetSection
                handle: Rectangle {
                    // Position the handle based on the slider's value
                    x: lastSetSectionSelectionSlider.leftPadding
                       + lastSetSectionSelectionSlider.visualPosition
                       * lastSetSectionSelectionSlider.availableWidth - width / 2
                    y: lastSetSectionSelectionSlider.topPadding
                       + lastSetSectionSelectionSlider.availableHeight / 2 - height / 2

                    width: 24
                    height: 24
                    radius: 12

                    color: "white"

                    // --- THE CORE LOGIC FOR HIGHLIGHTING ---
                    // Bind the border properties to the slider's focus state
                    border.color: lastSetSectionSelectionSlider.focus ? "#41a8e0" : "#7f8c8d"
                    border.width: lastSetSectionSelectionSlider.focus ? 3 : 1
                }
                onMoved: () => {
                    myModel.startAddMapping();
                    vModelMapper.lastBarSetSection = lastSetSectionSelectionSlider.value;
                    myModel.endAddMapping();
                    updateCategoryAxis();
                }
            }
        }
    }
    Row {
        id: legendRow
        anchors.top: background.top
        anchors.topMargin: 30
        anchors.horizontalCenter: background.horizontalCenter
        spacing: 2
        Repeater {
            model: barSeries.legendData.length
            Rectangle {
                id: legend1
                height: 20
                width: 40
                color: barSeries.legendData[index].color
                Text {
                    id: text1
                    text: barSeries.legendData[index].label
                }
            }
        }
    }

    Item {
        id: mainView
        anchors.left: tableViewItem.right
        anchors.right: background.right
        anchors.top: background.top
        anchors.bottom: background.bottom

        GraphsView {
            id: chartView
            anchors.fill: mainView
            anchors.margins: 20 * px
            anchors.topMargin: 80 * px
            anchors.leftMargin: 350 * px

            axisX: BarCategoryAxis {
                id: categoryAxis
                subGridVisible: false
            }
            axisY: ValueAxis {
                id: axisY
                max: 10
                subTickCount: 9
            }

            BarSeries {
                id: barSeries
                selectable: true
                onLegendDataChanged: addMapping()
            }

            BarModelMapper {
                id: vModelMapper
                series: barSeries
                model: myModel
                firstBarSetSection: background.startFirstSetSection
                lastBarSetSection: background.startLastSetSection
                first: background.startFirst
                count: background.startCount
                orientation: Qt.Vertical
                onOrientationChanged: handleOrientationChange()
            }
        }
    }
    SettingsView {
        Item {
            width: 260
            height: 10
        }

        Button {
            width: 250
            text: "Vertical/Horizontal bars"
            onClicked: {
                if (chartView.orientation === Qt.Vertical)
                    chartView.orientation = Qt.Horizontal;
                else
                    chartView.orientation = Qt.Vertical;
                myModel.startAddMapping();
                myModel.endAddMapping();
            }
        }

        Button {
            width: 250
            text: "Vertical/Horizontal mapper"
            onClicked: {
                vModelMapper.orientation = vModelMapper.orientation === Qt.Vertical
                        ? Qt.Horizontal
                        : Qt.Vertical;
            }
        }
    }
    Component.onCompleted: updateCategoryAxis()
}
