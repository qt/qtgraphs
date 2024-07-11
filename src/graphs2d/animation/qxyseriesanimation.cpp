// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "qxyseriesanimation_p.h"

QXYSeriesAnimation::QXYSeriesAnimation(QObject *parent)
    : QGraphAnimation(parent)
{}

QXYSeriesAnimation::~QXYSeriesAnimation() {}

// Hold state so previous values can be lerped
void QXYSeriesAnimation::initialize(QList<QPointF> state)
{
    m_pointsState = state;
}

void QXYSeriesAnimation::updateCurrent(QGraphTransition::TransitionType tt, int index)
{
    m_currentTransitionType = tt;
    m_newPointIndex = index;

    if (animating() == QGraphAnimation::AnimationState::Stopped)
        m_activePointIndex = index;
}
