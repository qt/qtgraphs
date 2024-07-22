// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "qgraphanimation_p.h"
#include <private/qgraphanimation_p.h>

QGraphAnimation::QGraphAnimation(QObject *parent)
    : QVariantAnimation(parent)
{
    connect(this, &QVariantAnimation::valueChanged, this, &QGraphAnimation::valueUpdated);
    connect(this, &QVariantAnimation::finished, this, &QGraphAnimation::end);
}

QGraphAnimation::~QGraphAnimation()
{
    stop();
}

QGraphAnimation::AnimationState QGraphAnimation::animating() const
{
    return m_animating;
}

void QGraphAnimation::setAnimating(AnimationState newAnimating)
{
    if (m_animating == newAnimating)
        return;
    m_animating = newAnimating;
    emit animatingChanged();
}
