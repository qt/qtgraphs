// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "piesimulator.h"
#include <QPieSlice>

PieSimulator::PieSimulator(QObject *parent) : DataSimulator(parent)
{
    connect(this, &DataSimulator::simulateDataCompleted, this, &PieSimulator::generatePieSlices);
}

void PieSimulator::addSeries(QPieSeries *series)
{
    m_seriesList.append(series);
    if (hasData())
        generatePieSlices();
}

void PieSimulator::generatePieSlices()
{
    if (m_seriesList.isEmpty())
        return;

    const auto values = data();

    // Create all slices first
    for (QPieSeries *series : std::as_const(m_seriesList)) {
        QList<QPieSlice *> slices;
        slices.reserve(values.size());

        for (const auto &value : values) {
            auto *slice = new QPieSlice();
            slice->setValue(value.toDouble());
            slices.append(slice);
        }

        series->clear();
        // Append all the slices in one go
        series->append(slices);
    }
}
