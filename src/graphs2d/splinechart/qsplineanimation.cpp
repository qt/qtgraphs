// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtCore/QPointF>
#include "QSplineSeries"
#include "qsplineanimation_p.h"

QSplineAnimation::QSplineAnimation(QObject *parent, QSplineSeries *spline)
    : QGraphAnimation(parent)
    , m_series(spline)
{
    setDuration(800);
    setEasingCurve(QEasingCurve::OutCubic);
}

void QSplineAnimation::setAnimatingValue(QVariant &start, QVariant &end)
{
    setStartValue(start);
    setEndValue(end);
}

QVariant QSplineAnimation::interpolated(const QVariant &start,
                                        const QVariant &end,
                                        qreal progress) const
{
    auto startList = qvariant_cast<QList<QPointF>>(start);
    auto endList = qvariant_cast<QList<QPointF>>(end);
    auto interpolateList = QList<QPointF>();

    interpolateList.push_back({startList[0].x() * (1.0 - progress) + endList[0].x() * progress,
                               startList[0].y() * (1.0 - progress) + endList[0].y() * progress});

    interpolateList.push_back({startList[1].x() * (1.0 - progress) + endList[1].x() * progress,
                               startList[1].y() * (1.0 - progress) + endList[1].y() * progress});

    interpolateList.push_back({startList[2].x() * (1.0 - progress) + endList[2].x() * progress,
                               startList[2].y() * (1.0 - progress) + endList[2].y() * progress});

    return QVariant::fromValue(interpolateList);
}

void QSplineAnimation::animate()
{
    setAnimating(QGraphAnimation::AnimationState::Playing);

    auto pointList = m_series->points();
    auto cPoints = m_series->getControlPoints();

    auto pointStartValue = pointList[pointList.size() - 2];
    auto pointEndValue = pointList[pointList.size() - 1];

    auto firstCurveHandleStartValue = pointList[pointList.size() - 2];
    auto firstCurveHandleEndValue = cPoints[cPoints.size() - 1];

    auto secondCurveHandleStartValue = pointList[pointList.size() - 2];
    auto secondCurveHandleEndValue = cPoints[cPoints.size() - 2];

    QList<QPointF> startValues;
    QList<QPointF> endValues;

    startValues.append(pointStartValue);
    startValues.append(firstCurveHandleStartValue);
    startValues.append(secondCurveHandleStartValue);

    endValues.append(pointEndValue);
    endValues.append(firstCurveHandleEndValue);
    endValues.append(secondCurveHandleEndValue);

    auto varStart = QVariant::fromValue(startValues);
    auto varEnd = QVariant::fromValue(endValues);

    setAnimatingValue(varStart, varEnd);
    start();
}

void QSplineAnimation::valueUpdated(const QVariant &value)
{
    // Current interpolated value
    auto cval = qvariant_cast<QList<QPointF>>(value);

    auto pointValue = cval.at(0);
    auto firstCurveHandleValue = cval.at(1);
    auto secondCurveHandleValue = cval.at(2);

    auto &cPoints = m_series->getControlPoints();
    auto points = m_series->points();

    m_series->replace(points.size() - 1, pointValue);
    cPoints.replace(cPoints.size() - 1, firstCurveHandleValue);
    cPoints.replace(cPoints.size() - 2, secondCurveHandleValue);
    emit m_series->update();
}
