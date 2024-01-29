// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef SCATTERSERIES_H
#define SCATTERSERIES_H

#include "cpuusageupdater.h"

#include <QObject>
#include <QQmlEngine>
#include <QScatterSeries>
#include <QtCore/QTimer>

class ScatterSeries : public QScatterSeries
{
    Q_OBJECT
    QML_NAMED_ELEMENT(CustomScatter)
public:
    ScatterSeries(QScatterSeries *series = nullptr);

private:
    QList<QPointF> m_points;
    QTimer m_timer;
    CpuUsageUpdater m_cpuUpdater;

    void frameUpdate();
};

#endif // SCATTERSERIES_H
