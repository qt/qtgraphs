// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtGraphs
import "."

Item {
    id: mainview
    width: 1280
    height: 1024

    property real itemCount: 1000.0
    property real addItems: 500.0

    Button {
        id: changeButton
        width: parent.width / 7
        height: 50
        anchors.left: parent.left
        enabled: true
        text: "Change"
        onClicked: {
            console.log("changeButton clicked");
            if (graphView.state === "meshsphere") {
                graphView.state = "meshcube"
            } else if (graphView.state === "meshcube") {
                graphView.state = "meshpyramid"
            } else if (graphView.state === "meshpyramid") {
                graphView.state = "meshpoint"
            } else if (graphView.state === "meshpoint") {
                graphView.state = "meshsphere"
            }
        }
    }

    Text {
        id: fpsText
        text: "Reading"
        width: (parent.width / 7) * 3
        height: 50
        anchors.left: changeButton.right
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }

    Button {
        id: optimization
        width: parent.width / 7
        height: 50
        anchors.left: fpsText.right
        enabled: true
        text: scatterPlot.optimizationHint === AbstractGraph3D.OptimizationHint.Legacy
              ? "To Default" : "To Legacy"
        onClicked: {
            console.log("Optimization " + optimization.text);
            if (scatterPlot.optimizationHint === AbstractGraph3D.OptimizationHint.Legacy) {
                scatterPlot.optimizationHint = AbstractGraph3D.OptimizationHint.Default;
                optimization.text = "To Legacy";
            } else {
                scatterPlot.optimizationHint = AbstractGraph3D.OptimizationHint.Legacy;
                optimization.text = "To Default";
            }
        }
    }

    Button {
        id: itemAdd
        width: parent.width / 7
        height: 50
        anchors.left: optimization.right
        enabled: true
        text: "Add"
        onClicked: {
            itemCount = itemCount + addItems;
            dataGenerator.add(scatterSeries, addItems);
        }
    }

    Button {
        id: writeLine
        width: parent.width / 7
        height: 50
        anchors.left: itemAdd.right
        enabled: true
        text: "Write"
        onClicked: {
            dataGenerator.writeLine(itemCount, scatterPlot.currentFps.toFixed(1));
        }
    }

    Item {
        id: graphView
        width: mainview.width
        height: mainview.height
        anchors.top: changeButton.bottom
        anchors.left: mainview.left
        state: "meshsphere"

        Timer {
            id: rotationTimer
            interval: 16
            running: true
            repeat: true
            onTriggered: {
                if (++scatterPlot.cameraXRotation == 360)
                    scatterPlot.cameraXRotation = 0;
            }
        }

        Scatter3D {
            id: scatterPlot
            width: graphView.width
            height: graphView.height
            shadowQuality: AbstractGraph3D.ShadowQuality.None
            optimizationHint: AbstractGraph3D.OptimizationHint.Legacy
            cameraYRotation: 45.0
            measureFps: true
            onCurrentFpsChanged: {
                fpsText.text = itemCount + " : " + scatterPlot.currentFps.toFixed(1);
            }

            theme: Theme3D {
                type: Theme3D.Theme.Retro
                colorStyle: Theme3D.ColorStyle.RangeGradient
                baseGradients: customGradient

                Gradient {
                    id: customGradient
                    GradientStop { position: 1.0; color: "red" }
                    GradientStop { position: 0.0; color: "blue" }
                }
            }

            Scatter3DSeries {
                id: scatterSeries
                mesh: Abstract3DSeries.Mesh.Sphere
            }

            Component.onCompleted: dataGenerator.generateData(scatterSeries, itemCount);
        }

        states: [
            State {
                name: "meshsphere"
                StateChangeScript {
                    name: "doSphere"
                    script: {
                        console.log("Do the sphere");
                        scatterSeries.mesh = Abstract3DSeries.Mesh.Sphere;
                    }
                }
            },
            State {
                name: "meshcube"
                StateChangeScript {
                    name: "doCube"
                    script: {
                        console.log("Do the cube");
                        scatterSeries.mesh = Abstract3DSeries.Mesh.Cube;
                    }
                }
            },
            State {
                name: "meshpyramid"
                StateChangeScript {
                    name: "doPyramid"
                    script: {
                        console.log("Do the pyramid");
                        scatterSeries.mesh = Abstract3DSeries.Mesh.Pyramid;
                    }
                }
            },
            State {
                name: "meshpoint"
                StateChangeScript {
                    name: "doPoint"
                    script: {
                        console.log("Do the point");
                        scatterSeries.mesh = Abstract3DSeries.Mesh.Point;
                    }
                }
            }
        ]
    }
}
