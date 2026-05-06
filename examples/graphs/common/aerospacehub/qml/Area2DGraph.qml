// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

pragma ComponentBehavior: Bound

import QtGraphs
import QtQuick
import QtQuick.Shapes
import AerospaceHubExample

GraphsView {
    id: graph

    property alias series: lineseries2d
    property bool customcolors: true
    property LineSimulator linesimulator
    property bool dataready: GlobalSettings.area2ddataready

    onDatareadyChanged: {
        if (dataready)
            fillArea()
    }

    //! [theme]
    theme: GlobalSettings.theme
    //! [theme]

    visible: opacity > 0.5

    onHoverEnter: {
        if (axisX.max !== axisX.visualMax || axisX.min !== axisX.visualMin)
            return
        tooltip.visible = true
    }

    onHoverExit: {
        tooltip.visible = false
    }

    onHover: (seriesName, position, value) => {
        if (axisX.max !== axisX.visualMax || axisX.min !== axisX.visualMin)
            return
        tooltip.x = position.x - tooltip.width / 2
        tooltip.y = position.y - tooltip.height
        tooltip.first = lineseries2d.at(value.x - 1).y.toFixed(1)
        tooltip.second = areaseries2d.at(value.x - 1).y.toFixed(1)
        tooltip.third = value.x.toFixed(0)
    }

    ToolTip2D {
        id: tooltip
        firstname: "Power (MW)"
        secondname: "Acc. (MWh)"
        thirdname: "Hour"
        secondicon: "../images/layers_24px.png"
        thirdicon: "../images/clock_24px.png"
    }

    axisX: ValueAxis {
        gridVisible: false
        subGridVisible: false
        min: 1
        max: 24
        tickInterval: 2
        titleText: "Hour"
        // Hide tooltip while panning or zooming
        onVisualMaxChanged: {
            if (visualMax !== max)
                tooltip.visible = false
        }
        onVisualMinChanged: {
            if (visualMin !== min)
                tooltip.visible = false
        }
    }

    axisY: ValueAxis {
        max: 100
        tickInterval: 20
        titleText: "Power output (MWh)"
    }

    SplineSeries {
        id: lineseries2d
        hoverable: true
        width: 3
        zValue: 1
        color: graph.customcolors ? "#3673FC" : "#00000000"
        // Add dummy points to remove a warning before data simulator starts feeding data
        XYPoint { x: 0; y: 0 }
        XYPoint { x: 0; y: 0 }
    }

    AreaSeries {
        id: area2d
        hoverable: true
        zValue: 0
        color: graph.customcolors ? "#173DDE" : "#00000000"

        borderColor: area2d.legendData[0] ? legendData[0].color : "transparent"

        gradient: LinearGradient {
            x1: 0; y1: 0
            x2: 0; y2: graph.plotArea.height
            GradientStop { position: 0.0; color: area2d.legendData[0] ? area2d.legendData[0].color : "transparent" }
            GradientStop { position: 1.0; color: area2d.legendData[0] ? area2d.legendData[0].color.darker(2) : "transparent" }
        }

        upperSeries: SplineSeries {
            id: areaseries2d
            // Add dummy points to remove a warning before data simulator starts feeding data
            XYPoint { x: 0; y: 0 }
            XYPoint { x: 0; y: 0 }
        }
    }

    Component.onCompleted: {
        if (linesimulator)
            linesimulator.addSeries(lineseries2d)
    }

    function fillArea() {
        let accumulation = 0
        // Use a temp series to be able to replace all points at once
        let tempseries = []
        for (let i = 0; i < lineseries2d.count; ++i) {
            accumulation += lineseries2d.at(i).y
            tempseries.push(Qt.point(i + 1, accumulation))
        }
        // Replace the points in one go
        areaseries2d.replace(tempseries)
        axisY.max = accumulation
        axisY.tickInterval = accumulation / 5
    }
}
