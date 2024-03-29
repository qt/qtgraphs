# Copyright (C) 2023 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

set(CMAKE_INCLUDE_CURRENT_DIR ON)

qt_internal_add_manual_test(tst_volumetrictest
    GUI
    SOURCES
        main.cpp
        volumetrictest.cpp volumetrictest.h
    )

target_link_libraries(tst_volumetrictest PUBLIC
    Qt::Gui
    Qt::Widgets
    Qt::Graphs
    )

set_source_files_properties("cubeFilledFlat.mesh"
    PROPERTIES QT_RESOURCE_ALIAS "mesh"
    )
set(volumetrictest_resource_files
    "cubeFilledFlat.mesh"
    "logo.png"
    "logo_no_padding.png"
    )

qt_internal_add_resource(tst_volumetrictest "volumetrictest"
    PREFIX
    "/"
    FILES
    ${volumetrictest_resource_files}
    )
