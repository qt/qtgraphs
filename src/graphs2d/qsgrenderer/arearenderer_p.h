// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
// Qt-Security score:significant reason:default


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

#include <QPainterPath>
#include <QQuickItem>
#ifdef USE_SHAPE_BACKEND
#include <QtQuickShapes/private/qquickshape_p.h>
#endif

QT_BEGIN_NAMESPACE

class QGraphsView;
class QAreaSeries;
class AxisRenderer;
class QAbstractSeries;
class QQuickTapHandler;
#ifdef USE_PAINTER_BACKEND
class QCanvasPainter;
#endif
class QQuickShapeGradient;

class AreaRenderer : public QQuickItem
{
    Q_OBJECT
public:
    AreaRenderer(QGraphsView *graph, bool clipPlotArea);
    ~AreaRenderer() override;

    void resetShapePathCount();

#ifdef USE_PAINTER_BACKEND
    void canvasPaint(QCanvasPainter *p);
#endif
    void handlePolish(QAreaSeries *series);
    void afterPolish(QList<QAbstractSeries *> &cleanupSeries);
    void afterUpdate(QList<QAbstractSeries *> &cleanupSeries);
    void updateSeries(QAreaSeries *series);
    bool handleHoverMove(QHoverEvent *event);

Q_SIGNALS:

private:
    struct PointGroup
    {
        QAreaSeries *series = nullptr;
        QPainterPath painterPath;
        qsizetype colorIndex = -1;
        qsizetype borderColorIndex = -1;
        bool hover = false;
#ifdef USE_SHAPE_BACKEND
        QQuickShapePath *shapePath = nullptr;
#endif
    };

    void onSingleTapped(QEventPoint eventPoint, Qt::MouseButton button);
    void onDoubleTapped(QEventPoint eventPoint, Qt::MouseButton button);
    void onPressedChanged();

    QGraphsView *m_graph = nullptr;
    QMap<QAreaSeries *, PointGroup *> m_groups;
    qsizetype m_currentShapePathIndex = 0;

#ifdef USE_SHAPE_BACKEND
    QQuickShape m_shape;
#endif

    // Render area variables
    qreal m_maxVertical = 0;
    qreal m_maxHorizontal = 0;
    qreal m_verticalOffset = 0;
    qreal m_horizontalOffset = 0;
    qreal m_areaWidth = 0;
    qreal m_areaHeight = 0;

    QQuickTapHandler *m_tapHandler = nullptr;

    struct SeriesStyle
    {
        QColor color;
        QQuickShapeGradient *gradient;
        QColor borderColor;
        qreal borderWidth;
    };

    SeriesStyle getSeriesStyle(PointGroup *group);

    void calculateRenderCoordinates(
        QAreaSeries *series, qreal origX, qreal origY, qreal *renderX, qreal *renderY) const;
    void calculateAxisCoordinates(
        QAreaSeries *series, qreal origX, qreal origY, qreal *axisX, qreal *axisY) const;
    bool pointInArea(QPoint pt, QAreaSeries *series) const;
};

QT_END_NAMESPACE

#endif // AREARENDERER_H
