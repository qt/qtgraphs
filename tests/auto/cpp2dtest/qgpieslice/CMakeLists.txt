# Copyright (C) 2024 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

qt_internal_add_test(tst_qgpieslice
    SOURCES
        tst_qgpieslice.cpp
    INCLUDE_DIRECTORIES
        ../common
    LIBRARIES
        Qt::Graphs
        Qt::Gui
        Qt::GuiPrivate
)
