// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
// Qt-Security score:significant reason:default


#ifndef CUSTOMRENDERER_H
#define CUSTOMRENDERER_H

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
#include <QtGraphs/qabstractseries.h>
#include <private/qgraphsglobal_p.h>
#include <private/qquickdraghandler_p.h>

QT_BEGIN_NAMESPACE

class QGraphsView;
class QCustomSeries;
class QCustomSeriesCanvasRenderer;
class AxisRenderer;
struct QLegendData;
#if QT_CONFIG(graphs_2d_high_performance_backend)
class QCanvasPainter;
class QCanvasPainterItem;
#endif

class CustomRenderer : public QQuickItem
{
    Q_PROPERTY(QRectF plotArea READ plotArea NOTIFY plotAreaChanged FINAL)

    Q_OBJECT
public:
    using PaintSnapshot = QList<QCustomSeriesCanvasRenderer *>;

    CustomRenderer(QGraphsView *graph, bool clipPlotArea);
    ~CustomRenderer() override;

#if QT_CONFIG(graphs_2d_high_performance_backend)
    PaintSnapshot paintSnapshot() const;
    void synchronizeData(QCanvasPainterItem *item, QHash<QCustomSeries *, QCustomSeriesCanvasRenderer *> &painters);
    static void paintSnapshot(const PaintSnapshot &snapshot, QCanvasPainter *p);
#endif
    void handlePolish(QCustomSeries *series);
    void afterPolish(QList<QAbstractSeries *> &cleanupSeries);
    void updateSeries(QCustomSeries *series);
    void seriesAboutToBeRemoved(QAbstractSeries *series);
    void afterUpdate(QList<QAbstractSeries *> &cleanupSeries);

    qreal mapX(AxisRenderer *axisRenderer, QCustomSeries *series, qreal x);
    qreal mapY(AxisRenderer *axisRenderer, QCustomSeries *series, qreal y);

    QRectF plotArea() const;

signals:
    void plotAreaChanged();

private:
    struct DataGroup
    {
        QCustomSeries *series = nullptr;
        QList<QQuickItem *> dataItems;
        QQmlComponent *currentDataItem = nullptr;
        QQmlComponent *previousDataItem = nullptr;
    };

    QGraphsView *m_graph = nullptr;
    QMap<QCustomSeries *, DataGroup *> m_groups;
#if QT_CONFIG(graphs_2d_high_performance_backend)
    PaintSnapshot m_paintSnapshot;
    QSet<QCustomSeries *> m_removedCustomSeries;
#endif

    // Render area variables
    qreal m_maxVertical = 0;
    qreal m_maxHorizontal = 0;
    qreal m_verticalOffset = 0;
    qreal m_horizontalOffset = 0;
    qreal m_areaWidth = 0;
    qreal m_areaHeight = 0;
    QRectF m_plotArea;

    void hideDelegates(QCustomSeries *series);
};

QT_END_NAMESPACE

#endif // CUSTOMRENDERER_H
