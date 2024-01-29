// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef PROCESSORINFO_H
#define PROCESSORINFO_H

#include <QVector>
#include <Pdh.h>
#include <windows.h>

class ProcessorInfo
{
public:
    ProcessorInfo();

    double updateTime();

private:
    const double maxSamples = 15.0;
    int m_currentSample;
    double m_normalizedUsage;

    PDH_HQUERY m_cpuQuery;
    PDH_HCOUNTER m_cpuTotal;
};

#endif // PROCESSORINFO_H
