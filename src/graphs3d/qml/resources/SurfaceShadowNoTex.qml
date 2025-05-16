// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial

import QtQuick3D
import QtQuick

CustomMaterial {
    property vector3d lightPosition_wrld

    shadingMode: CustomMaterial.Unshaded
    fragmentShader: "qrc:/shaders/fragmentSurfaceShadowNoTex"
}
