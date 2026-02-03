// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

pragma ComponentBehavior: Bound

import QtGraphs
import QtQuick
import AerospaceHubExample

Scatter3D {
    id: graph

    property alias series: scatterseries3d
    property var dataModel: GlobalSettings.scatterDataModel
    property bool dataready: GlobalSettings.scatter3ddataready

    onDatareadyChanged: {
        // TODO: Anything needs doing here?
        if (dataready) {
            console.log("Data ready on Scatter3D: " + this)
        }
    }

    msaaSamples: 8

    theme: GlobalSettings.theme

    visible: opacity > 0.5

    rotationEnabled: false
    zoomEnabled: false

    Scatter3DSeries {
        id: scatterseries3d

        colorStyle: GlobalSettings.colorstyle

        ItemModelScatterDataProxy {
            itemModel: graph.dataModel
            xPosRole: "x"
            yPosRole: "y"
            zPosRole: "z"
        }

        onSelectedItemChanged: (index) => {
            if (index < 0)
                return
            let values = itemLabel.split(", ")
            GlobalSettings.windspeed = values[1]
            var x = values[0].split(".")[0]
            var y = values[2].split(".")[0]
            GlobalSettings.turbineid = x + "/" + y
        }
    }
}
