// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick3D
import QtQuick

CustomMaterial {
    property TextureInput custex: TextureInput {}
    property TextureInput height: TextureInput {}
    property real xDiff: 0.0
    property real yDiff: 0.0
    property bool flatShading: false

    shadingMode: CustomMaterial.Shaded
    vertexShader: "qrc:/shaders/vertexSurface"
    fragmentShader: "qrc:/shaders/fragmentrangegradientsurface"
}
