// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
#include "qcpainteritem_p.h"

#include <private/arearenderer_p.h>
#include <private/barsrenderer_p.h>
#include <private/customrenderer_p.h>
#include <private/pierenderer_p.h>
#include <private/pointrenderer_p.h>

QT_BEGIN_NAMESPACE

QCPainterItemRenderer::QCPainterItemRenderer(AreaRenderer *&areaRenderer,
                                             BarsRenderer *&barsRenderer,
                                             PieRenderer *&pieRenderer,
                                             PointRenderer *&pointRenderer,
                                             CustomRenderer *&customRenderer)
    : areaRenderer(areaRenderer)
    , barsRenderer(barsRenderer)
    , pieRenderer(pieRenderer)
    , pointRenderer(pointRenderer)
    , customRenderer(customRenderer)
{}

void QCPainterItemRenderer::paint(QCanvasPainter *p)
{
    std::array<QQuickItem *, 5> renderers = {areaRenderer,
                                             barsRenderer,
                                             pieRenderer,
                                             pointRenderer,
                                             customRenderer};

    std::sort(renderers.begin(), renderers.end(), [](const QQuickItem *lhs, const QQuickItem *rhs) {
        if (!lhs)
            return false;
        if (!rhs)
            return true;
        return lhs->z() < rhs->z();
    });

    for (auto renderer : renderers) {
        if (auto area = qobject_cast<AreaRenderer *>(renderer))
            area->canvasPaint(p);
        else if (auto bars = qobject_cast<BarsRenderer *>(renderer))
            bars->canvasPaint(p);
        else if (auto pie = qobject_cast<PieRenderer *>(renderer))
            pie->canvasPaint(p);
        else if (auto point = qobject_cast<PointRenderer *>(renderer))
            point->canvasPaint(p);
        else if (auto custom = qobject_cast<CustomRenderer *>(renderer))
            custom->canvasPaint(p);
    }
}

QCPainterItem::QCPainterItem(AreaRenderer *&areaRenderer,
                             BarsRenderer *&barsRenderer,
                             PieRenderer *&pieRenderer,
                             PointRenderer *&pointRenderer,
                             CustomRenderer *&customRenderer,
                             QQuickItem *parent)
    : QCanvasPainterItem(parent)
    , areaRenderer(areaRenderer)
    , barsRenderer(barsRenderer)
    , pieRenderer(pieRenderer)
    , pointRenderer(pointRenderer)
    , customRenderer(customRenderer)
{}

QCPainterItemRenderer *QCPainterItem::createItemRenderer() const
{
    return new QCPainterItemRenderer(areaRenderer, barsRenderer, pieRenderer, pointRenderer, customRenderer);
}

QT_END_NAMESPACE
