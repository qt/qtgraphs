// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick3D
import QtQuick

CustomMaterial {
    property TextureInput custex: TextureInput {}
    property TextureInput height: TextureInput {}
    property TextureInput baseColor: TextureInput {}
    property real xDiff: 0.0
    property real yDiff: 0.0
    property real gradientMin
    property real gradientHeight
    property color uniformColor
    property bool flatShading: false
    property int colorStyle: 0

    shadingMode: CustomMaterial.Shaded
    vertexShader: "qrc:/shaders/vertexSurface"
    fragmentShader: "qrc:/shaders/fragmentSurface"
}