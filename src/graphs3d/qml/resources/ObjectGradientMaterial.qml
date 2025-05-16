// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial

import QtQuick3D
import QtQuick

CustomMaterial {
    property TextureInput custex: TextureInput {}

    shadingMode: CustomMaterial.Shaded
    vertexShader: "qrc:/shaders/objectgradientvert"
    fragmentShader: "qrc:/shaders/objectgradientfrag"
}
