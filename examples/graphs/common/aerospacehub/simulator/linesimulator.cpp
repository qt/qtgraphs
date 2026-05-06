// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "linesimulator.h"

LineSimulator::LineSimulator(QObject *parent) : DataSimulator(parent)
{
    connect(this, &DataSimulator::simulateDataCompleted, this, &LineSimulator::generatePoints);
}

void LineSimulator::addSeries(QSplineSeries *series)
{
    m_seriesList.append(series);
    if (hasData())
        generatePoints();
}

void LineSimulator::generateData()
{
    DataSimulator::generateData(1, 1);
}

void LineSimulator::generatePoints()
{
    if (m_seriesList.isEmpty())
        return;

    // Create a temp point list
    QList<QPointF> points;
    points.reserve(data().size());

    // Populate temp list
    for (int i = 0; i < data().size(); ++i)
        points.append(QPointF(i + 1, qAbs(data().at(i).toReal())));

    // Replace points of each series in one go
    for (QSplineSeries *series : std::as_const(m_seriesList))
        series->replace(points);
}
