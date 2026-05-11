// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick

Rectangle {
    property bool pointSelected
    property color pointColor
    property color pointBorderColor
    property color pointSelectedColor
    property real pointBorderWidth

    color: pointSelected ? pointSelectedColor : pointColor
    border.color: pointBorderColor
    border.width: pointBorderWidth
}
