// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtGraphs
import QtQuick
import AerospaceHubExample

pragma ComponentBehavior: Bound

Bars3D {
    id: graph

    property alias series: barseries3d
    property var dataModel: GlobalSettings.barDataModel
    property bool dataready: GlobalSettings.bar3ddataready

    onDatareadyChanged: {
        // TODO: Anything needs doing here?
        if (dataready) {
            console.log("Data ready on Bars3D: " + this)
        }
    }

    msaaSamples: 8

    theme: GlobalSettings.theme

    visible: opacity > 0.5

    rotationEnabled: false
    zoomEnabled: false

    Bar3DSeries {
        id: barseries3d

        colorStyle: GlobalSettings.colorstyle

        ItemModelBarDataProxy {
            itemModel: graph.dataModel
            rowRole: "row"
            columnRole: "column"
            valueRole: "value"
        }

        // TODO: Does this make sense in this graph, or should it be only in scatter?
        // onSelectedBarChanged: (position) => {
        //     GlobalSettings.windspeed = itemLabel
        //     GlobalSettings.turbineid = position.x + "/" + position.y
        // }
    }
}
