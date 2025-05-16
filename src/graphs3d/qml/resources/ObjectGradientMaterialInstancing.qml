// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial

import QtQuick3D
import QtQuick

CustomMaterial {
    property TextureInput custex: TextureInput {}

    shadingMode: CustomMaterial.Shaded
    fragmentShader: "qrc:/shaders/objectgradientinstancingfrag"
    vertexShader: "qrc:/shaders/objectgradientinstancingvert"
}
