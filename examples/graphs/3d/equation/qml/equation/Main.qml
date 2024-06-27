// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtGraphs

pragma ComponentBehavior: Bound

Item {
    id: mainView
    width: 1280
    height: 1024

    property Gradient gradient : gradient1

    Gradient {
        id: gradient1
        GradientStop { id: gradient1Yellow; position: 1.0; color: "yellow" }
        GradientStop { id: gradient1Red; position: 0.6; color: "red" }
        GradientStop { id: gradient1Blue; position: 0.4; color: "blue" }
        GradientStop { id: gradient1Green; position: 0.0; color: "green" }
    }

    Gradient {
        id: gradient2
        GradientStop { id: gradient2White; position: 1.0; color: "white" }
        GradientStop { id: gradient2Purple; position: 0.6; color: "purple" }
        GradientStop { id: gradient2Blue; position: 0.4; color: "blue" }
        GradientStop {id: gradient2Grey; position: 0.0; color: "grey" }
    }

    Timer {
        id: dataTimer
        interval: 3000
        running: true
        repeat: true

        onTriggered: {
            if (gradient == gradient2)
                gradient = gradient1
            else
                gradient = gradient2
        }
    }

    ListModel {
        id: dataModel
    }

    function createDataPoints() {
        dataModel.clear();
        if (graphTypes.currentIndex) {
            for (var i = xValues.first.value; i <= xValues.second.value; i += 0.3) {
                for (var j = zValues.first.value; j <= zValues.second.value; j += 0.3)
                    addDataPoints(i, j);
            }
        } else {
            for (var i = xValues.first.value; i <= xValues.second.value; ++i) {
                for (var j = zValues.first.value; j <= zValues.second.value; ++j)
                    addDataPoints(i, j);
            }
        }
    }

    function addDataPoints(i, j) {
        var xPow = xPower.xPow;
        var yPow = yPower.yPow;
        var cons = constant.cons;
        var operator1 = eqOperator.operator;
        var operator2 = eqOperator2.operator;

        var powResult1 = Math.pow(i, xPow);
        var powResult2 = Math.pow(j, yPow);

        var result;
        switch (operator1) {
            case '+':
                result = powResult1 + powResult2;
                break;
            case '-':
                result = powResult1 - powResult2;
                break;
            case '*':
                result = powResult1 * powResult2;
                break;
            default:
                throw new Error("Invalid operator");
        }

        var y;
        switch (operator2) {
            case '+':
                y = result + cons;
                break;
            case '-':
                y = result - cons;
                break;
            case '*':
                y = result * cons;
                break;
            default:
                throw new Error("Invalid operator");
        }

        dataModel.append({"xPos": i, "yPos": y, "zPos": j});
    }

    Rectangle{
        id: features
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        width: parent.width / 4
        color: surfaceTheme.backgroundColor

        ColumnLayout {
            id : container
            anchors.top: parent.top
            width: parent.width
            spacing: 25

            ComboBox {
                id: graphTypes
                editable: true
                currentIndex: 0
                Layout.preferredHeight: 30
                Layout.margins: 5
                Layout.alignment: Qt.AlignCenter

                model: ListModel {
                    id: model
                    ListElement { text: "Surface Graph" }
                    ListElement { text: "Scatter Graph" }
                }

                onCurrentIndexChanged: graphs.currentIndex = currentIndex
            }

            Row {
                leftPadding: 5
                spacing: 15

                MessageDialog {
                    id : messageDialog
                    title: qsTr("Error")
                    text: qsTr("Undefined behavior. Try again.")
                    buttons: MessageDialog.Ok
                    onButtonClicked: function (button, role) {
                        if (role === MessageDialog.Ok) {
                            Qt.quit()
                        }
                    }
                }

                Text {
                    text: "Equation:"
                    color: surfaceTheme.labelTextColor
                    font.pointSize: 15
                    font.bold: true
                    font.italic: true
                }

                Rectangle {
                    width: 30
                    height: 20
                    color: "transparent"
                    Text {
                        id: x
                        text: "x"
                        color: surfaceTheme.labelTextColor
                        font.pointSize: 15
                        font.bold: true
                        font.italic: true
                    }

                    TextField {
                        id: xPower
                        width: 30
                        height: 20
                        anchors.left: x.right
                        anchors.bottom: x.top
                        property int xPow: 2
                        text: xPow
                        color: surfaceTheme.labelTextColor
                        font.pointSize: 12
                        font.bold: true
                        onEditingFinished: {
                            if (/\d/.test(text)) {
                                xPow = text
                                createDataPoints();
                            } else {
                                messageDialog.open()
                            }
                        }
                    }
                }

                Rectangle {
                    width: 20
                    height: 20
                    color: "transparent"
                    TextField {
                        id: eqOperator
                        property string operator: '-'
                        width: 20
                        height: 20
                        text: operator
                        color: surfaceTheme.labelTextColor
                        font.pointSize: 12
                        font.bold: true
                        onEditingFinished: {
                            operator = text
                            if (operator === '+' | operator === '-' | operator === '*')
                                createDataPoints();
                            else
                                messageDialog.open()
                        }
                    }
                }

                Rectangle {
                    width: 30
                    height: 20
                    color: "transparent"
                    Text {
                        id: y
                        text: "y"
                        horizontalAlignment: Text.AlignHCenter
                        color: surfaceTheme.labelTextColor
                        font.pointSize: 15
                        font.bold: true
                        font.italic: true
                    }

                    TextField {
                        id: yPower
                        width: 30
                        height: 20
                        anchors.left: y.right
                        anchors.bottom: y.top
                        property int yPow: 2
                        text: yPow
                        color: surfaceTheme.labelTextColor
                        font.pointSize: 12
                        font.bold: true
                        onEditingFinished: {
                            if (/\d/.test(text)) {
                                yPow = text
                                createDataPoints();
                            } else {
                                messageDialog.open()
                            }
                        }
                    }
                }

                Rectangle {
                    width: 20
                    height: 20
                    color: "transparent"
                    TextField {
                        id: eqOperator2
                        property string operator: '-'
                        width: 20
                        height: 20
                        text: operator
                        color: surfaceTheme.labelTextColor
                        font.pointSize: 12
                        font.bold: true
                        onEditingFinished: {
                            operator = text
                            if (operator === '+' | operator === '-' | operator === '*')
                                createDataPoints();
                            else
                                messageDialog.open()
                        }
                    }
                }

                Rectangle {
                    width: 35
                    height: 20
                    color: "transparent"
                    TextField {
                        id: constant
                        width: 35
                        height: 20
                        property int cons: 0
                        text: cons
                        color: surfaceTheme.labelTextColor
                        font.pointSize: 12
                        font.bold: true
                        onEditingFinished: {
                            if (/\d/.test(text)) {
                                cons = text
                                yValues.value = text
                                createDataPoints();
                            } else {
                                messageDialog.open()
                            }
                        }
                    }
                }
            }

            ColumnLayout {
                id : transparencySlider
                Layout.fillWidth: true
                Layout.preferredHeight: 30
                Layout.margins: 5
                Layout.alignment: Qt.AlignCenter
                visible: graphTypes.currentIndex

                Text {
                    text: "Transparency"
                    Layout.fillWidth: true
                    horizontalAlignment: Text.AlignHCenter
                    color: surfaceTheme.labelTextColor
                    font.pointSize: 15
                    font.bold: true
                }

                RowLayout {
                    id: transparencyRow
                    Layout.alignment: Qt.AlignCenter
                    Slider {
                        id: transparency
                        from: 0.0
                        to: 1.0
                        value: 1.0
                        stepSize: 0.1
                        snapMode: Slider.SnapAlways
                        Layout.alignment: Qt.AlignCenter

                        onValueChanged: {
                            gradient1Yellow.color.a = value
                            gradient1Red.color.a = value
                            gradient1Blue.color.a = value
                            gradient1Green.color.a = value

                            gradient2White.color.a = value
                            gradient2Purple.color.a = value
                            gradient2Blue.color.a = value
                            gradient2Grey.color.a = value
                        }
                    }

                    Label {
                        id: transparencyValue
                        text: transparency.value.toFixed(2)
                        color: surfaceTheme.labelTextColor
                        font.pointSize: 12
                        font.bold: true
                        Layout.minimumWidth: 60
                    }
                }
            }

            ColumnLayout {
                Text {
                    text: "Equation Parameters"
                    Layout.fillWidth: true
                    horizontalAlignment: Text.AlignHCenter
                    color: surfaceTheme.labelTextColor
                    font.pointSize: 15
                    font.bold: true
                }

                RowLayout {
                    Text {
                        text: "x"
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignHCenter
                        color: surfaceTheme.labelTextColor
                        font.pointSize: 15
                        font.bold: true
                        font.italic: true
                    }

                    Label {
                        text: xValues.first.value.toFixed(1)
                        color: surfaceTheme.labelTextColor
                        font.pointSize: 12
                        font.bold: true
                        Layout.minimumWidth: 60
                    }

                    RangeSlider{
                        id: xValues
                        from: -10
                        to: 10
                        stepSize: (graphs.currentIndex === 2) ? 0.1 : 1
                        first.value: -2
                        second.value: 2

                        first.onValueChanged: createDataPoints();
                        second.onValueChanged: createDataPoints();
                    }

                    Label {
                        text: xValues.second.value.toFixed(1)
                        color: surfaceTheme.labelTextColor
                        font.pointSize: 12
                        font.bold: true
                        Layout.minimumWidth: 60
                    }
                }

                RowLayout {
                    Text {
                        text: "y"
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignHCenter
                        color: surfaceTheme.labelTextColor
                        font.pointSize: 15
                        font.bold: true
                        font.italic: true
                    }

                    Slider {
                        id: yValues
                        from: -10
                        to: 10
                        stepSize: 1.0
                        value: 0
                        snapMode: Slider.SnapAlways
                        Layout.alignment: Qt.AlignCenter
                        onValueChanged: {
                            constant.cons = value
                            createDataPoints();
                        }
                    }

                    Label {
                        text: yValues.value.toFixed(1)
                        color: surfaceTheme.labelTextColor
                        font.pointSize: 12
                        font.bold: true
                        Layout.minimumWidth: 60
                    }
                }

                RowLayout {
                    Text {
                        text: "z"
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignHCenter
                        color: surfaceTheme.labelTextColor
                        font.pointSize: 15
                        font.bold: true
                        font.italic: true
                    }

                    Label {
                        text: zValues.first.value.toFixed(1)
                        color: surfaceTheme.labelTextColor
                        font.pointSize: 12
                        font.bold: true
                        Layout.minimumWidth: 60
                    }

                    RangeSlider{
                        id: zValues
                        from: -10.0
                        to: 10.0
                        stepSize: 1.0
                        first.value: -2.0
                        second.value: 2.0

                        first.onValueChanged: createDataPoints();
                        second.onValueChanged: createDataPoints();
                    }

                    Label {
                        text: zValues.second.value.toFixed(1)
                        color: surfaceTheme.labelTextColor
                        font.pointSize: 12
                        font.bold: true
                        Layout.minimumWidth: 60
                    }
                }
            }
        }
    }

    StackLayout {
        id: graphs
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: features.left
        currentIndex: graphTypes.currentIndex
        onCurrentIndexChanged: createDataPoints()

        Item {
            id: surfaceView
            Surface3D {
                id: surfaceGraph
                anchors.fill: parent
                shadowQuality: Graphs3D.ShadowQuality.None
                aspectRatio: 2.0
                horizontalAspectRatio: 1.0
                msaaSamples: 8
                cameraPreset: Graphs3D.CameraPreset.IsometricRight
                cameraZoomLevel: 60
                axisX.segmentCount: 10
                axisZ.segmentCount: 10
                axisY.segmentCount: 10

                theme : GraphsTheme {
                    id: surfaceTheme
                    colorStyle: GraphsTheme.ColorStyle.RangeGradient
                    baseGradients: gradient
                    plotAreaBackgroundVisible: false
                }

                Surface3DSeries {
                    id: surfaceSeries
                    itemLabelFormat: "(@xLabel, @zLabel): @yLabel"

                    ItemModelSurfaceDataProxy {
                        itemModel: dataModel
                        columnRole: "xPos"
                        yPosRole: "yPos"
                        rowRole: "zPos"
                    }
                }
            }
        }

        Item {
            id: scatterView
            Scatter3D {
                id: scatterGraph
                anchors.fill: parent
                shadowQuality: Graphs3D.ShadowQuality.None
                aspectRatio: 2.0
                horizontalAspectRatio: 1.0
                msaaSamples: 8
                cameraPreset: Graphs3D.CameraPreset.IsometricRight
                cameraZoomLevel: 60
                axisX.segmentCount: 10
                axisZ.segmentCount: 10
                axisY.segmentCount: 10

                theme : GraphsTheme {
                    id: scatterTheme
                    colorStyle: GraphsTheme.ColorStyle.RangeGradient
                    baseGradients: gradient
                    plotAreaBackgroundVisible: false
                }
                Scatter3DSeries {
                    id: scatterSeries
                    itemSize: 0.1
                    itemLabelFormat: "(@xLabel, @zLabel): @yLabel"
                    mesh: Abstract3DSeries.Mesh.Cube

                    ItemModelScatterDataProxy {
                        itemModel: dataModel

                        xPosRole: "xPos"
                        yPosRole: "yPos"
                        zPosRole: "zPos"
                    }
                }
            }
        }
    }

    NumberAnimation {
        id: cameraAnimationX
        loops: Animation.Infinite
        running: true
        target: scatterGraph
        property:"cameraXRotation"
        from: 0.0
        to: 360.0
        duration: 20000
    }

    SequentialAnimation {
        id: cameraAnimationY
        loops: Animation.Infinite
        running: true

        NumberAnimation {
            target: scatterGraph
            property:"cameraYRotation"
            from: 5.0
            to: 45.0
            duration: 9000
            easing.type: Easing.InOutSine
        }

        NumberAnimation {
            target: scatterGraph
            property:"cameraYRotation"
            from: 45.0
            to: 5.0
            duration: 9000
            easing.type: Easing.InOutSine
        }
    }
}
