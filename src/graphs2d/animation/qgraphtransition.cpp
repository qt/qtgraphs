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

/*!
    \qmltype GraphTransition
    \inqmlmodule QtGraphs
    \ingroup graphs_qml_2D
    \brief A container in which all animations are defined.

GraphTransition is a container for animations inside on Graphs2D. Define this class
inside a graph type to enable animated changes for XYSeries within 2D graphs. To
define individual animations, add them inside of the GraphTransition. The
individual animations within the same GraphTransition are animated in parallel. If
a GraphTransition is found by the graph during a call to a supported function
which appends or replaces a point, then the values are interpolated according to
the animations that are added.

This example shows how to define a GraphTransition within a graph.

\snippet doc_src_qmlgraphs.cpp 11

Note: GraphTransition requires it to be defined directly inside the graph which needs to be animated.
Currently only XYSeries are supported.

\sa GraphPointAnimation, SplineControlAnimation
*/

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

/*!
    \qmlproperty list<object> GraphTransition::animations
    A container for all the animations in the GraphTransition.
    Currently only supports animations to be added and cleared.
    By default, the list is empty.
 */
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
