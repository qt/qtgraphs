// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "qcpainteritem_p.h"

QT_BEGIN_NAMESPACE

QCPainterItemRenderer::QCPainterItemRenderer() = default;

QCPainterItemRenderer::~QCPainterItemRenderer()
{
#if QT_CONFIG(graphs_2d_custom)
    qDeleteAll(m_customPainters);
#endif
}

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
#if QT_CONFIG(graphs_2d_area)
        case RendererKind::Area:
            AreaRenderer::paintSnapshot(m_areaPaintSnapshot, p);
            break;
#endif
#if QT_CONFIG(graphs_2d_bar)
        case RendererKind::Bars:
            BarsRenderer::paintSnapshot(m_barsSnapshot, p);
            break;
#endif
#if QT_CONFIG(graphs_2d_donut_pie)
        case RendererKind::Pie:
            PieRenderer::paintSnapshot(m_piePaintSnapshot, p);
            break;
#endif
#if QT_CONFIG(graphs_2d_area) || QT_CONFIG(graphs_2d_line) || QT_CONFIG(graphs_2d_scatter) || QT_CONFIG(graphs_2d_spline)
        case RendererKind::Point:
            PointRenderer::paintSnapshot(m_pointPaintSnapshot, p);
            break;
#endif
#if QT_CONFIG(graphs_2d_custom)
        case RendererKind::Custom:
            CustomRenderer::paintSnapshot(m_customPaintSnapshot, p);
            break;
#endif
        }
    }
}

void QCPainterItemRenderer::synchronizeData(QCanvasPainterItem *item)
{
    auto graphsPainterItem = static_cast<QCPainterItem *>(item);
    std::array<QQuickItem *, 5> renderers = {
#if QT_CONFIG(graphs_2d_area)
        graphsPainterItem->m_areaRenderer,
#endif
#if QT_CONFIG(graphs_2d_bar)
        graphsPainterItem->m_barsRenderer,
#endif
#if QT_CONFIG(graphs_2d_donut_pie)
        graphsPainterItem->m_pieRenderer,
#endif
#if QT_CONFIG(graphs_2d_area) || QT_CONFIG(graphs_2d_line) || QT_CONFIG(graphs_2d_scatter) || QT_CONFIG(graphs_2d_spline)
        graphsPainterItem->m_pointRenderer,
#endif
#if QT_CONFIG(graphs_2d_custom)
        graphsPainterItem->m_customRenderer
#endif
    };
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
#if QT_CONFIG(graphs_2d_area)
        if (auto area = qobject_cast<AreaRenderer *>(renderer)) {
            area->synchronizeData();
            m_areaPaintSnapshot = area->paintSnapshot();
            m_rendererSlots.append(RendererKind::Area);
            continue;
        }
#endif
#if QT_CONFIG(graphs_2d_bar)
        if (auto bars = qobject_cast<BarsRenderer *>(renderer)) {
            bars->synchronizeData();
            m_barsSnapshot = bars->paintSnapshot();
            m_rendererSlots.append(RendererKind::Bars);
            continue;
        }
#endif
#if QT_CONFIG(graphs_2d_donut_pie)
        if (auto pie = qobject_cast<PieRenderer *>(renderer)) {
            pie->synchronizeData();
            m_piePaintSnapshot = pie->paintSnapshot();
            m_rendererSlots.append(RendererKind::Pie);
            continue;
        }
#endif
#if QT_CONFIG(graphs_2d_area) || QT_CONFIG(graphs_2d_line) || QT_CONFIG(graphs_2d_scatter) || QT_CONFIG(graphs_2d_spline)
        if (auto point = qobject_cast<PointRenderer *>(renderer)) {
            point->synchronizeData();
            m_pointPaintSnapshot = point->paintSnapshot();
            m_rendererSlots.append(RendererKind::Point);
            continue;
        }
#endif
#if QT_CONFIG(graphs_2d_custom)
        if (auto custom = qobject_cast<CustomRenderer *>(renderer)) {
            custom->synchronizeData(item, m_customPainters);
            m_customPaintSnapshot = custom->paintSnapshot();
            m_rendererSlots.append(RendererKind::Custom);
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
