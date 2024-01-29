// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "scatterseries.h"
#include <QtGraphs/QValueAxis>

ScatterSeries::ScatterSeries(QScatterSeries *series)
    : m_cpuUpdater(this)
{
    connect(&m_timer, &QTimer::timeout, this, &ScatterSeries::frameUpdate);
    m_timer.start(100);

    for (int i = 0; i < 5; ++i)
        append(QPointF(double(i), double(i)));
}

void ScatterSeries::frameUpdate()
{
    auto reading = m_cpuUpdater.update();
    static int counter = 0;

    replace(counter, counter, reading);
    counter++;

    emit update();

    if (counter == 5)
        counter = 0;
}
