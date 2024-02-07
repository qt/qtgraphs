// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtGraphs

Item {
    anchors.fill: parent

    Text {
        id: title
        text: "Simple Pie Graph"
        anchors.horizontalCenter: parent.horizontalCenter
        color: "#f0f0f0"
        y: parent.height * .1
    }

    GraphsView {
        id: chartView
        width: parent.width
        height: parent.height
        anchors.top: title.bottom
        anchors.bottom: parent.bottom

        property variant otherSlice: 0

        theme: GraphTheme {
            id: myTheme
            colorTheme: GraphTheme.ColorThemeDark
        }

        PieSeries {
            id: pieSeries
            PieSlice { label: "Volkswagen"; value: 13.5 }
            PieSlice { label: "Toyota"; value: 10.9 }
            PieSlice { label: "Ford"; value: 8.6 }
            PieSlice { label: "Skoda"; value: 8.2 }
            PieSlice { label: "Volvo"; value: 6.8 }
        }

        Component.onCompleted: {
            otherSlice = pieSeries.append("Others", 52.0);
            pieSeries.find("Volkswagen").exploded = true;
        }
    }
}
