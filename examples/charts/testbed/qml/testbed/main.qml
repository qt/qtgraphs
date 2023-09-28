// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Controls

Window {
    id: rootWindow

    readonly property url startupView: "qrc:/qml/testbed/StartupView.qml"
    readonly property real px: Math.max(width, height) / 1280
    readonly property real iconSize: 30 + 30 * px
    readonly property real fontSizeSmall: 16 * px
    readonly property real fontSizeLarge: 26 * px

    width: 1280
    height: 720
    visible: true
    title: qsTr("Qt Quick Graphs Testbed")
    color: "#000000"

    Loader {
        id: loader
        anchors.fill: parent
        Component.onCompleted: setSource(rootWindow.startupView, {loader: loader})
    }

    Button {
        id: backButton
        anchors.left: parent.left
        anchors.top: parent.top
        implicitWidth: rootWindow.iconSize
        implicitHeight: rootWindow.iconSize
        opacity: loader.source != "" && loader.source != rootWindow.startupView
        visible: opacity
        icon.source: "qrc:/qml/testbed/images/arrow_icon.png"
        icon.width: backButton.width * 0.3
        icon.height: backButton.height * 0.3
        icon.color: "transparent"
        background: Rectangle {
            color: "transparent"
        }
        onClicked: {
            loader.setSource(rootWindow.startupView, {loader: loader})
        }
        Behavior on opacity {
            NumberAnimation {
                duration: 400
                easing.type: Easing.InOutQuad
            }
        }
    }

}
