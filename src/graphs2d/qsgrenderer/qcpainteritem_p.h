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
#ifdef USE_AREAGRAPH
#include <QtGraphs/private/arearenderer_p.h>
#endif
#ifdef USE_BARGRAPH
#include <QtGraphs/private/barsrenderer_p.h>
#endif
#ifdef USE_CUSTOMGRAPH
#include <QtGraphs/private/customrenderer_p.h>
#endif
#ifdef USE_PIEGRAPH
#include <QtGraphs/private/pierenderer_p.h>
#endif
#ifdef USE_POINTS
#include <QtGraphs/private/pointrenderer_p.h>
#endif

QT_BEGIN_NAMESPACE

#ifndef USE_AREAGRAPH
class AreaRenderer;
#endif
#ifndef USE_BARGRAPH
class BarsRenderer;
#endif
#ifndef USE_CUSTOMGRAPH
class CustomRenderer;
#endif
#ifndef USE_PIEGRAPH
class PieRenderer;
#endif
#ifndef USE_POINTS
class PointRenderer;
#endif

class QCPainterItemRenderer : public QCanvasPainterItemRenderer
{
public:
    enum class RendererKind {
#ifdef USE_AREAGRAPH
        Area,
#endif
#ifdef USE_BARGRAPH
        Bars,
#endif
#ifdef USE_PIEGRAPH
        Pie,
#endif
#ifdef USE_POINTS
        Point,
#endif
#ifdef USE_CUSTOMGRAPH
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
#ifdef USE_AREAGRAPH
    AreaRenderer::PaintSnapshot m_areaPaintSnapshot;
#endif
#ifdef USE_BARGRAPH
    BarsRenderer::PaintSnapshot m_barsSnapshot;
#endif
#ifdef USE_PIEGRAPH
    PieRenderer::PaintSnapshot m_piePaintSnapshot;
#endif
#ifdef USE_POINTS
    PointRenderer::PaintSnapshot m_pointPaintSnapshot;
#endif
#ifdef USE_CUSTOMGRAPH
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
