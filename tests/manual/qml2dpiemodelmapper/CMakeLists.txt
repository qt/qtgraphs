# Copyright (C) 2026 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)

if (NOT QT_BUILD_STANDALONE_TESTS AND NOT QT_BUILDING_QT)
    project(tst_qml2dpiemodelmapper LANGUAGES C CXX)
    find_package(Qt6BuildInternals COMPONENTS STANDALONE_TEST)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

qt_internal_add_manual_test(tst_qml2dpiemodelmapper
    GUI
    SOURCES
        main.cpp
)

target_link_libraries(tst_qml2dpiemodelmapper PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Qml
    Qt::Quick
    Qt::Graphs
)

set(qml2dpiemodelmapper_resource_files
    "qml/main.qml"
)

qt_internal_add_resource(tst_qml2dpiemodelmapper "qml2dpiemodelmapper"
    PREFIX
        "/"
    FILES
        ${qml2dpiemodelmapper_resource_files}
)

qt_add_qml_module(tst_qml2dpiemodelmapper
    URI Qml2DPieModelMapperExample
    VERSION 1.0
    SOURCES
        piemappermodel.h piemappermodel.cpp
)
