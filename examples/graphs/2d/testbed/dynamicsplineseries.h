// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef DYNAMICSPLINESERIES_H
#define DYNAMICSPLINESERIES_H

#include <QtCore/QObject>
#include <QtCore/QParallelAnimationGroup>
#include <QtCore/QPropertyAnimation>
#include <QtCore/QTimer>
#include <QtQml/QQmlEngine>
#include "QtGraphs/QSplineSeries"
#include "qvalueaxis.h"

class DynamicSplineSeries : public QSplineSeries
{
    Q_OBJECT
    QML_ELEMENT
public:
    DynamicSplineSeries(QObject *parent = nullptr);
    ~DynamicSplineSeries();

public Q_SLOTS:
    void updateData();

private:
    const int m_intervals;
    const int m_intervalDuration;
    bool m_once;
    QTimer m_updateTimer;
    int m_xValue;
    QValueAxis m_customAxis;
    QPropertyAnimation m_minAnimation;
    QPropertyAnimation m_maxAnimation;
    QParallelAnimationGroup m_animationGroup;
};

#endif // DYNAMICSPLINESERIES_H
