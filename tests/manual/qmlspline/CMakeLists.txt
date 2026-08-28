# Copyright (C) 2024 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

if(NOT QT_BUILD_STANDALONE_TESTS AND NOT QT_BUILDING_QT)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qmlspline LANGUAGES CXX)
    find_package(Qt6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

qt_internal_add_manual_test(tst_qmlspline
    GUI
    SOURCES
        main.cpp
    )

target_sources(tst_qmlspline
  PRIVATE
    splinegen.h splinegen.cpp
)

target_link_libraries(tst_qmlspline PUBLIC
    Qt::Gui
    Qt::Graphs
    )

set(qmlspline_resource_files
    "qml/qmlspline/main.qml"
    )

qt_internal_add_resource(tst_qmlspline "qmlspline"
    PREFIX
    "/"
    FILES
    ${qmlspline_resource_files}
    )
