// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtCore/QPointF>
#include "QSplineSeries"
#include "private/qsplinecontrolanimation_p.h"
#include "private/qsplineseries_p.h"

QSplineControlAnimation::QSplineControlAnimation(QObject *parent)
    : QXYSeriesAnimation(parent)
{
    setDuration(800);
    setEasingCurve(QEasingCurve::OutCubic);
}

QSplineControlAnimation::~QSplineControlAnimation() {}

void QSplineControlAnimation::setAnimatingValue(const QVariant &start, const QVariant &end)
{
    setStartValue(start);
    setEndValue(end);
}

QVariant QSplineControlAnimation::interpolated(const QVariant &start,
                                               const QVariant &end,
                                               qreal progress) const
{
    auto startList = qvariant_cast<QList<QPointF>>(start);
    auto endList = qvariant_cast<QList<QPointF>>(end);
    auto interpolateList = QList<QPointF>();

    for (int i = 0; i < endList.size(); ++i) {
        interpolateList.push_back(
                { qreal(startList[i].x() * (1.0 - progress) + endList[i].x() * progress),
                  qreal(startList[i].y() * (1.0 - progress) + endList[i].y() * progress) });
    }

    return QVariant::fromValue(interpolateList);
}

void QSplineControlAnimation::animate()
{
    // Hierarchy should look like GraphAnimation -> ParallelAnimationGroup -> GraphTransition -> SplineSeries
    auto series = qobject_cast<QSplineSeries *>(parent()->parent()->parent());

    if (!series || series->points().size() < 2)
        return;

    auto pointList = series->points();

    if (animating() == QGraphAnimation::AnimationState::Playing)
        end();

    setAnimating(QGraphAnimation::AnimationState::Playing);

    while (m_oldState.size() < m_newState.size()) {
        // Each point corresponds to a 2n - 1 control point pair other than the first
        // (Except when there are only 2 points)
        // 0 ---- 0
        // 1 ---- 1
        //   ---- 2
        // 2 ---- 3
        //   ---- 4 ...
        QPointF point = pointList[m_oldState.size() / 2];
        m_oldState.append(point);
    }

    auto varStart = QVariant::fromValue(m_oldState);
    auto varEnd = QVariant::fromValue(m_newState);

    setAnimatingValue(varStart, varEnd);
}

void QSplineControlAnimation::end()
{
    auto series = qobject_cast<QSplineSeries *>(parent()->parent()->parent());

    if (!series || animating() == QGraphAnimation::AnimationState::Stopped)
        return;

    setAnimating(QGraphAnimation::AnimationState::Stopped);
    stop();

    emit series->update();
}

void QSplineControlAnimation::setOldState(const QList<QPointF> &oldState)
{
    m_oldState = oldState;
}

void QSplineControlAnimation::setNewState(const QList<QPointF> &newState)
{
    m_newState = newState;
}

void QSplineControlAnimation::valueUpdated(const QVariant &value)
{
    auto series = qobject_cast<QSplineSeries *>(parent()->parent()->parent());

    if (!series)
        return;

    auto &cPoints = series->d_func()->m_controlPoints;
    auto points = qvariant_cast<QList<QPointF>>(value);

    for (int i = 0; i < points.size(); ++i)
        cPoints.replace(i, points[i]);

    emit series->update();
}
