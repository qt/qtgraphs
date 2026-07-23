# Copyright (C) 2026 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)

if (NOT QT_BUILD_STANDALONE_TESTS AND NOT QT_BUILDING_QT)
    project(tst_customseriespainter LANGUAGES C CXX)
    find_package(Qt6BuildInternals COMPONENTS STANDALONE_TEST)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

qt_internal_add_manual_test(tst_customseriespainter
    GUI
    SOURCES
        main.cpp
        squareseries.cpp squareseries.h
)

target_link_libraries(tst_customseriespainter PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Qml
    Qt::Quick
    Qt::Graphs
    Qt::CanvasPainter
)

set(customseriespainter_resource_files
    "main.qml"
)

qt_internal_add_resource(tst_customseriespainter "customseriespainter"
    PREFIX
        "/"
    FILES
        ${customseriespainter_resource_files}
)

qt_add_qml_module(tst_customseriespainter
    URI CustomSeriesPainterTest
    VERSION 1.0
)
