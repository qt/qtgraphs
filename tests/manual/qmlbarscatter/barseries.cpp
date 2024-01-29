// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "barseries.h"

#include <QtGraphs/QBarCategoryAxis>
#include <QtGraphs/QBarSet>
#include <QtGraphs/QValueAxis>

BarSeries::BarSeries(QBarSeries *series)
    : m_cpuUpdater(this)
{
    m_barSeries = series;

    connect(&m_timer, &QTimer::timeout, this, &BarSeries::frameUpdate);
    m_timer.start(100);

    QPointer<QBarCategoryAxis> axis1 = new QBarCategoryAxis(this);
    axis1->setCategories({"Reading 1", "Reading 2", "Reading 3", "Reading 4", "Reading 5"});

    QPointer<QValueAxis> axis2 = new QValueAxis(this);
    axis2->setMin(0);
    axis2->setMax(100);

    setAxisX(axis1);
    setAxisY(axis2);

    m_barList.append(new QBarSet(this));

    for (int i = 0; i < 5; ++i)
        m_barList[0]->append(double(0));
}

void BarSeries::frameUpdate()
{
    auto reading = m_cpuUpdater.update();
    static int counter = 0;

    m_barList[0]->replace(counter++, reading);

    if (counter == 5)
        counter = 0;
}
