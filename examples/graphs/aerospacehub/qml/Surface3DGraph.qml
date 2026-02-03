// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

pragma ComponentBehavior: Bound

import QtGraphs
import QtQuick
import AerospaceHubExample

Surface3D {
    id: graph

    property alias series: surfaceseries3d
    property var dataModel: GlobalSettings.surfaceDataModel
    property bool dataready: GlobalSettings.surface3ddataready

    onDatareadyChanged: {
        // TODO: Anything needs doing here?
        if (dataready) {
            console.log("Data ready on Surface3D: " + this)
        }
    }

    msaaSamples: 8

    theme: GlobalSettings.theme

    visible: opacity > 0.5

    rotationEnabled: false
    zoomEnabled: false

    Surface3DSeries {
        id: surfaceseries3d

        colorStyle: GlobalSettings.colorstyle

        ItemModelSurfaceDataProxy {
            itemModel: graph.dataModel
            rowRole: "row"
            columnRole: "column"
            yPosRole: "y"
        }
    }
}
