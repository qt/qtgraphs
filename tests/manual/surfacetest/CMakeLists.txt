# Copyright (C) 2023 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

set(CMAKE_INCLUDE_CURRENT_DIR ON)

qt_internal_add_manual_test(tst_surfacetest
    GUI
    SOURCES
        buttonwrapper.cpp buttonwrapper.h
        checkboxwrapper.cpp checkboxwrapper.h
        graphmodifier.cpp graphmodifier.h
        main.cpp
    )
target_link_libraries(tst_surfacetest PUBLIC
    Qt::Gui
    Qt::Widgets
    Qt::Graphs
    )

set_source_files_properties("Heightmap.png"
    PROPERTIES QT_RESOURCE_ALIAS "map"
    )
set_source_files_properties("mapimage.png"
    PROPERTIES QT_RESOURCE_ALIAS "mapimage"
    )
set(surfacetest_resource_files
    "Heightmap.png"
    "mapimage.png"
    )

qt_internal_add_resource(tst_surfacetest "surfacetest"
    PREFIX
    "/maps"
    FILES
    ${surfacetest_resource_files}
    )
