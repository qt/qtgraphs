# Copyright (C) 2023 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

set(CMAKE_INCLUDE_CURRENT_DIR ON)

qt_internal_add_manual_test(tst_rotations
    GUI
    SOURCES
        main.cpp
        scatterdatamodifier.cpp scatterdatamodifier.h
    )

target_link_libraries(tst_rotations PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Widgets
    Qt::Graphs
)

set(rotations_resource_files
    "mesh/largesphere.mesh"
    "mesh/narrowarrow.mesh"
)

qt6_add_resources(tst_rotations "rotations"
    PREFIX
        "/"
    FILES
        ${rotations_resource_files}
)
