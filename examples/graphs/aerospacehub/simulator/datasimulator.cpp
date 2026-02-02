// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
#include "datasimulator.h"

#include <QRandomGenerator>
#include <QDebug>
#include <QVariant>

DataSimulator::DataSimulator(QObject *parent) : QObject(parent)
{ }

qreal DataSimulator::max() const
{
    return m_max;
}

void DataSimulator::setMax(qreal max)
{
    if (qFuzzyCompare(m_max, max))
        return;

    m_max = max;
    emit maxChanged(max);
}

qreal DataSimulator::min() const
{
    return m_min;
}

void DataSimulator::setMin(qreal min)
{
    if (qFuzzyCompare(m_min, min))
        return;

    m_min = min;
    emit minChanged(min);
}

int DataSimulator::numberOfData() const
{
    return m_numberOfData;
}

void DataSimulator::setNumberOfData(int numberOfData)
{
    if (m_numberOfData == numberOfData)
        return;

    m_numberOfData = numberOfData;
    emit numberOfDataChanged(numberOfData);
}

bool DataSimulator::live() const
{
    return m_live;
}

void DataSimulator::setLive(bool live)
{
    if (m_live == live)
        return;

    m_live = live;
    emit liveChanged(live);
}

DataSimulator::SortingOrder DataSimulator::order() const
{
    return m_order;
}

void DataSimulator::setOrder(DataSimulator::SortingOrder order)
{
    if (m_order == order)
        return;

    m_order = order;
    emit orderChanged(order);
}

int DataSimulator::deviation() const
{
    return m_deviation;
}

void DataSimulator::setDeviation(int deviation)
{
    if (m_deviation == deviation)
        return;

    m_deviation = deviation;
    emit deviationChanged(deviation);
}

QVariantList DataSimulator::data()
{
    return m_Data;
}

void DataSimulator::generateData()
{
    generateData(1, 1);
}

void DataSimulator::generateData(int numberOfSet, int numberOfColumns, int numberOfRows)
{
    int numberOfCategory = numberOfColumns * numberOfRows;
    int size = m_numberOfData * numberOfCategory * numberOfSet;
    qreal range = m_max - m_min;
    qreal prev = m_order == SortingOrder::Ascending ? m_min : m_max;
    m_Data.clear();

    for (int i = 0; i < size; i++) {
        qreal randData;
        qreal diff;
        // TODO : DataSimulaotr just generates random numbers (i.e. diff). The actual data (i.e. m_min + randData) should be done at each types of simulator
        if (m_order != SortingOrder::NotSorted) {
            diff = QRandomGenerator::global()->bounded(range / numberOfSet);
            if (m_deviation > 0) {
                if (numberOfSet == 2) {
                    if (i % numberOfSet)
                        diff *= QRandomGenerator::global()->bounded(-1, m_deviation);
                    else
                        diff *= QRandomGenerator::global()->bounded(0, m_deviation);
                } else {
                    diff *= QRandomGenerator::global()->bounded(-1 * m_deviation, m_deviation);
                }
            }
            if (m_order == SortingOrder::Ascending) {
                if (numberOfSet == 2)
                    randData = (prev + diff) < m_min ? prev - diff : prev + diff;
                else
                    randData = prev + diff;
            } else if (m_order == SortingOrder::Descending)
                randData = prev - diff;
        } else {
            diff = QRandomGenerator::global()->bounded(range);
            randData = diff;
        }

        m_Data.append(QVariant(m_min + randData));
        if (i + 1 < numberOfSet)
            continue;
        prev = m_Data.at(m_Data.size() - numberOfSet).toReal();
    }

    m_hasData = true;
    emit simulateDataCompleted(m_Data, numberOfColumns, numberOfRows, m_numberOfData);
}

bool DataSimulator::hasData() const
{
    return m_hasData;
}
