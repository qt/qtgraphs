# Copyright (C) 2023 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT QT_BUILD_STANDALONE_TESTS AND NOT QT_BUILDING_QT)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qgqmltest LANGUAGES CXX)
    find_package(Qt6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

file(GLOB_RECURSE test_data_glob
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    *)
list(APPEND test_data ${test_data_glob})

qt_internal_add_test(tst_qgqmltest
    QMLTEST
    SOURCES
        tst_qmltest.cpp
    LIBRARIES
        Qt::Gui
    TESTDATA ${test_data}
)

set(qmltest_resource_files
    "customitem.mesh"
    "customtexture.jpg"
)

qt_internal_add_resource(tst_qgqmltest "qmltest"
    PREFIX
        "/"
    FILES
        ${qmltest_resource_files}
)
