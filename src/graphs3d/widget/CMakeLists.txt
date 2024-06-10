# Copyright (C) 2024 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

qt_internal_add_module(GraphsWidgets
    SOURCES
        qgraphswidgetsglobal.h
        qabstract3dgraphwidget.cpp qabstract3dgraphwidget.h
        q3dbarswidget.cpp q3dbarswidget.h
        q3dscatterwidget.cpp q3dscatterwidget.h
        q3dsurfacewidget.cpp q3dsurfacewidget.h
    DEFINES
        QT_NO_FOREACH
        QT_NO_INTEGER_EVENT_COORDINATES
        QT_NO_URL_CAST_FROM_STRING
    LIBRARIES
        Qt::GraphsPrivate
    PUBLIC_LIBRARIES
        Qt::Graphs
        Qt::QuickWidgets
    PRIVATE_MODULE_INTERFACE
        Qt::GraphsPrivate
)
