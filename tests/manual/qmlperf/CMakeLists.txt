# Copyright (C) 2023 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

set(CMAKE_INCLUDE_CURRENT_DIR ON)

qt_internal_add_manual_test(tst_qmlperf
    GUI
    SOURCES
        datagenerator.cpp datagenerator.h
        main.cpp
    )
target_link_libraries(tst_qmlperf PUBLIC
    Qt::Gui
    Qt::Graphs
    )

set(qmlperf_resource_files
    "qml/qmlperf/main.qml"
    )

qt_internal_add_resource(tst_qmlperf "qmlperf"
    PREFIX
    "/"
    FILES
    ${qmlperf_resource_files}
    )
