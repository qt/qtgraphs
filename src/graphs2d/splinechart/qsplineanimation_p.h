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

#ifndef QSPLINEANIMATION_P_H
#define QSPLINEANIMATION_P_H

#include <private/qgraphanimation_p.h>

QT_BEGIN_NAMESPACE

class QSplineSeries;

class QSplineAnimation : public QGraphAnimation
{
    Q_OBJECT

public:
    explicit QSplineAnimation(QObject *parent, QSplineSeries *spline);

    void setAnimatingValue(QVariant &start, QVariant &end) override;
    QVariant interpolated(const QVariant &start, const QVariant &end, qreal progress) const override;

    void animate() override;

public Q_SLOTS:
    void valueUpdated(const QVariant &value) override;

private:
    QSplineSeries *m_series;
};

QT_END_NAMESPACE

#endif // QSPLINEANIMATION_P_H
