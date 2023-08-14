// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick3D
import QtQuick

CustomMaterial {
    property color gridColor: 'black'
    property TextureInput height: TextureInput {}

    property vector2d rangeMin
    property vector2d rangeMax
    property bool xDesc
    property bool zDesc

    vertexShader: "qrc:/shaders/vertexSurfaceGrid"
    fragmentShader: "qrc:/shaders/fragSurfaceGrid"
}
