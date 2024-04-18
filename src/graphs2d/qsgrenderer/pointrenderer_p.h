// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef POINTRENDERER_H
#define POINTRENDERER_H

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
#include <QtQuick/private/qsgdefaultinternalrectanglenode_p.h>

QT_BEGIN_NAMESPACE

class QGraphsView;
class QXYSeries;
class QLineSeries;
class QScatterSeries;
class QSplineSeries;
class AxisRenderer;
struct QLegendData;

class PointRenderer : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT
public:
    PointRenderer(QQuickItem *parent = nullptr);
    ~PointRenderer() override;

    void handlePolish(QXYSeries *series);
    void updateSeries(QXYSeries *series);
    bool handleMouseMove(QMouseEvent *event);
    bool handleMousePress(QMouseEvent *event);
    bool handleMouseRelease(QMouseEvent *event);
    bool handleHoverMove(QHoverEvent *event);

Q_SIGNALS:

private:
    struct PointGroup
    {
        QXYSeries *series;
        QQuickShapePath *shapePath = nullptr;
        QList<QQuickPathElement *> paths;
        QList<QQuickItem *> markers;
        QQmlComponent *currentMarker = nullptr;
        QQmlComponent *previousMarker = nullptr;
        QList<QRectF> rects;
        int colorIndex = -1;
        bool hover = false;
    };

    QQmlComponent *m_tempMarker = nullptr;

    QGraphsView *m_graph = nullptr;
    QQuickShape m_shape;
    QMap<QXYSeries *, PointGroup *> m_groups;
    int m_currentColorIndex = 0;

    // Point drag variables
    bool m_pointPressed = false;
    bool m_pointDragging = false;
    QPoint m_pressStart;
    PointGroup *m_pressedGroup = nullptr;
    int m_pressedPointIndex = 0;

    // Render area variables
    qreal m_maxVertical = 0;
    qreal m_maxHorizontal = 0;
    qreal m_verticalOffset = 0;
    qreal m_horizontalOffset = 0;
    qreal m_areaWidth = 0;
    qreal m_areaHeight = 0;

    qreal defaultSize(QXYSeries *series = nullptr);

    void calculateRenderCoordinates(
        AxisRenderer *axisRenderer, qreal origX, qreal origY, qreal *renderX, qreal *renderY);
    void updatePointMarker(QXYSeries *series, PointGroup *group, int pointIndex, qreal x, qreal y);
    void updateLegendData(QXYSeries *series, QLegendData &legendData);

    void updateScatterSeries(QScatterSeries *scatter, QLegendData &legendData);
    void updateLineSeries(QLineSeries *line, QLegendData &legendData);
    void updateSplineSeries(QSplineSeries *spline, QLegendData &legendData);
};

QT_END_NAMESPACE

#endif // POINTRENDERER_H
