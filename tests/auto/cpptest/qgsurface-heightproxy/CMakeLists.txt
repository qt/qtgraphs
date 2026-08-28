# Copyright (C) 2023 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT QT_BUILD_STANDALONE_TESTS AND NOT QT_BUILDING_QT)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qgsurface_heightproxy LANGUAGES CXX)
    find_package(Qt6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

qt_internal_add_test(tst_qgsurface_heightproxy
    SOURCES
        tst_proxy.cpp
    LIBRARIES
        Qt::Gui
        Qt::Graphs
)

set(qgsurface_heightproxy_resource_files
    "customtexture.jpg"
)

qt_internal_add_resource(tst_qgsurface_heightproxy "qgdsurface_heightproxy"
    PREFIX
        "/"
    FILES
        ${qgsurface_heightproxy_resource_files}
)
