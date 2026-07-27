// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QCPAINTERITEM_H
#define QCPAINTERITEM_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the QtGraphs API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#include <QtCanvasPainter/QCanvasPainter>
#include <QtCanvasPainter/QCanvasPainterItem>
#include <QtCanvasPainter/QCanvasPainterItemRenderer>
#include <QtCore/qvarlengtharray.h>
#include <private/qgraphsglobal_p.h>
#if QT_CONFIG(graphs_2d_area)
#include <QtGraphs/private/arearenderer_p.h>
#endif
#if QT_CONFIG(graphs_2d_bar)
#include <QtGraphs/private/barsrenderer_p.h>
#endif
#if QT_CONFIG(graphs_2d_custom)
#include <QtGraphs/private/customrenderer_p.h>
#endif
#if QT_CONFIG(graphs_2d_donut_pie)
#include <QtGraphs/private/pierenderer_p.h>
#endif
#if QT_CONFIG(graphs_2d_area) || QT_CONFIG(graphs_2d_line) || QT_CONFIG(graphs_2d_scatter) || QT_CONFIG(graphs_2d_spline)
#include <QtGraphs/private/pointrenderer_p.h>
#endif

QT_BEGIN_NAMESPACE

#if !QT_CONFIG(graphs_2d_area)
class AreaRenderer;
#endif
#if !QT_CONFIG(graphs_2d_bar)
class BarsRenderer;
#endif
#if !QT_CONFIG(graphs_2d_custom)
class CustomRenderer;
#endif
#if !QT_CONFIG(graphs_2d_donut_pie)
class PieRenderer;
#endif
#if !QT_CONFIG(graphs_2d_area) && !QT_CONFIG(graphs_2d_line) && !QT_CONFIG(graphs_2d_scatter) && !QT_CONFIG(graphs_2d_spline)
class PointRenderer;
#endif

class QCPainterItemRenderer : public QCanvasPainterItemRenderer
{
public:
    enum class RendererKind {
#if QT_CONFIG(graphs_2d_area)
        Area,
#endif
#if QT_CONFIG(graphs_2d_bar)
        Bars,
#endif
#if QT_CONFIG(graphs_2d_donut_pie)
        Pie,
#endif
#if QT_CONFIG(graphs_2d_area) || QT_CONFIG(graphs_2d_line) || QT_CONFIG(graphs_2d_scatter) || QT_CONFIG(graphs_2d_spline)
        Point,
#endif
#if QT_CONFIG(graphs_2d_custom)
        Custom,
#endif
    };

    QCPainterItemRenderer();
    ~QCPainterItemRenderer() override;

    void initializeResources(QCanvasPainter *p) override;
    void synchronizeData(QCanvasPainterItem *item) override;
    void prePaint(QCanvasPainter *painter) override;
    void paint(QCanvasPainter *p) override;

private:
#if QT_CONFIG(graphs_2d_area)
    AreaRenderer::PaintSnapshot m_areaPaintSnapshot;
#endif
#if QT_CONFIG(graphs_2d_bar)
    BarsRenderer::PaintSnapshot m_barsSnapshot;
#endif
#if QT_CONFIG(graphs_2d_donut_pie)
    PieRenderer::PaintSnapshot m_piePaintSnapshot;
#endif
#if QT_CONFIG(graphs_2d_area) || QT_CONFIG(graphs_2d_line) || QT_CONFIG(graphs_2d_scatter) || QT_CONFIG(graphs_2d_spline)
    PointRenderer::PaintSnapshot m_pointPaintSnapshot;
#endif
#if QT_CONFIG(graphs_2d_custom)
    CustomRenderer::PaintSnapshot m_customPaintSnapshot;
    QHash<QCustomSeries *, QCustomSeriesCanvasRenderer *> m_customPainters;
#endif

    QVarLengthArray<RendererKind, 5> m_rendererSlots{};
    friend class QCPainterItem;
};

class QCPainterItem : public QCanvasPainterItem
{
    Q_OBJECT

public:
    QCPainterItem(AreaRenderer *&areaRenderer,
                  BarsRenderer *&barsRenderer,
                  PieRenderer *&pieRenderer,
                  PointRenderer *&pointRenderer,
                  CustomRenderer *&customRenderer,
                  QQuickItem *parent = nullptr);

    QCPainterItemRenderer *createItemRenderer() const override;

private:
    AreaRenderer *&m_areaRenderer;
    BarsRenderer *&m_barsRenderer;
    PieRenderer *&m_pieRenderer;
    PointRenderer *&m_pointRenderer;
    CustomRenderer *&m_customRenderer;

    friend class QCPainterItemRenderer;
};

QT_END_NAMESPACE

#endif // QCPAINTERITEM_H
