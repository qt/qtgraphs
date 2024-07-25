# Copyright (C) 2023 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)

if (NOT QT_BUILD_STANDALONE_TESTS AND NOT QT_BUILDING_QT)
    project(tst_galaxy LANGUAGES C CXX ASM)
    find_package(Qt6BuildInternals COMPONENTS STANDALONE_TEST)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fexceptions")

qt_add_executable(tst_galaxy
    main.cpp
    cumulativedistributor.cpp cumulativedistributor.h
    galaxydata.cpp galaxydata.h   
    star.cpp star.h
    )

target_link_libraries(tst_galaxy PUBLIC
    Qt::Gui
    Qt::Widgets
    Qt::Graphs
    )
