# Copyright (C) 2023 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

qt_internal_add_test(tst_qgsurface_series
    SOURCES
        tst_series.cpp
    LIBRARIES
        Qt::Gui
        Qt::Graphs
)
