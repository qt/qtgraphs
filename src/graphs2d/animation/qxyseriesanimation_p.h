// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

//  W A R N I N G
//  -------------
//
// This file is not part of the QtGraphs API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#ifndef QXYSERIESANIMATION_H
#define QXYSERIESANIMATION_H

#include <QtCore/QObject>
#include <QtCore/QPointF>
#include "private/qgraphanimation_p.h"

QT_BEGIN_NAMESPACE

class QXYSeriesAnimation : public QGraphAnimation
{
    Q_OBJECT
public:
    explicit QXYSeriesAnimation(QObject *parent = nullptr);

    void initialize(QList<QPointF> state);
    void updateCurrent(QGraphTransition::TransitionType tt, int index);

protected:
    QGraphTransition::TransitionType m_currentTransitionType;
    int m_activePointIndex;
    int m_newPointIndex;
    QList<QPointF> m_pointsState;

    // QGraphAnimation interface
public:
    void setAnimatingValue(QVariant &start, QVariant &end) override = 0;
    void animate() override = 0;
    void end() override = 0;

public Q_SLOTS:
    void valueUpdated(const QVariant &value) override = 0;
};

QT_END_NAMESPACE

#endif // QXYSERIESANIMATION_H
