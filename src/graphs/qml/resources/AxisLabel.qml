// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick
import QtQuick3D

Component {
    Model {
        id: root
        source: "#Cube"
        pickable: true

        property string labelText: "Bar"
        property color backgroundColor: "gray"
        property bool backgroundEnabled: false
        property color labelTextColor: "red"
        property bool borderEnabled : false
        property font labelFont
        property real labelWidth: -1
        property real labelHeight: -1

        materials: DefaultMaterial {
            diffuseMap: Texture {
                sourceItem: Item {
                    id: labelItem
                    width: labelWidth
                    height: labelHeight

                    Rectangle {
                        id: labelBackground
                        anchors.fill: parent
                        color: backgroundColor
                        visible: backgroundEnabled
                        border.color: labelTextColor
                        border.width: borderEnabled ? Math.max(0.5, (text0.font.pointSize / 16)) : 0
                        radius: 4
                    }

                    Text {
                        id: text0
                        anchors.centerIn: parent
                        color: labelTextColor
                        text: labelText
                        font: labelFont
                    }
                }
            }
        }
    }
}
