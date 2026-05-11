
# Copyright (C) 2026 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

qt_internal_add_qml_module(Graphs2DImpl
    URI "QtGraphs2D.impl"
    VERSION "${PROJECT_VERSION}"
    CLASS_NAME QtGraphs2DimplPlugin
    PLUGIN_TARGET qtgraphs2dimplplugin
    QML_FILES
        PointMarker.qml
        ZoomAreaDelegate.qml
    DEFINES
        QT_NO_CAST_FROM_ASCII
        QT_NO_CAST_TO_ASCII
    LIBRARIES
        Qt::CorePrivate
        Qt::GuiPrivate
        Qt::QmlPrivate
        Qt::QuickPrivate
    NO_PRIVATE_MODULE
    )
