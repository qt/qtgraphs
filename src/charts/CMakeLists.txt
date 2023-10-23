# Copyright (C) 2023 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

qt_internal_add_qml_module(Graphs2D
    URI "QtGraphs2D"
    TARGET_PRODUCT "Qt Graphs 2D (Qt $$QT_VERSION)"
    TARGET_DESCRIPTION "Qt Graphs 2D Visualization component for Qt."
    VERSION "${PROJECT_VERSION}"
    PAST_MAJOR_VERSIONS 1
    DEPENDENCIES
        QtQuick
        QtQuickPrivate
    SOURCES
        qquickgraphsview.cpp qquickgraphsview_p.h
        qabstractseries.cpp qabstractseries.h qabstractseries_p.h
        charthelpers_p.h
        domain/abstractdomain.cpp domain/abstractdomain_p.h
        barchart/qbarseries.cpp barchart/qbarseries.h barchart/qbarseries_p.h
        barchart/qabstractbarseries.cpp barchart/qabstractbarseries.h barchart/qabstractbarseries_p.h
        barchart/qbarset.cpp barchart/qbarset.h barchart/qbarset_p.h
        linechart/qlineseries.cpp linechart/qlineseries.h linechart/qlineseries_p.h
        xychart/qxyseries.cpp xychart/qxyseries.h xychart/qxyseries_p.h
        xychart/qxypoint.cpp xychart/qxypoint_p.h
        axis/qabstractaxis.cpp axis/qabstractaxis.h axis/qabstractaxis_p.h
        axis/barcategoryaxis/qbarcategoryaxis.cpp axis/barcategoryaxis/qbarcategoryaxis.h axis/barcategoryaxis/qbarcategoryaxis_p.h
        axis/axisgrid.cpp axis/axisgrid_p.h
        axis/axisticker.cpp axis/axisticker_p.h
        themes/graphtheme.cpp themes/graphtheme.h
        themes/seriestheme.cpp themes/seriestheme.h
    NO_PCH_SOURCES
        barchart/qabstractbarseries.cpp
        axis/barcategoryaxis/qbarcategoryaxis.cpp
        xychart/qxyseries.cpp
    PUBLIC_LIBRARIES
        Qt::Core
        Qt::Gui
        Qt::Quick
        Qt::QuickPrivate
        Qt::QuickShapesPrivate
    GENERATE_CPP_EXPORTS
)

set(shader_resource_files
    "data/gridshader.frag.qsb"
    "data/gridshader.vert.qsb"
    "data/tickershader.frag.qsb"
    "data/tickershader.vert.qsb"
    "data/tickershaderhorizontal.frag.qsb"
    "data/tickershaderhorizontal.vert.qsb"
)

qt_internal_add_resource(Graphs2D "graphsshaders"
    PREFIX
        "/shaders"
    BASE
        "data"
    FILES
        ${shader_resource_files}
)
