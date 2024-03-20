// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick3D
import QtQuick

CustomMaterial {
    property TextureInput custex: TextureInput {}
    property int colorStyle
    property color uniformColor
    property bool isHighlight
    property real specularBrightness: 0.25
    readonly property real shininess: (1.0 - specularBrightness) * 100

    shadingMode: CustomMaterial.Shaded
    sourceBlend: CustomMaterial.SrcAlpha
    destinationBlend: CustomMaterial.OneMinusSrcAlpha
    depthDrawMode: Material.OpaquePrePassDepthDraw

    vertexShader: "qrc:/shaders/objectgradientvert"
    fragmentShader: "qrc:/shaders/barsfrag"
}
