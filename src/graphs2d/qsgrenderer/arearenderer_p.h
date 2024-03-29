// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef AREARENDERER_H
#define AREARENDERER_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the QtGraphs API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#include <QQuickItem>
#include <QtQuickShapes/private/qquickshape_p.h>

QT_BEGIN_NAMESPACE

class QGraphsView;
class QAreaSeries;
class AxisRenderer;

class AreaRenderer : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT
public:
    AreaRenderer(QQuickItem *parent = nullptr);
    virtual ~AreaRenderer();

    void handlePolish(QAreaSeries *series);
    void updateSeries(QAreaSeries *series);
    bool handleMousePress(QMouseEvent *event);
    bool handleHoverMove(QHoverEvent *event);

Q_SIGNALS:

private:
    struct PointGroup
    {
        QAreaSeries *series;
        QQuickShapePath *shapePath = nullptr;
        QList<QQuickCurve *> paths;
        int colorIndex = -1;
        int borderColorIndex = -1;
        bool hover = false;
    };

    QGraphsView *m_graph = nullptr;
    QQuickShape m_shape;
    QMap<QAreaSeries *, PointGroup *> m_groups;

    // Render area variables
    qreal m_maxVertical = 0;
    qreal m_maxHorizontal = 0;
    qreal m_verticalOffset = 0;
    qreal m_horizontalOffset = 0;
    qreal m_areaWidth = 0;
    qreal m_areaHeight = 0;

    void calculateRenderCoordinates(qreal origX, qreal origY, qreal *renderX, qreal *renderY) const;
    void calculateAxisCoordinates(qreal origX, qreal origY, qreal *axisX, qreal *axisY) const;
    bool pointInArea(QPoint pt, QAreaSeries *series) const;
};

QT_END_NAMESPACE

#endif // AREARENDERER_H
