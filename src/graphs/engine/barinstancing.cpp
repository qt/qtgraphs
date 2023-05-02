// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "barinstancing_p.h"

BarInstancing::BarInstancing()
{

}

QByteArray BarInstancing::getInstanceBuffer(int *instanceCount)
{
    if (m_dirty) {
        m_instanceData.resize(0);
        int instanceNumber = 0;

        for (int i = 0; i < m_dataArray.size(); ++i) {
            auto item = m_dataArray.at(i);
            auto entry = calculateTableEntry(item.position, item.scale, item.eulerRotation, QColor(Qt::white));
            m_instanceData.append(reinterpret_cast<char *>(&entry), sizeof(entry));
            instanceNumber++;
        }
        m_instanceCount = instanceNumber;
        m_dirty = false;
    }

    if (instanceCount)
        *instanceCount = m_instanceCount;

    return m_instanceData;
}

bool BarInstancing::rangeGradient() const
{
    return m_rangeGradient;
}

void BarInstancing::setRangeGradient(bool newRangeGradient)
{
    m_rangeGradient = newRangeGradient;
}

void BarInstancing::clearDataArray()
{
    m_dataArray.clear();
    m_instanceData.clear();
}

void BarInstancing::markDataDirty()
{
    m_dirty = true;
    markDirty();
}

const QList<BarItemHolder> &BarInstancing::dataArray() const
{
    return m_dataArray;
}

void BarInstancing::setDataArray(const QList<BarItemHolder> &newDataArray)
{
    m_dataArray = newDataArray;
    markDataDirty();
}
