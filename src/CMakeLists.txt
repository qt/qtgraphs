# Copyright (C) 2023 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

qt_feature_evaluate_features("${CMAKE_CURRENT_SOURCE_DIR}/graphs2d/configure.cmake")
qt_feature_evaluate_features("${CMAKE_CURRENT_SOURCE_DIR}/graphs3d/configure.cmake")

qt_configure_add_summary_section(NAME "Qt Graphs Support")
qt_configure_add_summary_entry(ARGS "graphs-2d")
qt_configure_add_summary_entry(ARGS "graphs-3d")
qt_configure_end_summary_section() # end of "Qt Graphs Support" section

if(QT_FEATURE_graphs_3d)
    add_subdirectory(graphs3d)
endif()

if(QT_FEATURE_graphs_2d)
    add_subdirectory(graphs2d)
endif()

if(NOT QT_FEATURE_graphs_2d AND NOT QT_FEATURE_graphs_3d)
    message("You must have either FEATURE_graphs_2d or FEATURE_graphs_3d ON, or nothing will be built.")
endif()
