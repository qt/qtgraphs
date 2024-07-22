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

#ifndef QSPLINECONTROLANIMATION_P_H
#define QSPLINECONTROLANIMATION_P_H

#include "qqmlintegration.h"
#include <private/qxyseriesanimation_p.h>

QT_BEGIN_NAMESPACE

class QSplineSeries;

class QSplineControlAnimation : public QXYSeriesAnimation
{
    Q_OBJECT
    QML_NAMED_ELEMENT(SplineControlAnimation)

public:
    explicit QSplineControlAnimation(QObject *parent = nullptr);
    ~QSplineControlAnimation() override;

    void setAnimatingValue(const QVariant &start, const QVariant &end) override;
    QVariant interpolated(const QVariant &start, const QVariant &end, qreal progress) const override;

    void animate() override;
    void end() override;

    void setOldState(const QList<QPointF> &oldState);
    void setNewState(const QList<QPointF> &newState);

public Q_SLOTS:
    void valueUpdated(const QVariant &value) override;

private:
    QList<QPointF> m_oldState;
    QList<QPointF> m_newState;
};

QT_END_NAMESPACE

#endif // QSPLINECONTROLANIMATION_P_H
