// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef SCATTERGRAPH_H
#define SCATTERGRAPH_H

#include <QtCore/qobject.h>
#include <QtGraphsWidgets/q3dscatterwidget.h>
#include "scatterdatamodifier.h"

class ScatterGraph : public QObject
{
    Q_OBJECT
public:
    ScatterGraph();

    void initialize();
    QWidget *scatterWidget() { return m_scatterWidget; }

private:
    ScatterDataModifier *m_modifier = nullptr;
    Q3DScatterWidget *m_scatterGraph = nullptr;
    QWidget *m_scatterWidget = nullptr;
};

#endif
