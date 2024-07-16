// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick3D.Helpers
import QtQuick3D
import QtGraphs

pragma ComponentBehavior: Bound

Item {
    id: mainView
    width: 1280
    height: 1024

    property var activeTheme: surfaceTheme

    Gradient {
        id: gradient1
        GradientStop { id: gradient1Yellow; position: 1.0; color: "yellow" }
        GradientStop { id: gradient1Red; position: 0.66; color: "red" }
        GradientStop { id: gradient1Blue; position: 0.33; color: "blue" }
        GradientStop { id: gradient1Green; position: 0.0; color: "green" }
    }

    Gradient {
        id: gradient2
        GradientStop { position: 1.0; color: "#DBEB00" } // Qt Brand color; Lemon
        GradientStop { position: 0.0; color: "#373F26" } // Qt Brand color; Moss
    }

    ListModel {
        id: dataModel
    }

    function createDataPoints() {
        dataModel.clear()
        if (graphTypes.currentIndex === 2) {
            for (var i = xValues.first.value; i <= xValues.second.value; i += 0.3) {
                for (var j = zValues.first.value; j <= zValues.second.value; j += 0.3)
                    addDataPoints(i, j)
            }
        } else {
            for (i = xValues.first.value; i <= xValues.second.value; ++i) {
                for (j = zValues.first.value; j <= zValues.second.value; ++j)
                    addDataPoints(i, j)
            }
        }
    }

    function addDataPoints(i, j) {
        var xPow = xPower.xPow
        var yPow = yPower.yPow
        var cons = constant.cons
        var operator1 = eqOperator.operator
        var operator2 = eqOperator2.operator

        var powResult1 = Math.pow(i, xPow)
        var powResult2 = Math.pow(j, yPow)

        var result
        switch (operator1) {
        case '+':
            result = powResult1 + powResult2
            break
        case '-':
            result = powResult1 - powResult2
            break
        case '*':
            result = powResult1 * powResult2
            break
        default:
            throw new Error("Invalid operator")
        }

        var y
        switch (operator2) {
        case '+':
            y = result + cons
            break
        case '-':
            y = result - cons
            break
        case '*':
            y = result * cons
            break
        default:
            throw new Error("Invalid operator")
        }

        dataModel.append({"xPos": i, "yPos": y, "zPos": j})
    }

    Rectangle{
        id: features
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        width: parent.width / 4
        color: mainView.activeTheme.backgroundColor

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
                    ListElement { text: "Surface" }
                    ListElement { text: "Bars" }
                    ListElement { text: "Scatter" }
                }

                onCurrentIndexChanged: {
                    graphs.currentIndex = currentIndex
                    mainView.activeTheme = (currentIndex === 0)
                            ? surfaceTheme : ((currentIndex === 1)
                                              ? barTheme : scatterTheme)
                }
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
                        if (role === MessageDialog.Ok)
                            Qt.quit()
                    }
                }

                Text {
                    text: "Equation:"
                    color: mainView.activeTheme.labelTextColor
                    font.bold: true
                    font.italic: true
                }

                Item {
                    width: 30
                    height: 20

                    Text {
                        id: x
                        text: "x"
                        color: mainView.activeTheme.labelTextColor
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
                        color: mainView.activeTheme.labelTextColor
                        font.pointSize: 12
                        font.bold: true
                        onEditingFinished: {
                            if (/\d/.test(text)) {
                                xPow = text
                                mainView.createDataPoints()
                            } else {
                                messageDialog.open()
                            }
                        }
                    }
                }

                Item {
                    width: 20
                    height: 20

                    TextField {
                        id: eqOperator
                        property string operator: '-'
                        width: 20
                        height: 20
                        text: operator
                        color: mainView.activeTheme.labelTextColor
                        font.pointSize: 12
                        font.bold: true
                        onEditingFinished: {
                            operator = text
                            if (operator === '+' | operator === '-' | operator === '*')
                                mainView.createDataPoints()
                            else
                                messageDialog.open()
                        }
                    }
                }

                Item {
                    width: 30
                    height: 20

                    Text {
                        id: y
                        text: "y"
                        horizontalAlignment: Text.AlignHCenter
                        color: mainView.activeTheme.labelTextColor
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
                        color: mainView.activeTheme.labelTextColor
                        font.pointSize: 12
                        font.bold: true
                        onEditingFinished: {
                            if (/\d/.test(text)) {
                                yPow = text
                                mainView.createDataPoints()
                            } else {
                                messageDialog.open()
                            }
                        }
                    }
                }

                Item {
                    width: 20
                    height: 20
                    visible: graphs.currentIndex === 1

                    TextField {
                        id: eqOperator2
                        property string operator: '-'
                        width: 20
                        height: 20
                        text: operator
                        color: mainView.activeTheme.labelTextColor
                        font.pointSize: 12
                        font.bold: true
                        onEditingFinished: {
                            operator = text
                            if (operator === '+' | operator === '-' | operator === '*')
                                mainView.createDataPoints()
                            else
                                messageDialog.open()
                        }
                    }
                }

                Item {
                    width: 35
                    height: 20
                    visible: graphs.currentIndex === 1

                    TextField {
                        id: constant
                        width: 35
                        height: 20
                        property int cons: 0
                        text: cons
                        color: mainView.activeTheme.labelTextColor
                        font.pointSize: 12
                        font.bold: true
                        onEditingFinished: {
                            if (/\d/.test(text)) {
                                cons = text
                                mainView.createDataPoints()
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
                visible: graphTypes.currentIndex === 1

                Text {
                    text: "Transparency"
                    Layout.fillWidth: true
                    horizontalAlignment: Text.AlignHCenter
                    color: mainView.activeTheme.labelTextColor
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
                        value: 0.8
                        stepSize: 0.1
                        snapMode: Slider.SnapAlways
                        Layout.alignment: Qt.AlignCenter

                        onValueChanged: adjustOpacity()

                        Component.onCompleted: adjustOpacity()

                        function adjustOpacity() {
                            gradient1Yellow.color.a = value / 2.5
                            gradient1Red.color.a = value / 2.0
                            gradient1Blue.color.a = value / 1.5
                            gradient1Green.color.a = value
                        }
                    }

                    Label {
                        id: transparencyValue
                        text: transparency.value.toFixed(2)
                        color: mainView.activeTheme.labelTextColor
                        font.pointSize: 12
                        font.bold: true
                        Layout.minimumWidth: 60
                    }
                }
            }

            ColumnLayout {
                Text {
                    text: "Dimensions"
                    Layout.fillWidth: true
                    horizontalAlignment: Text.AlignHCenter
                    color: mainView.activeTheme.labelTextColor
                    font.pointSize: 15
                    font.bold: true
                }

                RowLayout {
                    Text {
                        text: "Columns"
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignHCenter
                        color: mainView.activeTheme.labelTextColor
                        font.bold: true
                        font.italic: true
                        Layout.minimumWidth: 60
                    }

                    Label {
                        text: xValues.first.value.toFixed(1)
                        color: mainView.activeTheme.labelTextColor
                        font.pointSize: 12
                        font.bold: true
                        Layout.minimumWidth: 40
                    }

                    RangeSlider{
                        id: xValues
                        from: -20
                        to: 20
                        stepSize: (graphs.currentIndex === 2) ? 0.1 : 1
                        first.value: -10
                        second.value: 10

                        first.onValueChanged: mainView.createDataPoints()
                        second.onValueChanged: mainView.createDataPoints()
                    }

                    Label {
                        text: xValues.second.value.toFixed(1)
                        color: mainView.activeTheme.labelTextColor
                        font.pointSize: 12
                        font.bold: true
                        Layout.minimumWidth: 40
                    }
                }

                RowLayout {
                    Text {
                        text: "Rows"
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignHCenter
                        color: mainView.activeTheme.labelTextColor
                        font.bold: true
                        font.italic: true
                        Layout.minimumWidth: 60
                    }

                    Label {
                        text: zValues.first.value.toFixed(1)
                        color: mainView.activeTheme.labelTextColor
                        font.pointSize: 12
                        font.bold: true
                        Layout.minimumWidth: 40
                    }

                    RangeSlider{
                        id: zValues
                        from: -20.0
                        to: 20.0
                        stepSize: 1.0
                        first.value: -10.0
                        second.value: 10.0

                        first.onValueChanged: mainView.createDataPoints()
                        second.onValueChanged: mainView.createDataPoints()
                    }

                    Label {
                        text: zValues.second.value.toFixed(1)
                        color: mainView.activeTheme.labelTextColor
                        font.pointSize: 12
                        font.bold: true
                        Layout.minimumWidth: 40
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
        onCurrentIndexChanged: mainView.createDataPoints()

        Item {
            id: surfaceView

            Surface3D {
                id: surfaceGraph
                anchors.fill: parent
                shadowQuality: Graphs3D.ShadowQuality.None
                selectionMode: Graphs3D.SelectionFlag.Item
                aspectRatio: 2.0
                horizontalAspectRatio: 1.0
                msaaSamples: 8
                cameraPreset: Graphs3D.CameraPreset.IsometricRight
                cameraZoomLevel: 80
                axisX.segmentCount: 10
                axisZ.segmentCount: 10
                axisY.segmentCount: 10

                theme : GraphsTheme {
                    id: surfaceTheme
                    colorStyle: GraphsTheme.ColorStyle.RangeGradient
                    baseGradients: [ gradient1 ]
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
            id: barView

            Bars3D {
                id: barGraph
                anchors.fill: parent
                shadowQuality: Graphs3D.ShadowQuality.None
                selectionMode: Graphs3D.SelectionFlag.Item
                msaaSamples: 8
                cameraPreset: Graphs3D.CameraPreset.IsometricRight
                cameraZoomLevel: 80
                barSpacing: Qt.size(0.1, 0.1)

                theme : GraphsTheme {
                    id: barTheme
                    colorStyle: GraphsTheme.ColorStyle.RangeGradient
                    baseGradients: [ gradient1 ]
                    plotAreaBackgroundVisible: false
                }

                Bar3DSeries {
                    id: barSeries
                    itemLabelFormat: "@colLabel, @rowLabel: @valueLabel"

                    ItemModelBarDataProxy {
                        itemModel: dataModel

                        columnRole: "xPos"
                        valueRole: "yPos"
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
                selectionMode: Graphs3D.SelectionFlag.Item
                aspectRatio: 2.0
                horizontalAspectRatio: 1.0
                msaaSamples: 8
                cameraPreset: Graphs3D.CameraPreset.IsometricRight
                cameraZoomLevel: 80
                axisX.segmentCount: 10
                axisZ.segmentCount: 10
                axisY.segmentCount: 10

                environment: ExtendedSceneEnvironment {
                    backgroundMode: ExtendedSceneEnvironment.Color
                    clearColor: scatterTheme.backgroundColor
                    tonemapMode: ExtendedSceneEnvironment.TonemapModeNone
                    glowEnabled: true
                    glowStrength: 1.2
                    glowIntensity: 8
                    glowBloom: 1
                    glowUseBicubicUpscale: true
                    glowLevel: ExtendedSceneEnvironment.GlowLevel.One
                               | ExtendedSceneEnvironment.GlowLevel.Two
                               | ExtendedSceneEnvironment.GlowLevel.Three
                               | ExtendedSceneEnvironment.GlowLevel.Four
                               | ExtendedSceneEnvironment.GlowLevel.Five
                }


                importScene: Node {
                    Model {
                        scale: Qt.vector3d(0.1, 0.1, 0.1)
                        source: "#Rectangle"
                        x: 0
                        z: 0
                        y: -2
                        eulerRotation.x: -90
                        castsReflections: false
                        receivesReflections: false
                        materials: [
                            PrincipledMaterial {
                                baseColor: "#373F26"
                                opacityMap: Texture {
                                    source: "qrc:/images/opacitymap.png"
                                }
                            }
                        ]
                    }
                }

                theme : GraphsTheme {
                    id: scatterTheme
                    colorStyle: GraphsTheme.ColorStyle.RangeGradient
                    colorScheme: Qt.Dark
                    baseGradients: [ gradient2 ]
                    plotAreaBackgroundVisible: false
                    labelsVisible: false
                    gridVisible: false
                }

                Scatter3DSeries {
                    id: scatterSeries
                    itemLabelFormat: "(@xLabel, @zLabel): @yLabel"
                    mesh: Abstract3DSeries.Mesh.Point

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
