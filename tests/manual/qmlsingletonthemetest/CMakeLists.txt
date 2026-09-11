# Copyright (C) 2026 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

qt_internal_add_manual_test(tst_qmltestsingletontheme
    GUI
    SOURCES
        main.cpp
)

target_link_libraries(tst_qmltestsingletontheme
    PRIVATE Qt6::Qml
    PRIVATE Qt6::Gui
    PRIVATE Qt6::Graphs
    PRIVATE Qt6::QuickTest
)

# no need to copy around qml test files for shadow builds - just set the respective define
add_definitions(-DQUICK_TEST_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

file(GLOB_RECURSE QML_FILES *.qml)
set_source_files_properties(SingletonTheme.qml
    PROPERTIES QT_QML_SINGLETON_TYPE TRUE)
add_custom_target(singletontheme_qmlfiles SOURCES ${QML_FILES})

qt_add_qml_module(tst_qmltestsingletontheme
    URI SingletonThemeTest
    QML_FILES
        SingletonTheme.qml
)
