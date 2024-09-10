// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtCore/QPointF>
#include <QtGraphs/QLineSeries>
#include "private/qgraphpointanimation_p.h"
#include "private/qgraphtransition_p.h"
#include "private/qxyseries_p.h"

/*!
    \qmltype GraphPointAnimation
    \inqmlmodule QtGraphs
    \ingroup graphs_qml_2D
    \brief An animation type which signifies the animation for points.

GraphPointAnimation is an animation type derived from QVariantAnimation which defines how points are animated.
It can make use of QVariantAnimation functionality and properties for its animations, such as \c duration and \c easing.
These animations are housed inside of a QParallelAnimationGroup and hence will run in parallel.

This example shows how to use a GraphPointAnimation to set points to animate with
a \c duration of 1000ms and \c easing of OutCubic:

\snippet doc_src_qmlgraphs.cpp 12

For XYSeries, this is considered to be the main list of points defined inside
the series. The point is linearly interpolated from the start to the end value.

\sa GraphTransition, SplineControlAnimation

*/

QGraphPointAnimation::QGraphPointAnimation(QObject *parent)
    : QXYSeriesAnimation(parent)
{
    setDuration(800);
    setEasingCurve(QEasingCurve::OutCubic);
}

QGraphPointAnimation::~QGraphPointAnimation() {}

void QGraphPointAnimation::setAnimatingValue(const QVariant &start, const QVariant &end)
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

    auto interpolatedPoint = QPointF{
        qreal(startPoint.x() * (1.0 - progress) + endPoint.x() * progress),
        qreal(startPoint.y() * (1.0 - progress) + endPoint.y() * progress),
    };

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
            auto midpoint = QPointF{
                qreal((nextPoint.x() + lastPoint.x()) * 0.5),
                qreal((nextPoint.y() + lastPoint.y()) * 0.5),
            };

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
