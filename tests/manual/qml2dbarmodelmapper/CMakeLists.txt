# Copyright (C) 2026 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)

if (NOT QT_BUILD_STANDALONE_TESTS AND NOT QT_BUILDING_QT)
    project(tst_qml2dbarmodelmapper LANGUAGES C CXX)
    find_package(Qt6BuildInternals COMPONENTS STANDALONE_TEST)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

qt_internal_add_manual_test(tst_qml2dbarmodelmapper
    GUI
    SOURCES
        main.cpp
        barmodelmappermodel.cpp barmodelmappermodel.h
    )

target_link_libraries(tst_qml2dbarmodelmapper PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Qml
    Qt::Quick
    Qt::Graphs
)

set(qml2dbarmodelmapper_resource_files
    "qml/qml2dbarmodelmapper/main.qml"
    "qml/qml2dbarmodelmapper/SettingsView.qml"
)

qt6_add_resources(tst_qml2dbarmodelmapper "qml2dbarmodelmapper"
    PREFIX
        "/"
    FILES
        ${qml2dbarmodelmapper_resource_files}
)

qt_add_qml_module(tst_qml2dbarmodelmapper
    URI qml2dbarmodelmapper
    VERSION 1.0
    SOURCES
        barmodelmappermodel.cpp barmodelmappermodel.h
)
