// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef BARSERIESHELPER_H
#define BARSERIESHELPER_H

#include "cpuusageupdater.h"

#include <QObject>
#include <QQmlEngine>
#include <QtGraphs/QBarSeries>
#include <QtCore/QTimer>

class BarSeries : public QBarSeries
{
    Q_OBJECT
    QML_NAMED_ELEMENT(CustomBar)

public:
    BarSeries(QBarSeries *parent = nullptr);

private:
    QBarSeries *m_barSeries;
    QList<QBarSet *> m_barList;
    QTimer m_timer;
    CpuUsageUpdater m_cpuUpdater;

    void frameUpdate();
};

#endif // BARSERIESHELPER_H
