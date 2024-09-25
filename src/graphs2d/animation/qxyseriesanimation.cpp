// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "qxyseriesanimation_p.h"

QXYSeriesAnimation::QXYSeriesAnimation(QObject *parent)
    : QGraphAnimation(parent)
{}

QXYSeriesAnimation::~QXYSeriesAnimation() {}

void QXYSeriesAnimation::updateCurrent(QGraphTransition::TransitionType tt, int index, QPointF point)
{
    m_currentTransitionType = tt;
    m_newPointIndex = index;
    m_newPoint = point;

    if (m_previousTransitionType == QGraphTransition::TransitionType::None)
        m_previousTransitionType = m_currentTransitionType;

    if (animating() == QGraphAnimation::AnimationState::Stopped)
        m_activePointIndex = index;
}
