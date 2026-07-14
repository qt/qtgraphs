// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
#include "qcpainteritem_p.h"

#include <private/arearenderer_p.h>
#include <private/barsrenderer_p.h>
#include <private/customrenderer_p.h>
#include <private/pierenderer_p.h>
#include <private/pointrenderer_p.h>

QT_BEGIN_NAMESPACE

QCPainterItemRenderer::QCPainterItemRenderer() = default;

QCPainterItemRenderer::~QCPainterItemRenderer() = default;

void QCPainterItemRenderer::initializeResources(QCanvasPainter *p)
{
    Q_UNUSED(p);
}

void QCPainterItemRenderer::prePaint(QCanvasPainter *painter)
{
    Q_UNUSED(painter);
}

void QCPainterItemRenderer::paint(QCanvasPainter *p) {
    for (auto rendererKind : std::as_const(m_rendererSlots)) {
        switch (rendererKind) {
#ifdef USE_AREAGRAPH
        case RendererKind::Area:
            AreaRenderer::paintSnapshot(m_areaPaintSnapshot, p);
            break;
#endif
#ifdef USE_BARGRAPH
        case RendererKind::Bars:
            BarsRenderer::paintSnapshot(m_barsSnapshot, p);
            break;
#endif
#ifdef USE_PIEGRAPH
        case RendererKind::Pie:
            PieRenderer::paintSnapshot(m_piePaintSnapshot, p);
            break;
#endif
#ifdef USE_POINTS
        case RendererKind::Point:
            PointRenderer::paintSnapshot(m_pointPaintSnapshot, p);
            break;
#endif
        }
    }
}

void QCPainterItemRenderer::synchronizeData(QCanvasPainterItem *item)
{
    auto graphsPainterItem = static_cast<QCPainterItem *>(item);
    std::array<QQuickItem *, 5> renderers = { graphsPainterItem->m_areaRenderer, graphsPainterItem->m_barsRenderer, graphsPainterItem->m_pieRenderer,
                                              graphsPainterItem->m_pointRenderer, graphsPainterItem->m_customRenderer };
    std::stable_sort(renderers.begin(),
                     renderers.end(),
                     [](QQuickItem *lhs, QQuickItem *rhs) {
                         if (!lhs)
                             return false;
                         if (!rhs)
                             return true;
                         return lhs->z() < rhs->z();
                     });
    m_rendererSlots.clear();
    for (auto renderer : renderers) {
#ifdef USE_AREAGRAPH
        if (auto area = qobject_cast<AreaRenderer *>(renderer)) {
            area->synchronizeData();
            m_areaPaintSnapshot = area->paintSnapshot();
            m_rendererSlots.append(RendererKind::Area);
            continue;
        }
#endif
#ifdef USE_BARGRAPH
        if (auto bars = qobject_cast<BarsRenderer *>(renderer)) {
            bars->synchronizeData();
            m_barsSnapshot = bars->paintSnapshot();
            m_rendererSlots.append(RendererKind::Bars);
            continue;
        }
#endif
#ifdef USE_PIEGRAPH
        if (auto pie = qobject_cast<PieRenderer *>(renderer)) {
            pie->synchronizeData();
            m_piePaintSnapshot = pie->paintSnapshot();
            m_rendererSlots.append(RendererKind::Pie);
            continue;
        }
#endif
#ifdef USE_POINTS
        if (auto point = qobject_cast<PointRenderer *>(renderer)) {
            point->synchronizeData();
            m_pointPaintSnapshot = point->paintSnapshot();
            m_rendererSlots.append(RendererKind::Point);
            continue;
        }
#endif
    }
}

QCPainterItem::QCPainterItem(AreaRenderer *&areaRenderer,
                             BarsRenderer *&barsRenderer,
                             PieRenderer *&pieRenderer,
                             PointRenderer *&pointRenderer,
                             CustomRenderer *&customRenderer,
                             QQuickItem *parent)
    : QCanvasPainterItem(parent)
    , m_areaRenderer(areaRenderer)
    , m_barsRenderer(barsRenderer)
    , m_pieRenderer(pieRenderer)
    , m_pointRenderer(pointRenderer)
    , m_customRenderer(customRenderer)
{}

QCPainterItemRenderer *QCPainterItem::createItemRenderer() const
{
    return new QCPainterItemRenderer();
}

QT_END_NAMESPACE
