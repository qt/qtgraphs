// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "dynamicsplineseries.h"

#include <QtCore/QRandomGenerator>

DynamicSplineSeries::DynamicSplineSeries(QObject *parent)
    : QSplineSeries(parent)
    , m_intervals(6)
    , m_intervalDuration(1200)
    , m_once(false)
    , m_xValue(1)
    , m_customAxis(parent)
    , m_minAnimation(new QPropertyAnimation(parent))
    , m_maxAnimation(new QPropertyAnimation(parent))
    , m_animationGroup(new QParallelAnimationGroup(parent))
{
    connect(&m_updateTimer, &QTimer::timeout, this, &DynamicSplineSeries::updateData);
    m_updateTimer.start(m_intervalDuration);

    m_customAxis.setMax(m_intervals);

    setAxisX(&m_customAxis);
    m_customAxis.setTickInterval(1);

    const auto animationTime = m_intervalDuration * 0.8;
    m_maxAnimation.setDuration(animationTime);
    m_maxAnimation.setEasingCurve(QEasingCurve::OutCubic);
    m_minAnimation.setDuration(animationTime);
    m_minAnimation.setEasingCurve(QEasingCurve::OutCubic);

    m_maxAnimation.setTargetObject(&m_customAxis);
    m_minAnimation.setTargetObject(&m_customAxis);

    m_maxAnimation.setPropertyName("max");
    m_minAnimation.setPropertyName("min");
}

DynamicSplineSeries::~DynamicSplineSeries()
{
    m_animationGroup.stop();

    if (m_once) {
        m_animationGroup.removeAnimation(&m_maxAnimation);
        m_animationGroup.removeAnimation(&m_minAnimation);
    }
}

void DynamicSplineSeries::updateData()
{
    auto y = QRandomGenerator::global()->bounded(4.0);

    append(QPointF(m_xValue++, y));

    if (m_xValue > (m_intervals / 2) + 1) {
        m_maxAnimation.setStartValue(m_customAxis.max());
        m_maxAnimation.setEndValue(m_customAxis.max() + 1);
        m_minAnimation.setStartValue(m_customAxis.min());
        m_minAnimation.setEndValue(m_customAxis.min() + 1);

        if (!m_once) {
            m_once = true;
            m_animationGroup.addAnimation(&m_maxAnimation);
            m_animationGroup.addAnimation(&m_minAnimation);
        }
    }

    m_animationGroup.start();
}
