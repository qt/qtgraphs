// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "barsimulator.h"
#include <QBarSet>

BarSimulator::BarSimulator(QObject *parent) : DataSimulator(parent)
{
    connect(this, &DataSimulator::simulateDataCompleted, this, &BarSimulator::generateBarSet);
}

int BarSimulator::numberOfCategory() const
{
    return m_numberOfCategory;
}

void BarSimulator::setNumberOfCategory(int numberOfCategory)
{
    if (m_numberOfCategory == numberOfCategory)
        return;

    m_numberOfCategory = numberOfCategory;
    emit numberOfCategoryChanged(numberOfCategory);
}

void BarSimulator::addSeries(QBarSeries *series)
{
    m_seriesList.append(series);
    if (hasData())
        generateBarSet();
}

void BarSimulator::generateData()
{
    DataSimulator::generateData(1, m_numberOfCategory);
}

void BarSimulator::generateBarSet()
{
    if (m_seriesList.isEmpty())
        return;

    const auto values = data();

    for (QBarSeries *series : std::as_const(m_seriesList)) {
        // Create all bar sets first
        QList<QBarSet *> sets;
        sets.reserve(values.size() / m_numberOfCategory);

        for (int i = 0; i < values.size() - m_numberOfCategory + 1; i += m_numberOfCategory) {
            auto *barSet = new QBarSet();
            barSet->setValues(values.sliced(i, m_numberOfCategory));
            sets.append(barSet);
        }

        series->clear();
        // Append all the sets in one go
        series->append(sets);
    }
}
