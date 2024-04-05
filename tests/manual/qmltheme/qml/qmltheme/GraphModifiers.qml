// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion
import QtGraphs

ColumnLayout {
    spacing: 10

    property alias barsVisible: barsVisible
    Label {
        text: "Bars3D Graph"
        color: "gray"
    }
    CheckBox {
        id: barsVisible
        onCheckedChanged: {
            customTheme = (checked ? customBarsTheme : customSurfaceTheme)
        }
    }

    Label {
        text: "Ambient Light Strength"
        color: "gray"
    }
    Slider {
        from: 0.0
        to: 1.0
        value: customTheme.ambientLightStrength
        onValueChanged: customTheme.ambientLightStrength = value
    }

    Label {
        text: "Light Strength"
        color: "gray"
    }
    Slider {
        from: 0.0
        to: 10.0
        value: customTheme.lightStrength
        onValueChanged: customTheme.lightStrength = value
    }

    Label {
        text: testgradientchange.checked ? "Gradient Color, Red" : "Light Color; Red"
        color: "gray"
    }
    Slider {
        from: 0.0
        to: 1.0
        value: testgradientchange.checked ? 1.0 : customTheme.lightColor.r
        onValueChanged: testgradientchange.checked ? (redstop.color.r = value)
                                                   : (customTheme.lightColor.r = value)
    }

    Label {
        text: testgradientchange.checked ? "Gradient Color, Green" : "Light Color; Green"
        color: "gray"
    }
    Slider {
        from: 0.0
        to: 1.0
        value: testgradientchange.checked ? 0.5 : customTheme.lightColor.g
        onValueChanged: testgradientchange.checked ? (greenstop.color.g = value)
                                                   : (customTheme.lightColor.g = value)
    }

    Label {
        text: testgradientchange.checked ? "Bar Color, Blue" : "Light Color; Blue"
        color: "gray"
    }
    Slider {
        from: 0.0
        to: 1.0
        value: testgradientchange.checked ? barColor.color.b
                                          : customTheme.lightColor.b
        onValueChanged: testgradientchange.checked ? barColor.color.b  = value
                                                   : customTheme.lightColor.b = value
    }


    Label {
        visible: !colorStyle.checked
        text: "Gradient green alpha"
        color: "gray"
    }
    Slider {
        visible: !colorStyle.checked
        from: 0.0
        to: 1.0
        value: 1.0
        onValueChanged: greenstop.color.a = value
    }

    Label {
        visible: !colorStyle.checked
        text: "Gradient red alpha"
        color: "gray"
    }
    Slider {
        visible: !colorStyle.checked
        from: 0.0
        to: 1.0
        value: 1.0
        onValueChanged: redstop.color.a = value
    }

    Label {
        visible: colorStyle.checked
        text: "Bar Color alpha"
        color: "gray"
    }
    Slider {
        visible: colorStyle.checked
        from: 0.0
        to: 1.0
        value: 1.0
        onValueChanged: barColor.color.a = value
    }

    Label {
        text: "Color Style Uniform"
        color: "gray"
    }
    CheckBox {
        id: colorStyle
        checked: (customTheme.colorStyle === Theme3D.ColorStyle.Uniform)
        onCheckedChanged: {
            if (checked)
                customTheme.colorStyle = Theme3D.ColorStyle.Uniform
            else
                customTheme.colorStyle = Theme3D.ColorStyle.ObjectGradient
        }
    }
    Label {
        text: "Test Theme Color/Gradient Change"
        color: "gray"
    }
    CheckBox {
        id: testgradientchange
        checked: false
    }
}
