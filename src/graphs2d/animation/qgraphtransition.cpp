// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtCore/QList>
#include "QtGraphs/QSplineSeries"
#include "QtGraphs/QXYSeries"
#include "private/qgraphtransition_p.h"
#include "private/qsplinecontrolanimation_p.h"
#include "private/qsplineseries_p.h"
#include "private/qxyseries_p.h"
#include "private/qxyseriesanimation_p.h"

QGraphTransition::QGraphTransition(QObject *parent)
    : QObject(parent)
    , m_animationGroup(this)
{}

QGraphTransition::~QGraphTransition() {}

void QGraphTransition::componentComplete()
{
    // Currently only assuming animations in QXYSeries
    Q_ASSERT(parent() != nullptr && qobject_cast<QXYSeries *>(parent()));
    auto series = qobject_cast<QXYSeries *>(parent());

    if (series)
        series->d_func()->m_graphTransition = this;
}

void QGraphTransition::onPointChanged(TransitionType type, int index)
{
    auto series = qobject_cast<QXYSeries *>(parent());

    if (!series || !series->hasLoaded())
        return;

    if (m_animationGroup.state() == QAbstractAnimation::Running)
        m_animationGroup.stop();

    for (auto child : m_animationGroup.children()) {
        auto childAnimation = qobject_cast<QXYSeriesAnimation *>(child);
        childAnimation->updateCurrent(type, index);

        auto splineAnimation = qobject_cast<QSplineControlAnimation *>(child);

        if (splineAnimation) {
            splineAnimation->setOldState(qobject_cast<QSplineSeries *>(series)->getControlPoints());
            qobject_cast<QSplineSeries *>(series)->d_func()->calculateSplinePoints();
            splineAnimation->setNewState(qobject_cast<QSplineSeries *>(series)->getControlPoints());
        }
    }

    for (auto child : m_animationGroup.children()) {
        auto childAnimation = qobject_cast<QXYSeriesAnimation *>(child);
        childAnimation->animate();
    }

    m_animationGroup.start();
}

void QGraphTransition::initialize()
{
    auto series = qobject_cast<QXYSeries *>(parent());

    if (!series)
        return;

    for (auto child : m_animationGroup.children()) {
        auto childAnimation = qobject_cast<QXYSeriesAnimation *>(child);
        childAnimation->initialize(series->points());

        auto splineAnimation = qobject_cast<QSplineControlAnimation *>(child);

        if (splineAnimation) {
            splineAnimation->setOldState(qobject_cast<QSplineSeries *>(series)->getControlPoints());
            splineAnimation->setNewState(qobject_cast<QSplineSeries *>(series)->getControlPoints());
        }
    }
}

void QGraphTransition::stop()
{
    m_animationGroup.stop();

    for (auto child : m_animationGroup.children()) {
        auto childAnimation = qobject_cast<QXYSeriesAnimation *>(child);
        childAnimation->end();
    }
}

QQmlListProperty<QObject> QGraphTransition::animations()
{
    return QQmlListProperty<QObject>{this,
                                     nullptr,
                                     &QGraphTransition::append,
                                     nullptr,
                                     nullptr,
                                     &QGraphTransition::clear};
}

void QGraphTransition::classBegin() {}

void QGraphTransition::append(QQmlListProperty<QObject> *animationProps, QObject *animation)
{
    auto graphTransition = qobject_cast<QGraphTransition *>(animationProps->object);

    if (graphTransition) {
        auto graphAnimation = qobject_cast<QGraphAnimation *>(animation);
        graphTransition->m_animations.append(graphAnimation);
        graphTransition->m_animationGroup.addAnimation(graphAnimation);
    }
}

void QGraphTransition::clear(QQmlListProperty<QObject> *)
{
    Q_UNIMPLEMENTED();
}
