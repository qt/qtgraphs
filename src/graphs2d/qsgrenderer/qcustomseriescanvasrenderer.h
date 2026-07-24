// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
// Qt-Security score:significant reason:default

#ifndef QTGRAPHS_QCUSTOMSERIESCANVASRENDERER_H
#define QTGRAPHS_QCUSTOMSERIESCANVASRENDERER_H

#include <QtGraphs/qgraphsglobal.h>

QT_BEGIN_NAMESPACE

class QCanvasPainter;
class QCustomSeries;
class QCanvasPainterItem;

class Q_GRAPHS_EXPORT QCustomSeriesCanvasRenderer
{
public:
    QCustomSeriesCanvasRenderer() = default;
    virtual ~QCustomSeriesCanvasRenderer();
    virtual void initializeResources(QCustomSeries *series, QCanvasPainter *p);
    virtual void synchronizeData(QCustomSeries *series, QCanvasPainterItem *item);
    virtual void canvasPrePaint(QCanvasPainter *p);
    virtual void canvasPaint(QCanvasPainter *p) = 0;

private:
    Q_DISABLE_COPY_MOVE(QCustomSeriesCanvasRenderer)
};

QT_END_NAMESPACE

#endif // QTGRAPHS_QCUSTOMSERIESCANVASRENDERER_H
