// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtCore/QPointF>
#include <QtGraphs/QLineSeries>
#include "private/qgraphpointanimation_p.h"
#include "private/qgraphtransition_p.h"
#include "private/qxyseries_p.h"

QGraphPointAnimation::QGraphPointAnimation(QObject *parent)
    : QXYSeriesAnimation(parent)
{
    setDuration(800);
    setEasingCurve(QEasingCurve::OutCubic);
}

QGraphPointAnimation::~QGraphPointAnimation() {}

void QGraphPointAnimation::setAnimatingValue(QVariant &start, QVariant &end)
{
    setStartValue(start);
    setEndValue(end);
}

QVariant QGraphPointAnimation::interpolated(const QVariant &start,
                                            const QVariant &end,
                                            qreal progress) const
{
    auto startPoint = qvariant_cast<QPointF>(start);
    auto endPoint = qvariant_cast<QPointF>(end);

    auto interpolatedPoint = QPointF{startPoint.x() * (1.0 - progress) + endPoint.x() * progress,
                                     startPoint.y() * (1.0 - progress) + endPoint.y() * progress};

    return QVariant::fromValue(interpolatedPoint);
}

void QGraphPointAnimation::animate()
{
    // Hierarchy should look like GraphAnimation -> ParallelAnimationGroup -> GraphTransition -> QXYSeries
    auto series = qobject_cast<QXYSeries *>(parent()->parent()->parent());

    if (!series || series->points().size() < 2)
        return;

    if (animating() == QGraphAnimation::AnimationState::Playing) {
        end();
        m_activePointIndex = m_newPointIndex;
    }

    setAnimating(QGraphAnimation::AnimationState::Playing);

    auto &pointList = series->d_func()->m_points;

    switch (m_currentTransitionType) {
    default:
    case QGraphTransition::TransitionType::PointAdded: {
        auto startv = QVariant::fromValue(pointList[pointList.size() - 2]);
        auto endv = QVariant::fromValue(pointList[pointList.size() - 1]);

        setAnimatingValue(startv, endv);
    } break;
    case QGraphTransition::TransitionType::PointReplaced: {
        auto startv = QVariant::fromValue(m_pointsState[m_activePointIndex]);
        auto endv = QVariant::fromValue(pointList[m_newPointIndex]);

        setAnimatingValue(startv, endv);
    } break;
    case QGraphTransition::TransitionType::PointRemoved: {
        pointList.insert(m_newPointIndex, m_pointsState[m_newPointIndex]);
        auto startv = QVariant::fromValue(m_pointsState[m_newPointIndex]);

        if (m_newPointIndex == 0 || m_newPointIndex == m_pointsState.size() - 1) {
            auto endv = QVariant::fromValue(m_pointsState[m_newPointIndex - 1]);
            setAnimatingValue(startv, endv);
        } else {
            auto lastPoint = m_pointsState[m_newPointIndex - 1];
            auto nextPoint = m_pointsState[m_newPointIndex + 1];
            auto midpoint = QPointF{(nextPoint.x() + lastPoint.x()) * 0.5,
                                    (nextPoint.y() + lastPoint.y()) * 0.5};

            auto endv = QVariant::fromValue(midpoint);
            setAnimatingValue(startv, endv);
        }
    } break;
    }

    m_pointsState = pointList;
}

void QGraphPointAnimation::end()
{
    auto series = qobject_cast<QXYSeries *>(parent()->parent()->parent());

    if (!series || animating() == QGraphAnimation::AnimationState::Stopped)
        return;

    setAnimating(QGraphAnimation::AnimationState::Stopped);
    stop();

    auto &points = series->d_func()->m_points;

    switch (m_currentTransitionType) {
    default:
    case QGraphTransition::TransitionType::PointAdded: {
        points.replace(m_activePointIndex, qvariant_cast<QPointF>(endValue()));
    } break;
    case QGraphTransition::TransitionType::PointReplaced: {
        m_pointsState.replace(m_activePointIndex, qvariant_cast<QPointF>(endValue()));
    } break;
    case QGraphTransition::TransitionType::PointRemoved: {
        points.remove(m_activePointIndex);
    } break;
    }

    emit series->update();
}

void QGraphPointAnimation::valueUpdated(const QVariant &value)
{
    auto series = qobject_cast<QXYSeries *>(parent()->parent()->parent());

    if (!series)
        return;

    auto val = qvariant_cast<QPointF>(value);
    auto &points = series->d_func()->m_points;

    switch (m_currentTransitionType) {
    default:
    case QGraphTransition::TransitionType::PointAdded: {
        points.replace(m_activePointIndex, val);
    } break;
    case QGraphTransition::TransitionType::PointReplaced: {
        points.replace(m_activePointIndex, val);
    } break;
    case QGraphTransition::TransitionType::PointRemoved: {
        points.replace(m_activePointIndex, val);
    } break;
    }

    emit series->update();
}
