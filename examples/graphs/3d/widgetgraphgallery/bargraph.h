// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef BARGRAPH_H
#define BARGRAPH_H

#include <QtCore/qobject.h>
#include <QtGraphsWidgets/q3dbarswidget.h>
#include "graphmodifier.h"

class BarGraph : public QObject
{
    Q_OBJECT
public:
    BarGraph();

    void initialize();
    QWidget *barsWidget() { return m_barsWidget; }

private:
    GraphModifier *m_modifier = nullptr;
    Q3DBarsWidget *m_barsGraph = nullptr;
    QWidget *m_barsWidget = nullptr;
};

#endif
