// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
// Qt-Security score:significant reason:default


#ifndef POLARAXISRENDERER_H
#define POLARAXISRENDERER_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the QtGraphs API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#include <QtQuickShapes/private/qquickshape_p.h>
#include <private/axisrenderer_p.h>

QT_BEGIN_NAMESPACE

class QPolarView;

class PolarAxisRenderer : public AxisRenderer
{
    Q_OBJECT
public:
    explicit PolarAxisRenderer(QQuickItem *parent = nullptr);
    ~PolarAxisRenderer() override;

    void handlePolish() override;

    virtual qreal radialLabelBand() const { return m_radialLabelBand; }

private:
    void updateAxisMeasurements() override;
    bool zoomsHorizontalAxis() const override { return false; }

    void computeAxisProperties(AxisProperties &ax, QAbstractAxis *axis);
    void updatePolarGrid(const QList<qreal> &radialTicks);
    void updatePolarLabels(const QList<qreal> &radialTicks);
    QList<qreal> radialTickValues(const AxisProperties &radial) const;
    int angularTickCount(const AxisProperties &angular) const;

    QQuickShape m_shape;
    QQuickShapePath *m_ringsPath = nullptr;
    QQuickShapePath *m_spokesPath = nullptr;

    qreal m_radialLabelBand = 0;
};

QT_END_NAMESPACE

#endif // POLARAXISRENDERER_H
